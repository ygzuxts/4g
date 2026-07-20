#include "bsp_can.h"
#include <stdio.h>

CAN_HandleTypeDef hcan1;

static uint8_t can_started = 0;
static uint8_t last_state = BSP_CAN_STATE_BOOT;
static uint8_t last_error = 0;
static uint32_t last_value = 0;
static uint32_t last_heartbeat_tick = 0;
static uint8_t reported_state_mask = 0;
static uint8_t last_reported_error = 0xFF;

static HAL_StatusTypeDef BSP_CAN_SendFrame(uint8_t state, uint8_t error, uint32_t value)
{
    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    uint8_t data[8];

    if (!can_started || HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0U)
    {
        return HAL_BUSY;
    }

    header.StdId = BSP_CAN_DEBUG_STD_ID;
    header.ExtId = 0;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.DLC = 8;
    header.TransmitGlobalTime = DISABLE;

    /* A5 01 makes the debug frame easy to recognize in a USB-CAN tool. */
    data[0] = 0xA5;
    data[1] = 0x01;
    data[2] = state;
    data[3] = error;
    data[4] = (uint8_t)value;
    data[5] = (uint8_t)(value >> 8);
    data[6] = (uint8_t)(value >> 16);
    data[7] = (uint8_t)(value >> 24);

    return HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox);
}

HAL_StatusTypeDef BSP_CAN1_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    CAN_FilterTypeDef filter = {0};

    __HAL_RCC_CAN1_CLK_ENABLE();

    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF9_CAN1;

#if (BSP_CAN1_PINMAP == BSP_CAN1_PINMAP_PB8_PB9)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &gpio);
#elif (BSP_CAN1_PINMAP == BSP_CAN1_PINMAP_PD0_PD1)
    __HAL_RCC_GPIOD_CLK_ENABLE();
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOD, &gpio);
#else
#error "Unsupported BSP_CAN1_PINMAP"
#endif

    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 3;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = ENABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK ||
        HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    can_started = 1;
    last_heartbeat_tick = HAL_GetTick();
    return BSP_CAN_SetDebugStatus(BSP_CAN_STATE_BOOT, 0, 0);
}

HAL_StatusTypeDef BSP_CAN_SetDebugStatus(uint8_t state, uint8_t error, uint32_t value)
{
    HAL_StatusTypeDef result;
    uint8_t should_report = 0;

    last_state = state;
    last_error = error;
    last_value = value;
    result = BSP_CAN_SendFrame(state, error, value);

    /* USART3 only: report each normal state once and report error changes. */
    if (state >= BSP_CAN_STATE_BOOT && state <= BSP_CAN_STATE_RTCM_MAVLINK_SENT)
    {
        uint8_t state_bit = (uint8_t)(1U << (state - 1U));
        if ((reported_state_mask & state_bit) == 0U)
        {
            reported_state_mask |= state_bit;
            should_report = 1;
        }
    }
    else if (state == BSP_CAN_STATE_ERROR && error != last_reported_error)
    {
        last_reported_error = error;
        should_report = 1;
    }

    if (should_report)
    {
        printf("DBG: can tx id=301 state=%u err=%u value=%lu result=%u halerr=%08lX\r\n",
               state,
               error,
               (unsigned long)value,
               (unsigned int)result,
               (unsigned long)HAL_CAN_GetError(&hcan1));
    }

    return result;
}

void BSP_CAN_DebugHeartbeat(void)
{
    uint32_t now = HAL_GetTick();

    if (can_started && (now - last_heartbeat_tick) >= 1000U)
    {
        last_heartbeat_tick = now;
        BSP_CAN_SendFrame(last_state, last_error, last_value);
    }
}
