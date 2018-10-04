#include "led.h"

void led_init(void)
{
	u32 tmp;

	clock_enable(CLKSRC_GPIO);

	// GPF4 set output
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (4 * 2));
	tmp |= (0x1 & 0x3) << (4 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPF5 set output
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (5 * 2));
	tmp |= (0x1 & 0x3) << (5 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPF6 set output
	tmp = *(vu32 *)GPFCON;
	tmp &= ~((0x3 & 0x3) << (6 * 2));
	tmp |= (0x1 & 0x3) << (6 * 2);
	*(vu32 *)GPFCON = tmp;

	// GPF4, GPF5, GPF6 set to be high
	*(vu32 *)GPFDAT |= 1 << 4;
	*(vu32 *)GPFDAT |= 1 << 5;
	*(vu32 *)GPFDAT |= 1 << 6;
}

void led_ctrl(u32 select, bool light)
{
	u8 i;

	// there are 3 leds in total
	for(i = 0; i < 3; i ++)
	{
		if((select >> i) & 1)
		{
			switch(i)
			{
			case 0:
				if(light)
				{
					*(vu32 *)GPFDAT &= ~(1 << 4);
				}
				else
				{
					*(vu32 *)GPFDAT |= 1 << 4;
				}
				break;

			case 1:
				if(light)
				{
					*(vu32 *)GPFDAT &= ~(1 << 5);
				}
				else
				{
					*(vu32 *)GPFDAT |= 1 << 5;
				}
				break;

			case 2:
				if(light)
				{
					*(vu32 *)GPFDAT &= ~(1 << 6);
				}
				else
				{
					*(vu32 *)GPFDAT |= 1 << 6;
				}
				break;
			}
		}
	}
}
