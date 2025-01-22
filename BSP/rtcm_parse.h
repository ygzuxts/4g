#ifndef _RTCM_H_
#define _RTCM_H_

#include "stm32f4xx.h"
#include "stdint.h"
#include "stdbool.h"



 void process_rtcm_data(uint8_t *buffer, uint16_t length);
#endif



