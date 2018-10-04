#include "timer4.h"
#include "printf.h"

/**
 * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
 * {prescaler value} = 0~255
 * {divider value} = 2, 4, 8, 16
 *
 * @param nms : million seconds to count
 */
void timer4_init(u32 nms)
{
	u32 prescaler;    // [0,255]
	u32 divider;      // [2,4,8,16]
	u32 _divider;
	u32 counter;
	u32 tmp;

	clock_enable(CLKSRC_PWMTIMER);

	if(pclk != 50000000)
	{
		my_printf("warning, pclk != 50000000, the timer4's accuracy become unsure\r\n");
	}

	switch(nms)
	{
	case 0:
		return;

	case 1:
		prescaler = 249;
		divider = 8;
		counter = 25;
		break;

	case 5:
		prescaler = 249;
		divider = 8;
		counter = 125;
		break;

	case 10:
		prescaler = 249;
		divider = 8;
		counter = 250;
		break;

	case 20:
		prescaler = 249;
		divider = 8;
		counter = 500;
		break;

	case 50:
		prescaler = 249;
		divider = 8;
		counter = 1250;
		break;

	case 100:
		prescaler = 249;
		divider = 8;
		counter = 2500;
		break;

	case 500:
		prescaler = 249;
		divider = 8;
		counter = 12500;
		break;

	case 1000:
		prescaler = 99;
		divider = 16;
		counter = 31250;
		break;

	case 5000:
		prescaler = 249;
		divider = 16;
		counter = 62500;
		break;

	default:
		my_printf("nms %d is not supported in %d, %s !\r\n", nms, __LINE__, __FILE__);
		while(1);
	}

	switch(divider)
	{
	case 2:
		_divider = 0;
		break;
	case 4:
		_divider = 1;
		break;
	case 8:
		_divider = 2;
		break;
	case 16:
		_divider = 3;
		break;
	default:
		my_printf("divider error !\r\n");
	}

	tmp = *(vu32 *)TCFG0;
    tmp &= ~(0xFF << 8);
    tmp |= (prescaler & 0xFF) << 8;
	*(vu32 *)TCFG0 = tmp;

	tmp = *(vu32 *)TCFG1;
    tmp &= ~(0xF << 16);
    tmp |= (_divider & 0xF) << 16;
	*(vu32 *)TCFG1 = tmp;

	tmp = *(vu32 *)TCNTB4;
    tmp = counter & 0xFFFF;
	*(vu32 *)TCNTB4 = tmp;

	tmp = *(vu32 *)TCON;
    tmp &= ~(7 << 20);
	tmp |= 1 << 21;
	tmp |= 1 << 22;
	*(vu32 *)TCON = tmp;

    tmp &= ~(1 << 21);
    tmp |= 1 << 20;
	*(vu32 *)TCON = tmp;

	tmp = *(vu32 *)INTMSK;
	tmp &= ~(1 << 14);
	*(vu32 *)INTMSK = tmp;
}

void timer4_enable(void)
{
	u32 tmp;

	tmp = *(vu32 *)INTMSK;
	tmp &= ~(1 << 14);
	*(vu32 *)INTMSK = tmp;
}

void timer4_disable(void)
{
	u32 tmp;

	tmp = *(vu32 *)INTMSK;
	tmp |= 1 << 14;
	*(vu32 *)INTMSK = tmp;
}

void timer4_overflow_irq_handler(vu32 intpnd)
{
	if(intpnd & (1 << 14))
	{
		my_printf("timer4 irq\r\n");
	}
}
