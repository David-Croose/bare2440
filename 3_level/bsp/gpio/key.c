#include "key.h"
#include "printf.h"

void key_init(void)
{
	u32 tmp;

	clock_enable(CLKSRC_GPIO);

	// GPF0 set input
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (0 * 2));
	tmp |= (0x0 & 0x3) << (0 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPF2 set input
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (2 * 2));
	tmp |= (0x0 & 0x3) << (2 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPG3 set input
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (3 * 2));
	tmp |= (0x0 & 0x3) << (3 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPG11 set input
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (11 * 2));
	tmp |= (0x0 & 0x3) << (11 * 2);
	*(vu32 *)GPFCON = tmp;
}

/**
 * return 2, 3, 4, 5 --- KEY2, KEY3, KEY4, KEY5
 *        -1         --- no key pressed
 */
s8 key_scan(void)
{
	if(!((*(vu32 *)GPFDAT) & (1 << 0)))
	{
		return KEY_2_PRESSED;
	}
	if(!((*(vu32 *)GPFDAT) & (1 << 2)))
	{
		return KEY_3_PRESSED;
	}
	if(!((*(vu32 *)GPGDAT) & (1 << 3)))
	{
		return KEY_4_PRESSED;
	}
	if(!((*(vu32 *)GPGDAT) & (1 << 11)))
	{
		return KEY_5_PRESSED;
	}

	return KEY_NONE_PRESSED;
}

/**
 *
 */
void key_irq_enable(void)
{
	u32 tmp;

	//==============================================
	// GPF0 set as EINT0
	tmp = *(vu32 *)GPFCON;
	tmp &= ~3;
	tmp |= 2;
	*(vu32 *)GPFCON = tmp;

	// GPF0 set falling edge trigger
	tmp = *(vu32 *)EXTINT0;
	tmp &= ~7;
	tmp |= 2;
	*(vu32 *)EXTINT0 = tmp;

	// GPF0 set unmask
	tmp = *(vu32 *)INTMSK;
	tmp &= ~1;
	*(vu32 *)INTMSK = tmp;

	//==============================================
	// GPF2 set as EINT2
	tmp = *(vu32 *)GPFCON;
	tmp &= ~(3 << 4);
	tmp |= (2 & 3) << 4;
	*(vu32 *)GPFCON = tmp;

	// GPF2 set falling edge trigger
	tmp = *(vu32 *)EXTINT0;
	tmp &= ~(7 << 8);
	tmp |= (2 & 7) << 8;
	*(vu32 *)EXTINT0 = tmp;

	// GPF2 set unmask
	tmp = *(vu32 *)INTMSK;
	tmp &= ~(1 << 2);
	*(vu32 *)INTMSK = tmp;

	//==============================================
	// GPG3 set as EINT11
	tmp = *(vu32 *)GPGCON;
	tmp &= ~(3 << 6);
	tmp |= (2 & 3) << 6;
	*(vu32 *)GPGCON = tmp;

	// GPG3 set falling edge trigger
	tmp = *(vu32 *)EXTINT1;
	tmp &= ~(7 << 12);
	tmp |= (2 & 7) << 12;
	*(vu32 *)EXTINT1 = tmp;

	// GPG3 set unmask
	tmp = *(vu32 *)EINTMASK;
	tmp &= ~(1 << 11);
	*(vu32 *)EINTMASK = tmp;

	// GPG3 set unmask
	tmp = *(vu32 *)INTMSK;
	tmp &= ~(1 << 5);
	*(vu32 *)INTMSK = tmp;

	//==============================================
	// GPG11 set as EINT19
	tmp = *(vu32 *)GPGCON;
	tmp &= ~(3 << 22);
	tmp |= (2 & 3) << 22;
	*(vu32 *)GPGCON = tmp;

	// GPG11 set falling edge trigger
	tmp = *(vu32 *)EXTINT2;
	tmp &= ~(7 << 12);
	tmp |= (2 & 7) << 12;
	*(vu32 *)EXTINT2 = tmp;

	// GPG11 set unmask
	tmp = *(vu32 *)EINTMASK;
	tmp &= ~(1 << 19);
	*(vu32 *)EINTMASK = tmp;

	// GPG11 set unmask
	tmp = *(vu32 *)INTMSK;
	tmp &= ~(1 << 5);
	*(vu32 *)INTMSK = tmp;
}

void key_irq_handler(vu32 intpnd, vu32 eintpnd)
{
	//===============================================
	// key2 GPF0 EINT0
	if(intpnd & 1)
	{
		my_printf("key2 irq\r\n");
	}

	//===============================================
	// key3 GPF2 EINT2
	if(intpnd & (1 << 2))
	{
		my_printf("key3 irq\r\n");
	}	

	//===============================================
	// key4 GPG3 EINT11
	// key5 GPG11 EINT19
	if(intpnd & (1 << 5))
	{
		if(eintpnd & (1 << 11))
		{
			my_printf("key4 irq\r\n");
		}

		if(eintpnd & (1 << 19))
		{
			my_printf("key5 irq\r\n");
		}
	}
}

