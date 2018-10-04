/**
 * LED driver
 * LED0 --- GPF4
 * LED1 --- GPF5
 * LED2 --- GPF6
 * low voltage lights up, high voltage lights off
 */

#ifndef _LED_H_
#define _LED_H_

#include "s3c2440.h"

void led_init(void);
void led_ctrl(u32 select, bool light);

#endif
