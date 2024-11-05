.global _start
_start:
    // Disable interuptions
    cpsid i

    ldr sp, =stack_top

    // Call to main()
    bl main

    // Infinite loop
hang:
    b hang

.section .bss
.space 0x1000
stack_top:
