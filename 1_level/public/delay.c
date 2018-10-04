#include "delay.h"

void delay_rough_us(u32 n)
{
	u32 i;

	while(n --)
	{
		for(i = 0; i < 10; i ++);
	}
}

void delay_rough_ms(u32 n)
{
	u32 i;

	while(n --)
	{
		for(i = 0; i < 10000; i ++);
	}
}

void delay_us(u32 n)
{

}

void delay_ms(u32 n)
{

}

void delay_calibration(void)
{

}
