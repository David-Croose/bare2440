.section .text

.global get_cpsr
.global set_cpsr
.global enable_arm_irq
.global disable_arm_irq

get_cpsr:
	mrs		r0, cpsr
	bx		lr

set_cpsr:
	msr		cpsr, r0
	bx		lr

enable_arm_irq:
	mrs		r0, cpsr
	orr		r0, r0, #0x80
	msr		cpsr, r0
	bx		lr

disable_arm_irq:
	mrs		r0, cpsr
	bic		r0, r0, #0x80
	msr		cpsr, r0
	bx		lr

.global prepare_led
prepare_led:
	ldr		r8, =0x56000054
	ldr		r9, [r8]
	and		r9, r9, #0xFFFFFFEF
	bx		lr

.global led4_on
led4_on:
	str		r9, [r8]
	bx		lr

