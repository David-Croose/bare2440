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

int main(void)
{
	create_page_table();
	mmu_init();
	s3c2440_clock_init();
	uart0_init(115200);
	uart0_set_fifo(16, 16);
	uart0_tx_irq(TRUE);
	uart0_rx_irq(TRUE);
	nand_init();
	led_init();
	key_irq_enable();
	/// timer0_init(1000);
	/// timer4_init(100);
	my_printf("start\r\n");

	lcd_init();
	/// lcd_clear(5 << 11);
	lcd_draw_point(10, 10, 0xFFFF);

	while(1)
	{
		/// lcd_clear(0x1234);
		/// delay_rough_ms(100);
		/// lcd_clear(0x4321);
		/// delay_rough_ms(100);
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

