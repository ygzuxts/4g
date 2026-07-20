#ifndef _USART_FIFO_H_//作者：恒久力行  qq:624668529
#define _USART_FIFO_H_
#include "stdint.h" 
#define true 1
#define false 0
	

typedef struct _fifo {
	uint8_t* buf;
	uint16_t length;
	uint16_t head;
	uint16_t tail;
} fifo_t;

extern fifo_t mavlink_uart_rx_fifo;
extern volatile uint32_t mavlink_rx_byte_count;

uint8_t fifo_read_ch(fifo_t* fifo, uint8_t* ch);
uint8_t fifo_write_ch(fifo_t* fifo, uint8_t ch);
uint16_t fifo_free(fifo_t* fifo);
uint16_t fifo_used(fifo_t* fifo);
void fifo_init(fifo_t* fifo, uint8_t* buf, uint16_t length);
uint8_t serial_write_buf(uint8_t* buf, uint16_t length);
uint8_t serial_read_ch(fifo_t *fifo);
uint16_t serial_free(fifo_t *fifo);
uint16_t serial_available(fifo_t *fifo);
#endif  /*_USART_FIFO_H_*/
