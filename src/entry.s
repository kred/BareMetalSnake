.section ".text.boot"

.globl _start
.globl enable_irq

_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler

reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word fiq

reset:
    // copy handlers to 0x0000
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    // setup stack for IRQ mode
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    // setup stack for FIQ mode
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

    // setup stack for SVC mode
    mov r0,#0xD3
    msr cpsr_c,r0
    mov sp,#0x8000000
    
    // we are still in SVC mode

    bl main

hang: 
    b hang

enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

irq:
    // store registers on stack
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl _irq_handler
    // we are back, restore registers
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

fiq:
    // store registers on stack
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl _fiq_handler
    // we are back, restore registers
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4
