#include <linux/kernel.h> // For printk
#include <linux/module.h> // For module macros

// Function to read the timer count
unsigned long get_timer_count() {
    unsigned long *timer_count_register = (unsigned long *)0x3F003004;
    return *timer_count_register; // Read and return the timer count
}

// Your kernel main function
void kernel_main() {
    unsigned long timer_count = get_timer_count();
    printk("Timer count: %lu\n", timer_count);
    
}
void wait_1ms() {
    unsigned long start = get_timer_count(); // Get the current timer count
    unsigned long end = start + 1000; // Calculate the target count for 1 ms

    // Busy wait until 1 ms has passed
    while (get_timer_count() < end) {
        // Do nothing, just wait
    }
}

// Call this function in your kernel main
void kernel_main() {
    unsigned long timer_count = get_timer_count();
    printk("Timer count before wait: %lu\n", timer_count);
    wait_1ms(); // Wait for 1 ms
    timer_count = get_timer_count(); // Get the timer count again
    printk("Timer count after wait: %lu\n", timer_count);
}

