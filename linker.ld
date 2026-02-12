/* linker.ld — Raspberry Pi 5 bare-metal kernel */
ENTRY(_start)

SECTIONS
{
    /* Firmware loads kernel8.img at 0x80000 */
    . = 0x80000;

    /* Code (text) */
    .text : ALIGN(4096) {
        *(.text.boot)
        *(.text*)
    }

    /* Read-only data */
    .rodata : ALIGN(4096) {
        *(.rodata*)
    }

    /* Initialized data */
    .data : ALIGN(4096) {
        *(.data*)
    }

    /* Uninitialized data (zeroed at runtime) */
    .bss : ALIGN(4096) {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    /* Define stack at top of 16MB RAM region */
    . = 0x2000000;   /* 32 MB mark */
    _stack_top = .;
}
