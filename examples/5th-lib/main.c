/**
 * Systick Blink 16 MHz HSE bare-metal STM32 example
 */

#include "main.h" 

void system_clock_init(void) {

    // Enable FPU and Flash Latency (Keep original guide code here)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);  // Flash latency, prefetch

    // Enable HSE
    RCC->CR |= BIT(16);            // Set HSEON
    while (!(RCC->CR & BIT(17)));  // Wait for HSERDY

    // Configure PLL
    // Clear and set M, N, P, and importantly: Set Bit 22 to select HSE as source
    RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | BIT(22);

    // Enable PLL
    RCC->CR |= BIT(24);            // Set PLLON
    while (!(RCC->CR & BIT(25)));  // Wait for PLLRDY

    // Select PLL as System Clock
    RCC->CFGR &= ~(uint32_t)3;                   // Clear SW bits
    RCC->CFGR |= 2;                              // Select PLL (0b10)
    while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // Wait for SWS to indicate PLL
    
}

// Main function
int main(void) {

    uint16_t led = PIN('C', 13);  // Blue LED

    system_clock_init();

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

// Systick interrupt handler
void systick_handler(void) {
    ++s_ticks;  // Will increase every 1 ms
}

// vim: set ts=4 sw=4 expandtab:
