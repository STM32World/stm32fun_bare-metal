/**
 *
 * External Interrupt example
 *
 * In this example we will enable external interrupt on pin PC15.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "main.h" // Main header contains config and common includes

#include <stdio.h>

volatile uint8_t btn_changed = 0; // Flag to indicate button state change
volatile uint8_t btn_state = 0;   // Current state of the button (0 or 1)

// Main function
int main(void) {

    uint16_t led = PIN('C', 13); // Blue LED
    uint16_t btn = PIN('C', 15); // User button

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode
    gpio_set_mode(btn, GPIO_MODE_INPUT);  // Set user button to input mode

    uart_init(USART1, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock

    exti_init(btn, 1, 1); // Enable both rising and falling edge triggers for the button pin

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock  : %9d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock  : %9d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock  : %9d Hz\n", APB2_FREQUENCY);
    printf("48 MHz clock: %9d Hz\n", CLK48);

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

            printf("Tick: %7lu ( loop = %lu )\n", now / 1000, loop_cnt);

            loop_cnt = 0;
            next_tick = now + 1000; // Schedule next tick in 1000 ms
        }

        if (btn_changed) {
            printf("Button state changed: %d\n", btn_state);
            btn_changed = 0; // Clear the flag
        }

        ++loop_cnt; // Just a counter to show how many times the loop runs between ticks
    }

    return 0;
}

// External interrupt handler for pin PC15
void exti15_10_irq_handler(void) {
    if (exti_get_pending(PIN('C', 15))) {    // Check if the interrupt is from pin PC15
        btn_state = gpio_read(PIN('C', 15)); // Toggle button state
        btn_changed = 1;                     // Set flag to indicate state change
        exti_clear_pending(PIN('C', 15));    // Clear the interrupt pending bit
    }
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
