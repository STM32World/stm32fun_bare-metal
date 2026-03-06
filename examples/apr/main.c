/**
 *
 * Signetics 25120 Simulator
 *
 * In this example we will be simulating the Signetics 25120, the legendary WOM (Write-Only Memory) chip.
 * The 25120 was a simple 128-byte memory chip that could only be written to, not read from.
 * It was often used in early computers and video game consoles for storing data that didn't need to be
 * read back, such as color palettes or sound data.
 *
 * It is blindingly fast and you can write massive amounts of data to it.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#define WOM_SIZE (1 * 1024 * 1024 * 1024) // Define the size of our simulated WOM chip

#include "main.h" // Main header contains config and common includes

#include <stdio.h>

#include "sig25120.h" // Include the header for our Signetics 25120 Library

sig25120_handler sig25120; // Create an instance of the handler for the 25120 simulator

uint8_t buffer[1024]; // 1 kB Buffer to hold data to write to the 25120

// Main function
int main(void) {

    uint16_t led = PIN('C', 13); // Blue LED

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uart_init(USART1, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock  : %9d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock  : %9d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock  : %9d Hz\n", APB2_FREQUENCY);
    printf("48 MHz clock: %9d Hz\n", CLK48);

    // Fill the buffer with some data to write to the 25120
    for (uint16_t i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = (uint8_t)(i % 256); // Just fill it with a pattern
    }

    // Initialize the 25120 simulator with a max size of 128 bytes
    sig25120_init(&sig25120, WOM_SIZE);

    bool led_state = true;
    uint32_t now = 0, next_blink = 500, next_tick = 1000, next_test = 10000, loop_cnt = 0;

    for (;;) { // Super loop

        now = s_ticks;

        if (now >= next_blink) {
            gpio_write(led, led_state); // Toggle LED
            led_state = !led_state;
            next_blink = now + 500; // Schedule next toggle in 500 ms
        }

        if (now >= next_tick) {

            printf("Tick: %7lu ( loop = %lu  )\n", now / 1000, loop_cnt);

            loop_cnt = 0;
            next_tick = now + 1000; // Schedule next tick in 1000 ms
        }

        if (now >= next_test) {

            uint32_t written = 0;
            // Write the buffer to the 25120 at address 0
            uint32_t t = s_ticks;
            for (uint32_t i = 0; i < (WOM_SIZE / 1024); ++i) { // Fill the entire WOM chip with our buffer in 1 kB chunks
                written += (uint32_t)sig25120_write(&sig25120, (int) i * 1024, buffer, sizeof(buffer));
            }
            printf("Wrote %ld bytes to the 25120 in %ld ms.\n", written, s_ticks - t);
            next_test = now + 10000; // Schedule next test in 10 seconds
        }

        ++loop_cnt; // Just a counter to show how many times the loop runs between ticks
    }

    return 0;
}

// Send printf to uart1
int _write(int fd, char *ptr, int len) {
    int i = 0;

    /*
     * Write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
     * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2) {
        return -1;
    }
    while (*ptr && (i < len)) {
        if (*ptr == '\n') {
            uart_write_char(USART1, '\r');
        }
        uart_write_char(USART1, *ptr);
        i++;
        ptr++;
    }
    return i;
}

// vim: et ts=4 nowrap autoindent
