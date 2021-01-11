#ifndef _DELAY_H
#define _DELAY_H
#include "driverlib.h"

#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 48000000
#endif

void delay_init(uint32_t SystemCoreClock);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif