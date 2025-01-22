#ifndef __OPEN_TEL_MAVLINK_H
#define __OPEN_TEL_MAVLINK_H
//#include "./minimal/minimal/minimal.h"
#include "define.h"
#include "stdint.h"
#include "usart_debug.h"
#include "common.h"
#include "mavlink_helpers.h"
#include "mavlink_usart_fifo"
#define Pi 3.1415926


extern fifo_t mavlink_uart_rx_fifo;

extern mavlink_system_t mavlink_system;



void update(void);
void handleMessage(mavlink_message_t msg);

#endif /*__OPENTEL_MAVLINK_H*/
