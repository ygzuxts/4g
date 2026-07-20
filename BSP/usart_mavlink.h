#ifndef __USART_4GMOUDLE_H_
#define __USART_4GMOUDLE_H_

#include "stm32f4xx.h"

/*
 * Temporary debug switch:
 * 1 = use the flight-controller UART (USART2) as a readable ASCII debug port.
 *     Binary MAVLink output is suppressed so a serial assistant can show
 *     AT/NTRIP logs clearly.
 * 0 = normal product mode. USART2 sends real MAVLink data to the flight
 *     controller.
 *
 * Change this back to 0 before testing RTK with the flight controller.
 */
#define USART2_TEXT_DEBUG_ONLY 0

void USART2_SendBytes(uint8_t *buf, uint16_t len);
void USART2_SendDebugText(const char *text);
void USART2_SendMavlinkBytes(uint8_t *buf, uint16_t len);
void USART2_ReportInfo(const char *text);
void USART2_ReportWarning(const char *text);
void MX_USART2_UART_Init(void);
extern UART_HandleTypeDef huart2;
#endif
