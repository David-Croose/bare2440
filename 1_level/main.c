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
    /// memcpy((void *)0x30100000, &__new_vector_start, &__new_vector_end - &__new_vector_start);

    volatile unsigned int *d, *s;

    for (d = (volatile unsigned int *)0x30100000, s = &__new_vector_start;
         s < &__new_vector_end; d++, s++) {
        *d = *s;
    }
}

int main(void)
{
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
	/// key_irq_enable();
	/// timer0_init(1000);
	/// timer4_init(100);
	my_printf("start\r\n");
	my_printf("__new_vector_start=0x%08x\r\n", &__new_vector_start);
	my_printf("__new_vector_end=0x%08x\r\n", &__new_vector_end);

	/// lcd_init();
	/// lcd_clear(5 << 11);
	/// lcd_draw_point(10, 10, 0xFFFF);

	while(1)
	{
		led_ctrl(1, 1);
		delay_rough_ms(100);
		led_ctrl(1, 0);
		delay_rough_ms(100);
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

