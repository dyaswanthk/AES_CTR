#ifndef TIMER_H
#define TIMER_H

#include "stm32f756xx.h"
#include "sys_clk.h"

#define ISR_FREQ 10

void timer2_init(void);

#endif