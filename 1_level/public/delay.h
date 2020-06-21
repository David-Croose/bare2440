#ifndef _DELAY_H_
#define _DELAY_H_

#include "def.h"

void delay_rough_us(u32 n);
void delay_rough_ms(u32 n);
void delay_us(u32 n);
void delay_ms(u32 n);
void delay_calibration(void);

#endif
