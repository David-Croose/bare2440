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
#include "tusb.h"

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

	//===============================================================
	/// these code below will cause data-abort bug
	/// lcd_init();
	/// lcd_clear(5 << 11);
	/// lcd_draw_point(10, 10, 0xFFFF);
	/// lcd_draw_point(100, 10, 0xFFFF);
    /// lcd_fill_rect(20, 60, 80, 100, (n += 579, n));
	//===============================================================

	//===============================================================
	// USB initialization
	*(vu32 *)MISCCR |= 1 << 13;			// USB Port1 in suspend mode, can be Host or Device
	*(vu32 *)MISCCR &= ~(1 << 12);		// USB Port0 in normal mode, can be Host only

	clock_enable(CLKSRC_USBH);			// USB clock enable
	*(vu32 *)INTMSK &= ~(1 << 26);		// USB Host interrupt enable
	//===============================================================

	tusb_init();

	n = 0;
	while(1)
	{
		tuh_task();
		led_ctrl(1, (++n / 100000 % 2));
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

