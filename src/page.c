#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "page.h"

#define PAGE_SIZE (2 * 1024 * 1024) // 2MB per page
#define NUM_PAGES 128

struct ppage physical_page_array[NUM_PAGES];
struct ppage *free_list_head = NULL;

void init_pfa_list(void) {
    for (int i = 0; i < NUM_PAGES; i++) {
        physical_page_array[i].physical_addr = (void *)(i * PAGE_SIZE);
        physical_page_array[i].next = free_list_head;
        physical_page_array[i].prev = NULL;

        if (free_list_head != NULL) {
            free_list_head->prev = &physical_page_array[i];
        }
        free_list_head = &physical_page_array[i];
    }
}

struct ppage *allocate_physical_pages(unsigned int npages) {
    struct ppage *allocated_list_head = NULL;
    struct ppage *current = free_list_head;
    
    for (unsigned int i = 0; i < npages; i++) {
        if (current == NULL) {
            // Not enough pages available
            return NULL;
        }

        // Unlink from free list
        if (allocated_list_head == NULL) {
            allocated_list_head = current; // Initialize allocated list head
        } else {
            allocated_list_head->prev = current; // Link in allocated list
        }
        
        free_list_head = current->next; // Move head of free list
        current->next = NULL; // Terminate allocated list
        
        if (free_list_head != NULL) {
            free_list_head->prev = NULL; // Update previous pointer
        }
        
        current = free_list_head; // Move to next page
    }
    
    return allocated_list_head; // Return the head of allocated pages
}

void free_physical_pages(struct ppage *ppage_list) {
    struct ppage *current = ppage_list;
    
    while (current != NULL) {
        // Add back to the free list
        current->next = free_list_head;
        current->prev = NULL;
        
        if (free_list_head != NULL) {
            free_list_head->prev = current;
        }
        
        free_list_head = current; // Update head of free list
        current = current->prev; // Move to next allocated page
    }
}

