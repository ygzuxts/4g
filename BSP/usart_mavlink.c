#include "usart_mavlink.h"
#include "mavlink_parse.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */
    /* USER CODE BEGIN USART2_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
    
    /* USER CODE BEGIN USART2_Init 2 */
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    /* USER CODE END USART2_Init 2 */
}


void USART2_SendBytes(uint8_t *buf, uint16_t len)
{
    HAL_UART_Transmit(&huart2, buf, len,1000); // 串口发送数据
}

void USART2_SendDebugText(const char *text)
{
#if USART2_TEXT_DEBUG_ONLY
    HAL_UART_Transmit(&huart2, (uint8_t *)text, strlen(text), 1000);
#else
    (void)text;
#endif
}

static void USART2_SendStatusText(uint8_t severity, const char *text)
{
#if USART2_TEXT_DEBUG_ONLY
    char debug_buf[80];
    snprintf(debug_buf, sizeof(debug_buf), "STATUS: %.50s\r\n", text);
    USART2_SendDebugText(debug_buf);
#else
    mavlink_message_t message;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    char status_text[50] = {0};

    strncpy(status_text, text, sizeof(status_text) - 1);
    mavlink_msg_statustext_pack(1, 191, &message, severity, status_text, 0, 0);
    uint16_t length = mavlink_msg_to_send_buffer(buffer, &message);
    USART2_SendBytes(buffer, length);
#endif
}

void USART2_ReportInfo(const char *text)
{
    USART2_SendStatusText(MAV_SEVERITY_INFO, text);
}

void USART2_ReportWarning(const char *text)
{
    USART2_SendStatusText(MAV_SEVERITY_WARNING, text);
}

void USART2_SendMavlinkBytes(uint8_t *buf, uint16_t len)
{
#if USART2_TEXT_DEBUG_ONLY
    static uint32_t suppressed_count = 0;
    char debug_buf[64];

    suppressed_count++;
    snprintf(debug_buf, sizeof(debug_buf), "DBG: mavlink suppressed len=%u cnt=%lu\r\n",
             len, (unsigned long)suppressed_count);
    USART2_SendDebugText(debug_buf);
#else
    USART2_SendBytes(buf, len);
#endif
}
