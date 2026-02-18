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

// Main function
int main(void) {

    uint16_t led = PIN('C', 13);  // Blue LED

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz

    systick_init(SYS_FREQUENCY / 1000);  // 1ms SysTick 

    RCC->AHB1ENR |= BIT(PINBANK(led));     // Enable GPIO clock for LED
    gpio_set_mode(led, GPIO_MODE_OUTPUT);  // Set blue LED to output mode

    bool led_state = true;
    uint32_t now = 0, next_blink = 500;

    while (true) {
        now = s_ticks;

        if (now >= next_blink) {
            gpio_write(led, led_state);  // Toggle LED every 500ms
            led_state = !led_state;
            next_blink = now + 500;
        }
    }

    return 0;
}

// vim: set ts=4 sw=4 expandtab:
