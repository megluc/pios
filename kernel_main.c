#include "page.h"
#include <stdio.h> // Include necessary headers for printing

// Function prototypes
void init_pfa_list(void);
struct ppage *allocate_physical_pages(unsigned int npages);
extern void mmu_on(void); // Declare the external assembly function

void kernel_main(void) {
    // Other initializations...

    mmu_on(); // Call the function to enable MMU

    // Initialize the physical frame allocator
    init_pfa_list();

    // Attempt to allocate 5 pages
    struct ppage *allocated_pages = allocate_physical_pages(5);

    // Check if allocation was successful
    if (allocated_pages != NULL) {
        // Print out the addresses of the allocated pages
        for (int i = 0; i < 5; i++) {
            if (allocated_pages[i].physical_addr != NULL) {
                // Example print statement (adjust based on your environment)
                printf("Allocated page %d at address: %p\n", i, allocated_pages[i].physical_addr);
            }
        }
    } else {
        printf("Allocation failed: Not enough free pages available.\n");
    }

    // Optionally, free the allocated pages here or continue testing
    free_physical_pages(allocated_pages); // Free the allocated pages if you implement this
}

