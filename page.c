#include "page.h"
#include <stddef.h>  // for NULL

#define TOTAL_PAGES 128
#define PAGE_SIZE 4096 // 4KB pages

// Statically allocated array of physical pages
struct ppage physical_page_array[TOTAL_PAGES];

// Head of the free physical pages list
struct ppage *free_pages_head = NULL;

// Initialize the free physical page list
void init_pfa_list(void) {
    // Set up the free list with physical pages
    for (int i = 0; i < TOTAL_PAGES; i++) {
        physical_page_array[i].physical_addr = (void *)(i * PAGE_SIZE); // Assign physical address
        physical_page_array[i].next = (i < TOTAL_PAGES - 1) ? &physical_page_array[i + 1] : NULL;
        physical_page_array[i].prev = (i > 0) ? &physical_page_array[i - 1] : NULL;
    }
    free_pages_head = &physical_page_array[0]; // Initialize head of free list
}

// Allocate physical pages
struct ppage *allocate_physical_pages(unsigned int npages) {
    struct ppage *current = free_pages_head;
    struct ppage *allocated_head = NULL;

    for (unsigned int i = 0; i < npages; i++) {
        if (current == NULL) {
            // Not enough pages available
            return NULL; // Allocation failed
        }

        // Unlink the current page from the free list
        if (allocated_head == NULL) {
            allocated_head = current; // Mark the head of allocated pages
        }

        // Move the head pointer to the next free page
        free_pages_head = current->next;

        // Update the pointers to unlink the current page
        if (current->next) {
            current->next->prev = current->prev;
        }
        if (current->prev) {
            current->prev->next = current->next;
        }

        // Clear pointers for the allocated page
        current->next = NULL;
        current->prev = NULL;

        // Move to the next page in the free list
        current = free_pages_head;
    }

    return allocated_head; // Return head of allocated pages
}

// Free physical pages
void free_physical_pages(struct ppage *ppage_list) {
    struct ppage *current = ppage_list;

    while (current) {
        // Link back to the free list
        current->next = free_pages_head; // Point to the current head of free list
        if (free_pages_head) {
            free_pages_head->prev = current; // Update the previous pointer of the new head
        }
        free_pages_head = current; // Update the head to the newly freed page
        current->prev = NULL; // Clear previous pointer for the newly freed page

        // Move to the next page
        current = current->next; // Move to the next allocated page (already linked)
    }
}

