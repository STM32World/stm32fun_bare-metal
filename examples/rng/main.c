/**
 *
 * Systick Blink 16 MHz HSE bare-metal STM32 example
 *
 * This example demonstrates how to use the SysTick timer to create a simple LED blinking application
 * on an STM32 microcontroller running at 168 MHz with a 16 MHz external crystal (HSE).
 * The blue LED connected to pin C13 will toggle every 500 milliseconds.
 * The code initializes the system clock, configures the SysTick timer for 1 ms ticks,
 * and sets up the GPIO pin for the LED. The main loop checks the elapsed time and toggles
 * the LED state accordingly.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "main.h" // Main header contains config and common includes

#include <stdio.h>

// Main function
int main(void) {

    uint16_t led = PIN('C', 13); // Blue LED

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz and 48 MHz clock for USB, RNG, etc.

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uart_init(USART1, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock  : %9d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock  : %9d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock  : %9d Hz\n", APB2_FREQUENCY);
    printf("48 MHz clock: %9d Hz\n", CLK48);

    printf("Enabling RNG...\n");
    rng_init(); // Initialize the random number generator

    printf("Starting main loop...\n");

    bool led_state = true;
    uint32_t now = 0, next_blink = 500, next_tick = 1000, loop_cnt = 0;

    for (;;) { // Super loop

        now = s_ticks;

        if (now >= next_blink) {
            gpio_write(led, led_state); // Toggle LED
            led_state = !led_state;
            next_blink = now + 500; // Schedule next toggle in 500 ms
        }

        if (now >= next_tick) {

            uint32_t random_value = rng_get_random(); // Get a random number from the RNG

            printf("Tick: %5lu (loop = %lu rnd = 0x%08lx)\n", now / 1000, loop_cnt, random_value);
            loop_cnt = 0;
            next_tick = now + 1000; // Schedule next tick in 1000 ms
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

// vim: set ts=4 sw=4 expandtab:
