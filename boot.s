// boot.S - Raspberry Pi 5 AArch64 boot code
// Assumes: loaded at 0x80000, started in EL2
// Goal: set up stack, drop to EL1, call kernel_main

    .section .text.boot
    .align  7
    .global _start
    .global _vectors

// ---------------------------------------------------------------------
// Exception vector table (minimal stubs for now)
// ---------------------------------------------------------------------
_vectors:
    // EL1t
    b .
    b .
    b .
    b .

    // EL1h
    b .
    b .
    b .
    b .

    // EL0_64
    b .
    b .
    b .
    b .

    // EL0_32
    b .
    b .
    b .
    b .

// ---------------------------------------------------------------------
// Entry point from firmware
// ---------------------------------------------------------------------
_start:
    // We assume we are in EL2 on entry.
    // First, set up a temporary stack in some safe region of RAM.
    // You will later define _stack_top in your linker script.
    ldr     x0, =_stack_top
    mov     sp, x0

    // Set VBAR_EL2 to our vector table (for now, just to be safe)
    ldr     x0, =_vectors
    msr     VBAR_EL2, x0
    isb

    // -----------------------------------------------------------------
    // Drop from EL2 -> EL1
    // -----------------------------------------------------------------
    // HCR_EL2: set EL1 to AArch64 (RW = 1)
    mov     x0, #0
    orr     x0, x0, #(1 << 31)        // RW = 1 (EL1 is AArch64)
    msr     HCR_EL2, x0
    isb

    // SPSR_EL2: set up PSTATE for EL1h
    // M[3:0] = 0101 (EL1h)
    // Disable FIQ, IRQ, SError, Debug (DAIF = 1111)
    mov     x0, #(0b0101)             // EL1h
    orr     x0, x0, #(0b1111 << 6)    // DAIF = 1111
    msr     SPSR_EL2, x0

    // ELR_EL2: where to return to (our EL1 entry label)
    ldr     x0, =el1_entry
    msr     ELR_EL2, x0
    isb

    // ERET to EL1
    eret

// ---------------------------------------------------------------------
// EL1 entry point
// ---------------------------------------------------------------------
el1_entry:
    // Now running at EL1h.
    // Set up EL1 vector base.
    ldr     x0, =_vectors
    msr     VBAR_EL1, x0
    isb

    // Optionally, set up a new stack for EL1 (can reuse same for now).
    ldr     x0, =_stack_top
    mov     sp, x0

    // Disable MMU for now (identity mapping, flat physical space).
    // We assume firmware left MMU off; if not, you can explicitly clear SCTLR_EL1.M.
    
    mrs     x0, SCTLR_EL1
    bic     x0, x0, #(1 << 0)         // M = 0 (MMU off)
    msr     SCTLR_EL1, x0
    isb

    // Call C entry point
    bl      kernel_main

// ---------------------------------------------------------------------
// If kernel_main returns, just loop forever.
// ---------------------------------------------------------------------
1:
    wfe
    b       1

    .size _start, . - _start
