/**
 * Minimal bare-metal STM32 example
 */

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

struct rcc {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR, RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR, AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR,
        CSR, RESERVED6[2], SSCGR, PLLI2SCFGR;
};

#define RCC ((struct rcc*)0x40023800)

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};

#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank)))

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

struct systick {
    volatile uint32_t CSR, RVR, CVR, CALIB;
};

#define SYSTICK ((struct systick *) 0xe000e010)

static inline void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;         // Set reload register
    SYSTICK->CVR = 0;                 // Clear current value register
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);  // Enable SysTick, use processor clock
    RCC->APB2ENR |= BIT(14);  // Enable SYSTICK clock
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

static inline void spin(volatile uint32_t count) {
    while (count--) (void)0;
}

static volatile uint32_t s_ticks = 0;

// main
int main(void) {

    systick_init(16000000 / 1000);         // 1ms SysTick (assuming 16MHz clock)

    uint16_t led = PIN('C', 13);           // Blue LED
    RCC->AHB1ENR |= BIT(PINBANK(led));     // Enable GPIO clock for LED
    gpio_set_mode(led, GPIO_MODE_OUTPUT);  // Set blue LED to output mode

    bool led_state = false;
    uint32_t now = 0, next_blink = 500;

    while(1) {
        now = s_ticks;
        if (now >= next_blink) {
            gpio_write(led, led_state);
            led_state = !led_state;
            next_blink = now + 500;
        }
    }

    return 0;
}

void SysTick_Handler(void) {
    s_ticks++;
}


// Startup code
__attribute__((noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long* dst = &_sbss; dst < &_ebss; dst++) *dst = 0;

    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    main();

    for (;;) (void)0;  // Infinite loop - should never be reached
}

extern void _estack(void);  // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};

