/**
 *
 * Pseudo-PWM example using TIM2 and SysTick for timing, with UART for debugging output. 
 * 
 * The blue LED on PC13 will be toggled based on a duty cycle that increments every 10 ms, 
 * creating a fading effect. The program also prints the current tick count and duty cycle every second.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "main.h" // Main header contains config and common includes

#include <stdio.h>

const uint16_t led = PIN('C', 13); // Blue LED

volatile uint8_t pwm_cnt = 0;
volatile uint8_t duty_cycle = 0;

/**
 * Main function - entry point of the program, system initialization and main loop
 */
int main(void) {

    sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz

    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uart_init(USART1, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock

    // Deal with timer and timer interrupt
    timer_setup_interrupt(TIMER2, 25000, 28); // Setup TIM2 to generate an interrupt every 0.01 second (10 ms) - IRQ number 28 for TIM2
    timer_enable(TIMER2);                     // Start the timer

    printf("\n\n\nSystem initialized.\n");
    printf("Core clock  : %9d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock  : %9d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock  : %9d Hz\n", APB2_FREQUENCY);
    printf("48 MHz clock: %9d Hz\n", CLK48);

    uint32_t now = 0, next_update = 10, next_tick = 1000, loop_cnt = 0;

    while (1) { // Super loop

        now = s_ticks;

        if (now >= next_update) {
            ++duty_cycle;
            next_update = now + 10; // Schedule next toggle in 500 ms
        }

        if (now >= next_tick) {

            printf("Tick: %7lu ( loop = %lu duty = %d )\n", now / 1000, loop_cnt, duty_cycle);

            loop_cnt = 0;
            next_tick = now + 1000; // Schedule next tick in 1000 ms
        }

        ++loop_cnt; // Just a counter to show how many times the loop runs between ticks
    }

    return 0;
}

/**
 * Timer 2 interrupt handler
 */
void tim2_irq_handler(void) {
    gpio_write(led, pwm_cnt >= duty_cycle); // Set LED state based on duty cycle
    TIMER2->SR_b.UIF = 0;                   // Clear flag
    ++pwm_cnt;                              // Increment timer counter
}

/**
 * Write function for printf
 */
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
