#ifndef _UART0_H_
#define _UART0_H_

#include "s3c2440.h"

#define UART0_RECV_BUF_SIZE    100

void uart0_tx_irq(bool enable_flag);
void uart0_rx_irq(bool enable_flag);
void uart0_set_fifo(s32 tx_deep, s32 rx_deep);

void uart0_init(u32 baudrate);
void uart0_send_byte(u8 sendbyte);
void uart0_send_break(void);
u8 uart0_receive_byte(void);
void uart0_irq_handler(vu32 intpnd, vu32 subsrcpnd);

#endif
