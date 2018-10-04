.section .new_vector

irq_new_vector:
	ldr		pc, =HandleReset2
	ldr		pc, =HandleUndef2
	ldr		pc, =HandleSWI2
	ldr		pc, =HandlePrefetchAbort2
	ldr		pc, =HandleDataAbort2
	ldr		pc, =HandleNotUsed2
	ldr		pc, =HandleIRQ2
	ldr		pc, =HandleFIQ2

HandleReset2:
	ldr			sp, =0x33F00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =reset_irq_handler

HandleUndef2:
	ldr			sp, =0x33E00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =undef_irq_handler

HandleSWI2:
	ldr			sp, =0x33D00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =swi_irq_handler

HandlePrefetchAbort2:
	ldr			sp, =0x33C00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =prefetch_abort_handler

HandleDataAbort2:
	ldr			sp, =0x33B00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =data_abort_handler

HandleNotUsed2:
	ldr			sp, =0x33A00000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =not_used_handler

HandleIRQ2:
 	ldr			sp, =0x33900000
 	sub 		lr, lr, #4
 	stmdb		sp!, {r0-r12, lr}
 	ldr			lr, =return
 	ldr			pc, =irq_handler

HandleFIQ2:
	ldr			sp, =0x33800000
	sub 		lr, lr, #4
	stmdb		sp!, {r0-r12, lr}
	ldr			lr, =return
	ldr			pc, =fiq_handler

return:
	ldmia		sp!, {r0-r12, pc}^

