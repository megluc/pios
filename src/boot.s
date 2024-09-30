.section ".kernel-header"

// Kernel Header
b _start              // Jump to start
.word 0               // Executable code
.quad _start          // text_offset: Image load offset, little endian
.quad 0x0             // image_size: Image load offset, little endian
.word 0x2             // flags
.word 0x0             // reserved
.word 0x0             // reserved

.section ".text.boot"

// Make _start global.
.globl _start

// Function to read CurrentEL
.global read_current_el
read_current_el:
    mrs x0, CurrentEL       // Read CurrentEL register into x0
    ret                      // Return with value in x0

// Entry point for the kernel.
_start:
    mrs x1, mpidr_el1       // Read MPIDR EL1
    and x1, x1, #3
    cbz x1, notmaincore      // CPU id > 0: stop
    wfi                      // Wait for interrupt
    b notmaincore

maincore:
    // Set stack pointer
    ldr x5, =_start
    mov sp, x5

    // Clear BSS
    ldr x5, =__bss_start
    ldr w6, =__bss_size
clear_bss:
    cbz w6, bss_cleared
    str xzr, [x5], #8       // Store zero to BSS area
    sub w6, w6, #8
    b clear_bss

bss_cleared:
    // Transition to EL1
    msr sp_el1, x5
    msr sp_el0, x5

    // Enable CNTP for EL1
    mrs x0, cnthctl_el2
    orr x0, x0, #3
    msr cnthctl_el2, x0
    msr cntvoff_el2, xzr

    // Enable AArch64 in EL1
    mov x0, #(1 << 31)      // AArch64
    orr x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
    msr hcr_el2, x0

    // Change execution level to EL1
    mov x2, #0x3c4          // Set up for EL1
    msr spsr_el2, x2        // Set SPSR for EL2
    adr x2, el1_return       // Address of the EL1 return label
    msr elr_el2, x2         // Set ELR for EL2
    eret                     // Return from exception

el1_return:
    bl read_current_el       // Call to read CurrentEL

    // Now transition to EL0
    mov x2, #0x3c0          // Set up for EL0
    msr spsr_el2, x2        // Set SPSR for EL2
    adr x2, el0_return       // Address of the EL0 return label
    msr elr_el2, x2         // Set ELR for EL2
    eret                     // Return to EL0

el0_return:
    mov sp, x5              // Restore stack pointer for EL0
    // Call a driver function (e.g., LED init)
    bl init_led              // Make sure to define init_led function

