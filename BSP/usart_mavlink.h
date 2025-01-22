#ifndef __USART_4GMOUDLE_H_
#define __USART_4GMOUDLE_H_

#include "stm32f4xx.h"
void USART2_SendBytes(uint8_t *buf, uint16_t len);
void MX_USART2_UART_Init(void);
extern UART_HandleTypeDef huart2;
#endif
