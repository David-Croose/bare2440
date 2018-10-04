/**
 * key driver, 4 key in total, each of them has pull-up resistance
 * KEY2 --- EINT0  --- GPF0
 * KEY3 --- EINT2  --- GPF2
 * KEY4 --- EINT11 --- GPG3
 * KEY5 --- EINT19 --- GPG11
 */

#ifndef _KEY_H_
#define _KEY_H_

#include "s3c2440.h"

#define KEY_NONE_PRESSED    (-1)
#define KEY_2_PRESSED        2
#define KEY_3_PRESSED        3
#define KEY_4_PRESSED        4
#define KEY_5_PRESSED        5

void key_init(void);
s8 key_scan(void);
void key_irq_enable(void);
void key_irq_handler(vu32 intpnd, vu32 eintpnd);

#endif
