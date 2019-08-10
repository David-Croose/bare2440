#include <string.h>
#include "config.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "uart0.h"
#include "timer0.h"
#include "timer4.h"
#include "nand.h"
#include "lcd.h"
#include "printf.h"
#include "s3c2440_it.h"

extern unsigned int __new_vector_start;
extern unsigned int __new_vector_end;

static void move_program(void)
{
    volatile unsigned int *d, *s;

    /*
     * it won't work if you use memcpy here
     */
    for (d = (volatile unsigned int *)0x30100000, s = &__new_vector_start;
         s < &__new_vector_end; d++, s++) {
        *d = *s;
    }
}

int main(void)
{
    int n = 0;

    *(vu32 *)INTMSK = 0xFFFFFFFF;       /* close all irq */
	create_page_table();
	mmu_init();
    move_program();
	s3c2440_clock_init();
	uart0_init(115200);
	uart0_set_fifo(16, 16);
	uart0_tx_irq(TRUE);
	uart0_rx_irq(TRUE);
	nand_init();
	led_init();
	led_ctrl(1, 0);
	led_ctrl(2, 0);
	led_ctrl(4, 0);
	key_irq_enable();
	timer0_init(1000);
    timer0_disable();
	timer4_init(100);
    timer4_disable();
	my_printf("start\r\n");
	my_printf("__new_vector_start=0x%08x\r\n", &__new_vector_start);
	my_printf("__new_vector_end=0x%08x\r\n", &__new_vector_end);

	lcd_init();
	lcd_clear(5 << 11);
	lcd_draw_point(10, 10, 0xFFFF);
	lcd_draw_point(100, 10, 0xFFFF);

	while(1)
	{
		led_ctrl(1, 1);
		delay_rough_ms(5000);
		led_ctrl(1, 0);
		delay_rough_ms(5000);
        lcd_clear((n += 579, n));
        /// lcd_fill_rect(20, 180, 400, 270, (n += 579, n));
	}
}

/**
 * this function is just for fixing the error of linking :
 * undefined reference to `raise'
 */
int raise(int a)
{
	return 0;
}

