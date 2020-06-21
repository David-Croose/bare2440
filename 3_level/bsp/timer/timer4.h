#ifndef _TIMER4_H_
#define _TIMER4_H_

#include "s3c2440.h"
#include "config.h"

void timer4_init(u32 nms);
void timer4_enable(void);
void timer4_disable(void);
void timer4_overflow_irq_handler(vu32 intpnd);

#endif
