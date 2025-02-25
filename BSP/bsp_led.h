#ifndef _BSP_LED_H_
#define _BSP_LED_H_


#include "stm32f4xx.h"


void Led_init(void);
void SetLEDState(uint8_t LEDX, uint8_t LEDColor);

#endif

