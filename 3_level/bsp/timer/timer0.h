#ifndef _TIMER0_H_
#define _TIMER0_H_

#include "s3c2440.h"
#include "config.h"

void timer0_init(u32 nms);
void timer0_enable(void);
void timer0_disable(void);
void timer0_overflow_irq_handler(vu32 intpnd);

#endif

