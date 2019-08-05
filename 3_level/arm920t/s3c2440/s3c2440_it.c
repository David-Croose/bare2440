#include "s3c2440_it.h"

u32 get_cpsr(void);

void reset_irq_handler(void)
{
	my_printf("reset_irq_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

void undef_irq_handler(void)
{
	my_printf("undef_irq_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

void swi_irq_handler(void)
{
	my_printf("swi_irq_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

void prefetch_abort_handler(void)
{
	my_printf("prefetch_abort_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

void data_abort_handler(void)
{
	my_printf("data_abort_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

void not_used_handler(void)
{
	my_printf("not_used_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

/**
 * add your irq hander below
 */
void irq_handler(void)
{
	/// my_printf("irq_handler, cpsr = %#x\r\n", get_cpsr());
	u32 i;

	vu32 intpnd    = *(vu32 *)INTPND;
	vu32 subsrcpnd = *(vu32 *)SUBSRCPND;
	vu32 eintpnd   = *(vu32 *)EINTPEND;
	vu32 intmsk    = *(vu32 *)INTMSK;
	vu32 intsubmsk = *(vu32 *)INTSUBMSK;

	intpnd &= ~intmsk;
	subsrcpnd &= ~intsubmsk;

	uart0_irq_handler(intpnd, subsrcpnd);
	timer0_overflow_irq_handler(intpnd);
	timer4_overflow_irq_handler(intpnd);
	key_irq_handler(intpnd, eintpnd);

	// make sure the pending registers had been clear out
	for(i = 0; i < 3; i++)
	{
		*(vu32 *)SRCPND    = *(vu32 *)SRCPND;
		*(vu32 *)INTPND    = *(vu32 *)INTPND;
		*(vu32 *)SUBSRCPND = *(vu32 *)SUBSRCPND;
		*(vu32 *)EINTPEND  = *(vu32 *)EINTPEND;
	}

	// no need to care SRCPND whether it has been cleared
	if((*(vu32 *)INTPND) != 0)
	{
		my_printf("Warning : INTPND(0x%X) had not been clear over yet !\r\n", *(vu32 *)INTPND);
	}
	if((*(vu32 *)SUBSRCPND) != 0)
	{
		my_printf("Warning : SUBSRCPND(0x%X) had not been clear over yet !\r\n", *(vu32 *)SUBSRCPND);
	}
	if((*(vu32 *)EINTPEND) != 0)
	{
		my_printf("Warning : EINTPEND(0x%X) had not been clear over yet !\r\n", *(vu32 *)EINTPEND);
	}
}

void fiq_handler(void)
{
	my_printf("fiq_handler, cpsr = %#x\r\n", get_cpsr());
	while(1)
	{

	}
}

