/**
 * File:s3c2440_start.S
 * Author:Sam Hotiky
 * Description:boot routine for JZ2440 board(CPU:S3C2440A,400MHz,ARM920T;SDRAM:128MB;NANDFLASH:256MB;NORFLASH:2MB;etc
 */

/**
 * watch dog timer register
 */
#define  WTCON          0x53000000

/**
 * bank6 registers for SDRAM
 */
#define  SDRAMCON       0x48000000
#define  BANKCON6       0x4800001C
#define  RFSHCON        0x48000024
#define  BKSZCON        0x48000028
#define  MRSR6CON       0x4800002C

/**
 * clock registers
 */
#define  CLKDIV         0x4C000014
#define  MPLLCON        0x4C000004

.section .boot

.extern main
.text
.global _start
_start:
	b   Reset
	b   HandleUndef
	b   HandleSWI
	b   HandlePrefetchAbort
	b   HandleDataAbort
	b   HandleNotUsed
	b   HandleIRQ
	b   HandleFIQ

Reset:
	/**
	 * disable the watch dog timer
	 */
	ldr		r0, =0x53000000
	mov		r1, #0
	str		r1, [r0]

	/**
	 * make the clock run in 400MHz
	 */
	ldr		r0, =CLKDIV	
	mov		r1, #5
	str		r1, [r0]
	/***************************/
	mrc		p15, 0, r1, c1, c0, 0
	orr		r1, r1, #0xC0000000
	mcr		p15, 0, r1, c1, c0, 0
	/***************************/
	ldr		r0, =MPLLCON	
	ldr		r1, =0x5c012
	str		r1, [r0]

	/**
	 * enable the I,D cache
	 */
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #0x0004
	orr		r0, r0, #0x1000
	mcr		p15, 0, r0, c1, c0, 0

	/**
	 * setup the SDRAM
	 */
	ldr		r0, =SDRAMCON
	ldr		r1, =0x22011110
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000700
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00018005
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00018005
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x008C07A3
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x000000B1
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000030
	str		r1, [r0]
	add		r0, r0, #4
	ldr		r1, =0x00000030
	str		r1, [r0]
	add		r0, r0, #4

	/**
	 * copy nandflash's app code to sdram
	 */
	ldr		sp, =0x34000000
	bl		nand_boot_init
	ldr		r0, =4096
	ldr		r1, =0x30100000
	ldr		r2, =30
	bl		nand_boot_copy2sdram

	/**
	 * clear bss
	 */
	mov		r1, #0
	ldr		r0, =__bss_start
	ldr		r2, =__bss_end
	cmp		r0, r2
	bne		copy_loop
copy_loop:
	str		r1, [r0]
	add		r0, r0, #4
	cmp		r0, r2
	blt		copy_loop
	
	/**
	 * enable the ARM irq, CPU at super visor mode
	 */
	msr		cpsr_c, #0x53
	ldr		sp, =0x34000000

	/**
	 * jump to main()
	 */
	ldr		lr, =halt_loop
	ldr		pc, =main
halt_loop:
	b		halt_loop

HandleUndef:
    b		.

HandleSWI:
    b		.

HandlePrefetchAbort:
    b		.

HandleDataAbort:
    b		.

HandleNotUsed:
    b		.

HandleIRQ:
	b		.

HandleFIQ:
    b		.
