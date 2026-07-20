#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/*
 * CAN1 pin trial selection. Only one pin pair is configured at a time.
 * Start with PB8/PB9. If CAN ID 0x301 is not visible, change this to
 * BSP_CAN1_PINMAP_PD0_PD1, rebuild, and flash again.
 */
#define BSP_CAN1_PINMAP_PB8_PB9  0U
#define BSP_CAN1_PINMAP_PD0_PD1  1U
#define BSP_CAN1_PINMAP           BSP_CAN1_PINMAP_PB8_PB9

#define BSP_CAN_DEBUG_STD_ID      0x301U

typedef enum
{
    BSP_CAN_STATE_BOOT = 1,
    BSP_CAN_STATE_4G_CONFIGURED = 2,
    BSP_CAN_STATE_NTRIP_REQUEST_SENT = 3,
    BSP_CAN_STATE_NTRIP_ACCEPTED = 4,
    BSP_CAN_STATE_RTCM_RECEIVED = 5,
    BSP_CAN_STATE_RTCM_MAVLINK_SENT = 6,
    BSP_CAN_STATE_ERROR = 0x80
} BSP_CAN_DebugState;

extern CAN_HandleTypeDef hcan1;

HAL_StatusTypeDef BSP_CAN1_Init(void);
HAL_StatusTypeDef BSP_CAN_SetDebugStatus(uint8_t state, uint8_t error, uint32_t value);
void BSP_CAN_DebugHeartbeat(void);

#endif
