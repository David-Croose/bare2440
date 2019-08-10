#include <string.h>
#include "boot.h"
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

int main(void)
{
    int n = 0;

    *(vu32 *)INTMSK = 0xFFFFFFFF;       /* close all irq */
	create_page_table();
	mmu_init();
#ifdef BOOTING_FROM_NORFLASH
    move_program();
#endif
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

	lcd_init();
	lcd_clear(5 << 11);
	lcd_draw_point(10, 10, 0xFFFF);
	lcd_draw_point(100, 10, 0xFFFF);
    lcd_fill_rect(20, 60, 80, 100, (n += 579, n));

	while(1)
	{
		led_ctrl(1, 1);
		delay_rough_ms(5000);
		led_ctrl(1, 0);
		delay_rough_ms(5000);
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

