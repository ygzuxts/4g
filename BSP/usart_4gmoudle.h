#ifndef _USART_MAVLINK_H_
#define _USART_MAVLINK_H_

#include "stm32f4xx.h"

extern UART_HandleTypeDef huart3;
void USART3_SendBytes(uint8_t *buf, uint16_t len);
void MX_USART3_UART_Init(void);
uint8_t usrMoudle_Clear(void);
uint8_t usrMoudle_Init(void);
void USRMoudle_uart_rx_restart(void);
uint8_t *USRMoudle_uart_rx_get_frame(void);
uint16_t USRMoudle_uart_rx_get_frame_len(void);
void USRMoudle_uart_printf(char *fmt, ...);
#endif
