	.syntax unified
	.cpu cortex-m0
	.align 2
	.global fibonacci
	.thumb
	.thumb_func
fibonacci:
	push {lr}
	sub sp, sp, #4
	str r0, [sp]
	cmp r0, #1
	bne base_case0
base_case1:
	movs r0, #1
	add sp, sp, #4
	pop {pc}
base_case0:
	cmp r0, #0
	bne recurse
	movs r0, #0
	add sp, sp, #4
	pop {pc}
recurse:
	subs r0, r0, #1
	bl fibonacci
	sub sp, sp, #4
	str r0, [sp]
	ldr r0, [sp, #4]
	subs r0, r0, #2
	bl fibonacci
	ldr r1, [sp]
	add r0, r0, r1
    add sp, sp, #8
    pop {pc}



