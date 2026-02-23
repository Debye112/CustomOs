// boot.S
.section .text.boot, "ax"
.align 11
.global _start
.extern kmain

_start:
    // Assume EL1 (Pi 5 firmware usually drops us there)
    // If you want EL2→EL1 later, we can add it back.

    // Set up stack
    ldr x0, =_stack_top
    mov sp, x0

    // Jump into C
    bl kmain

1:  wfe
    b 1b
