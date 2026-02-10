/**
 * Systick Blink bare-metal STM32 example
 */

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

struct rcc {
    volatile uint32_t CR;            // Clock control register (Internal/External/PLL enable)
    volatile uint32_t PLLCFGR;       // PLL configuration register
    volatile uint32_t CFGR;          // Clock configuration register (Bus prescalers/Source selection)
    volatile uint32_t CIR;           // Clock interrupt register
    volatile uint32_t AHB1RSTR;      // AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;      // AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;      // AHB3 peripheral reset register
    volatile uint32_t RESERVED0;     // Boundary padding
    volatile uint32_t APB1RSTR;      // APB1 peripheral reset register
    volatile uint32_t APB2RSTR;      // APB2 peripheral reset register
    volatile uint32_t RESERVED1[2];  // Boundary padding
    volatile uint32_t AHB1ENR;       // AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;       // AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;       // AHB3 peripheral clock enable register
    volatile uint32_t RESERVED2;     // Boundary padding
    volatile uint32_t APB1ENR;       // APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;       // APB2 peripheral clock enable register
    volatile uint32_t RESERVED3[2];  // Boundary padding
    volatile uint32_t AHB1LPENR;     // AHB1 peripheral clock enable in low power mode
    volatile uint32_t AHB2LPENR;     // AHB2 peripheral clock enable in low power mode
    volatile uint32_t AHB3LPENR;     // AHB3 peripheral clock enable in low power mode
    volatile uint32_t RESERVED4;     // Boundary padding
    volatile uint32_t APB1LPENR;     // APB1 peripheral clock enable in low power mode
    volatile uint32_t APB2LPENR;     // APB2 peripheral clock enable in low power mode
    volatile uint32_t RESERVED5[2];  // Boundary padding
    volatile uint32_t BDCR;          // Backup domain control register (RTC settings)
    volatile uint32_t CSR;           // Control/status register (Reset flags and LSI control)
    volatile uint32_t RESERVED6[2];  // Boundary padding
    volatile uint32_t SSCGR;         // Spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR;    // PLLI2S configuration register
};

#define RCC ((struct rcc*)0x40023800)

struct systick {
    volatile uint32_t CSR;    // Control and Status Register (Enable/Interrupt/Source/Flag)
    volatile uint32_t RVR;    // Reload Value Register (The start value for the countdown)
    volatile uint32_t CVR;    // Current Value Register (Read to see current time/Write to clear)
    volatile uint32_t CALIB;  // Calibration Value Register
};

#define SYSTICK ((struct systick*)0xe000e010)

struct gpio {
    volatile uint32_t MODER;    // Port mode register (Input/Output/AF/Analog)
    volatile uint32_t OTYPER;   // Port output type register (Push-pull/Open-drain)
    volatile uint32_t OSPEEDR;  // Port output speed register
    volatile uint32_t PUPDR;    // Port pull-up/pull-down register
    volatile uint32_t IDR;      // Port input data register
    volatile uint32_t ODR;      // Port output data register
    volatile uint32_t BSRR;     // Port bit set/reset register (Atomic pin control)
    volatile uint32_t LCKR;     // Port configuration lock register
    volatile uint32_t AFR[2];   // Alternate function registers (Low/High)
};

#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank)))

// Enum values are per datasheet: 0, 1, 2, 3
enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
};

static inline void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;                 // Set reload register
    SYSTICK->CVR = 0;                         // Clear current value register
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);  // Enable SysTick, use processor clock
    // We dont need to enable anything in RCC since its a core peripheral and is clocked by the core
    // RCC->APB2ENR |= BIT(14);                  // Enable SYSTICK clock
}

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio* gpio = GPIO(PINBANK(pin));  // GPIO bank
    int n = PINNO(pin);                      // Pin number
    gpio->MODER &= ~(3U << (n * 2));         // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2);    // Set new mode
}

static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio* gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

static volatile uint32_t s_ticks = 0;

int main(void) {
    uint16_t led = PIN('C', 13);  // Blue LED

    systick_init(16000000 / 1000);  // 1ms SysTick (assuming 16MHz clock)

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
    ++s_ticks; // Will increase every 1 ms
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) { // Naked might cause a warning but is allowed in gcc
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long* dst = &_sbss; dst < &_ebss; dst++) *dst = 0;

    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();

    for (;;) (void)0;  // Infinite loop - should never be reached
}

extern void _estack(void);  // Defined in f407.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack,
    _reset,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    systick_handler,
};
