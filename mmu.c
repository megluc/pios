#include <stdint.h> // For uintptr_t

// Define the L1 and L2 descriptor structures
struct table_descriptor_stage1 {
    unsigned int type : 2; // Least significant bits
    unsigned int ignored : 10;
    unsigned long next_lvl_table : 36;
    unsigned int res0 : 11;
    unsigned int pxn_table : 1;
    unsigned int xn_table : 1;
    unsigned int ap_table : 2;
    unsigned int ns_table : 1; // Most significant bit
} __attribute__((packed));

struct page_descriptor_stage1 {
    unsigned int type : 2; // block (2’b01) or table (2’b11)
    unsigned int attrindx : 3; // stage 1 memory attr index field
    unsigned int ns : 1; // non-secure
    unsigned int ap : 2; // data access permissions
    unsigned int sh : 2; // sharability field
    unsigned int af : 1; // accessed flag
    unsigned int ng : 1; // not global
    unsigned int oa : 4; // ?
    unsigned int nt : 1; // block translation entry
    unsigned int output_addr : 18; // output address
    unsigned int res01 : 13; // another res0 field
    unsigned int res00 : 2; // reserved, 0
    unsigned int gp : 1; // guarded page
    unsigned int dbm : 1; // dirty bit modifier
    unsigned int contiguous : 1; // translation table entry is contiguous
    unsigned int pxn : 1; // privileged execute never
    unsigned int xn : 1; // execute never
    unsigned int ignored2 : 4;
    unsigned int pbha : 4; // page-based hardware attributes
    unsigned int ignored1 : 1;
} __attribute__((packed));

void mmu_on(void) {
    // Step 1: Allocate and set up page tables
    struct table_descriptor_stage1 *l1_table = (struct table_descriptor_stage1 *)0x00000000; // L1 table base
    struct page_descriptor_stage1 *l2_table = (struct page_descriptor_stage1 *)0x00002000; // L2 table base

    // Step 2: Initialize L1 descriptor
    l1_table[0].type = 3; // Table descriptor
    l1_table[0].next_lvl_table = (uintptr_t)l2_table >> 10; // Point to L2 table
    l1_table[0].ap_table = 0; // Read/write access
    l1_table[0].ns_table = 0; // Secure access

    // Step 3: Initialize L2 descriptor for identity mapping
    l2_table[0].type = 1; // Block descriptor
    l2_table[0].output_addr = 0; // Physical address (0x00000000)
    l2_table[0].ap = 0; // Read/write access
    l2_table[0].ns = 0; // Secure access
    // Configure other fields as necessary (e.g., attributes, flags)
    
    // Step 4: Set up the translation table base address
    uintptr_t ttbr = (uintptr_t)l1_table; // Use the address of the L1 table

    // Step 5: Write the TTBR0 register
    asm volatile(
        "msr ttbr0_el1, %0\n"  // Set the translation table base register
        : // No output operands
        : "r" (ttbr)           // Input operand
    );

    // Ensure memory operations are completed before enabling MMU
    asm volatile("isb");

    // Enable the MMU in the system control register
    asm volatile(
        "mrs x0, sctlr_el1\n"  // Read the system control register
        "orr x0, x0, #0x1\n"   // Set the M bit (bit 0) to enable MMU
        "msr sctlr_el1, x0\n"  // Write back to the system control register
        "isb\n"                // Ensure the changes are visible
    );
}

