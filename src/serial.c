#include <stdint.h> // For standard integer types
#include "rprintf.h" // Include the rprintf header

#define MU_IO_REG 0x3F215040 // For Raspberry Pi 3
// #define MU_IO_REG 0xFE215004 // For Raspberry Pi 4

// Function to write a character to the serial port
void putc(int data) {
    volatile char *mu_io = (char *)MU_IO_REG;
    *mu_io = (char)data; // Writes  the character to the MU IO register
}

// Function to print the current execution level
void print_execution_level() {
    int current_execution_level = getEL(); // Get current execution level
    esp_printf(putc, "Current Execution Level is %d\r\n", current_execution_level);
}

// Kernel initialization function
void kernel_init() {
    print_execution_level(); // Call the function during kernel initialization
}

