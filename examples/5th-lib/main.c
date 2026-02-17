/**
 * Systick Blink 16 MHz HSE bare-metal STM32 example
 */

#include "main.h" 

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
