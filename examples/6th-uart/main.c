/**
 *
 * UART example
 *
 * In this example we will be building on the previous example by adding
 * UART support for debugging. We will initialize USART1 to run at 2 Mbps,
 * which is a good speed for a 168 MHz core clock.
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

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uart_init(USART1, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock: %d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock: %d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock: %d Hz\n", APB2_FREQUENCY);

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
            printf("Tick: %lu (loop = %lu)\n", now / 1000, loop_cnt);
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
