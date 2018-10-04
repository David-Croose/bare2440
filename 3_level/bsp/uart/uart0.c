#include <string.h>
#include "uart0.h"
#include "led.h"
#include "printf.h"
#include "shell.h"

u8 uart0_recv_buf[UART0_RECV_BUF_SIZE];
u32 uart0_recv_index;

void uart0_tx_irq(bool enable_flag)
{
	if(enable_flag == TRUE)
	{
		*(vu32 *)INTSUBMSK &= ~(1 << 1);      // make TX0 irq available
	}
	else
	{
		*(vu32 *)INTSUBMSK |= 1 << 1;         // make TX0 irq not available
	}
}

void uart0_rx_irq(bool enable_flag)
{
	if(enable_flag == TRUE)
	{
		*(vu32 *)INTSUBMSK &= ~1;             // make RX0 irq available
	}
	else
	{
		*(vu32 *)INTSUBMSK |= 1;              // make RX0 irq not available
	}
}

void uart0_set_fifo(s32 tx_deep, s32 rx_deep)
{
	u8 tmp;
	u8 tx_fifo, rx_fifo;

	if(tx_deep == -1 && rx_deep == -1)
	{
		// disable fifo
		tmp = *(vu8 *)UFCON0;
		tmp &= ~1;
		*(vu8 *)UFCON0 = tmp;
		return;
	}

	switch(tx_deep)
	{
	case 0:
		tx_fifo = 0;
		break;
	case 16:
		tx_fifo = 1;
		break;
	case 32:
		tx_fifo = 2;	
		break;
	case 48:
		tx_fifo = 3;
		break;

	default:
		tx_fifo = 0;
	}

	switch(rx_deep)
	{
	case 1:
		rx_fifo = 0;
		break;
	case 8:
		rx_fifo = 1;
		break;
	case 16:
		rx_fifo = 2;	
		break;
	case 32:
		rx_fifo = 3;
		break;

	default:
		rx_fifo = 0;
	}

	// reset fifo
	tmp = *(vu8 *)UFCON0;
	tmp |= 6;
	*(vu8 *)UFCON0 = tmp;

	// set fifo
	tmp = 0;
	tmp |= tx_fifo << 6;
	tmp |= rx_fifo << 4;
	tmp |= 1;
	*(vu8 *)UFCON0 = tmp;
}

void uart0_init(u32 baudrate)
{
	u32 tmp;

	clock_enable(CLKSRC_UART0);

	// set GPH2 multiplexed as TX0
	tmp = *(vu32 *)GPHCON;
	tmp &= ~((0x3 & 0x3) << (2 * 2));
	tmp |= (0x2 & 0x3) << (2 * 2);
	*(vu32 *)GPHCON = tmp;

	// set GPH3 multiplexed as RX0
	tmp = *(vu32 *)GPHCON;
	tmp &= ~((0x3 & 0x3) << (3 * 2));
	tmp |= (0x2 & 0x3) << (3 * 2);
	*(vu32 *)GPHCON = tmp;

	// set GPH2 pull up
	tmp = *(vu32 *)GPHUP;
	tmp &= ~(1 << 2);
	*(vu32 *)GPHUP = tmp;

	// set GPH3 pull up
	tmp = *(vu32 *)GPHUP;
	tmp &= ~(1 << 3);
	*(vu32 *)GPHUP = tmp;

	tmp = 0;
	tmp |= (3 & 0x3) << 0;    // Normal mode operation
	tmp |= (0 & 0x1) << 2;    // No parity
	tmp |= (0 & 0x7) << 3;    // One stop bit per frame
	tmp |= (0 & 0x1) << 6;    // 8-bits data
	*(vu32 *)ULCON0 = tmp;

	tmp = 0;
	tmp |= (1 & 0x3) << 0;    // Rx Interrupt request or polling mode
	tmp |= (1 & 0x3) << 2;    // Tx Interrupt request or polling mode
	tmp |= (0 & 0x1) << 4;    // Don't send break signal while transmitting
	tmp |= (0 & 0x1) << 5;    // Don't use loopback mode
	tmp |= (1 & 0x1) << 6;    // Generate receive error status interrupt
	tmp |= (1 & 0x1) << 7;    // Disable Rx time out interrupt when UART FIFO is enabled. The interrupt is a receive interrupt
	tmp |= (0 & 0x1) << 8;    // Interrupt is requested the instant Rx buffer receivesthe data in Non-FIFO mode or reaches Rx FIFO Trigger Level inFIFO mode
	tmp |= (0 & 0x1) << 9;    // Interrupt is requested as soon as the Tx bufferbecomes empty in Non-FIFO mode or reaches Tx FIFO TriggerLevel in FIFO mode
	tmp |= (0 & 0x3) << 10;   // Select PCLK as the source clock of UART0
	*(vu32 *)UCON0 = tmp;

	// UBRDIVn = (int)( UART clock / ( buad rate x 16) ) –1
	*(vu32 *)UBRDIV0 = (pclk * 10 / baudrate / 16 % 10) >= 5
	                   ? (pclk / baudrate / 16 + 1 - 1)
					   : (pclk / baudrate / 16 - 1);

	*(vu32 *)SRCPND |= (1 << 28);    // clear uart0's irq request pending bit, whether it is masked or not
	*(vu32 *)INTPND |= (1 << 28);    // clear uart0's irq request pending bit 
	*(vu32 *)SUBSRCPND |= 1;         // clear uart0's rx irq pending flag

	*(vu32 *)INTMSK &= ~(1 << 28);   // make INT_UART0 Irq available
}

/**
 * sending a break signal
 */
void uart0_send_break(void)
{
	u32 tmp;

	tmp = *(vu32 *)UCON0;
	tmp |= 1 << 4;
	*(vu32 *)UCON0 = tmp;
}

// this function is servered for printf.c
void _putchar(char c)
{
	while(!((*(vu32 *)UTRSTAT0) & (1 << 2)));
	*(vu8 *)UTXH0 = (u8)c;
	while(!((*(vu32 *)UTRSTAT0) & (1 << 2)));
}

void uart0_send_byte(u8 sendbyte)
{
	while(!((*(vu32 *)UTRSTAT0) & (1 << 2)));
	*(vu8 *)UTXH0 = sendbyte;
	while(!((*(vu32 *)UTRSTAT0) & (1 << 2)));
}

u8 uart0_receive_byte(void)
{
	return *(vu8 *)URXH0;
}

void uart0_irq_handler(vu32 intpnd, vu32 subsrcpnd)
{
	extern bool had_got_msg(u8 byte, void (*callback)(u8 *buf, u32 len));
	vu8 ch;
	u32 i;
	vu32 len;

	if(intpnd & (1 << 28))
	{
		// if send done irq comes
		if(subsrcpnd & (1 << 1))
		{
		}

		// if receive done irq comes
		if(subsrcpnd & 1)
		{
			len = *(vu32 *)UFSTAT0 & 0x3F;
			for(i = 0; i < len; i++)
			{
				ch = uart0_receive_byte();
				// if you use a *term to oberve the uart, write 1 into the "if" below
				if(1)
				{
					if(ch == '\r')
					{
						uart0_send_byte('\r');
						uart0_send_byte('\n');
						
						had_got_msg('\r', shell_do);
						had_got_msg('\n', shell_do);
						ch = 0;
					}
					else
					{
						uart0_send_byte(ch);
					}
				}

				had_got_msg(ch, shell_do);
			}
		}
	}
}

/**
 * judging had a whole message beed received
 * @param byte : the byte that uart0 got in rx interruption
 * @param callback : the call back function, it will be called when a whole message received
 * @retval : TRUE --- got a message
 *           FALSE --- not got a message yet
 */
bool had_got_msg(u8 byte, void (*callback)(u8 *buf, u32 len))
{
	static bool got_0xd_flag;
	static bool got_0xa_flag;
	
	if(byte == 0)
	{
		goto error;
	}

	if(uart0_recv_index < UART0_RECV_BUF_SIZE)
	{
		uart0_recv_buf[uart0_recv_index] = byte;
		uart0_recv_index++;
	}
	else
	{
		goto error;
	}

	if(byte == 0xd)
	{
		got_0xd_flag = TRUE;
		got_0xa_flag = FALSE;
	}
	else if(byte == 0xa)
	{
		if(got_0xd_flag == TRUE && got_0xa_flag == FALSE)
		{
			got_0xd_flag = FALSE;
			got_0xa_flag = FALSE;
			if(uart0_recv_index > 2 && callback != NULL)
			{
				callback(uart0_recv_buf, uart0_recv_index);
			}
			goto after_callback;
		}
		else
		{
			goto error;
		}
	}
	else
	{
		got_0xd_flag = FALSE;
		got_0xa_flag = FALSE;
	}

	return FALSE;

after_callback:
	memset(uart0_recv_buf, 0, UART0_RECV_BUF_SIZE);
	uart0_recv_index = FALSE;
	got_0xd_flag = FALSE;
	got_0xa_flag = FALSE;
	return TRUE;

error:
	memset(uart0_recv_buf, 0, UART0_RECV_BUF_SIZE);
	uart0_recv_index = FALSE;
	got_0xd_flag = FALSE;
	got_0xa_flag = FALSE;
	return FALSE;
}

