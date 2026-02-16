/**
 * Systick Blink w. 16 MHz HSE bare-metal STM32 example
 */

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

enum {
    APB1_PRE = 4, /* AHB clock / 4 */
    APB2_PRE = 2, /* AHB clock / 2 */
};

enum {  // Run at 168 Mhz
    PLL_HSE = 16,
    PLL_M = 8,
    PLL_N = 168,
    PLL_P = 2,
};

#define FLASH_LATENCY 5
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3)))

struct rcc {
    uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED0,
        APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR, RESERVED2,
        APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR, AHB3LPENR,
        RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR, RESERVED6[2],
        SSCGR, PLLI2SCFGR;
};

#define RCC ((volatile struct rcc*)0x40023800)

struct flash {
    volatile uint32_t ACR;      // Access control register
    volatile uint32_t KEYR;     // Key register
    volatile uint32_t OPTKEYR;  // Option key register
    volatile uint32_t SR;       // Status register
    volatile uint32_t CR;       // Control register
    volatile uint32_t OPTCR;    // Option control register
};

#define FLASH ((struct flash*)0x40023C00)

// --- SCB (System Control Block) Register Map ---
struct scb {
    volatile uint32_t CPUID;    // CPU ID base register
    volatile uint32_t ICSR;     // Interrupt control and state register
    volatile uint32_t VTOR;     // Vector table offset register
    volatile uint32_t AIRCR;    // Application interrupt and reset control register
    volatile uint32_t SCR;      // System control register
    volatile uint32_t CCR;      // Configuration and control register
    volatile uint8_t SHP[12];   // System handler priority registers
    volatile uint32_t SHCSR;    // System handler control and state register
    volatile uint32_t CFSR;     // Configurable fault status register
    volatile uint32_t HFSR;     // Hard fault status register
    volatile uint32_t DFSR;     // Debug fault status register
    volatile uint32_t MMFAR;    // MemManage fault address register
    volatile uint32_t BFAR;     // Bus fault address register
    volatile uint32_t AFSR;     // Auxiliary fault status register
    volatile uint32_t RES[12];  // Padding to 0xD8
    volatile uint32_t CPACR;    // Coprocessor access control register (For FPU)
};

#define SCB ((struct scb*)0xE000ED00)

struct systick {
    volatile uint32_t CSR, RVR, CVR, CALIB;
};

#define SYSTICK ((struct systick*)0xe000e010)

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};

#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank)))

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT,
       GPIO_MODE_OUTPUT,
       GPIO_MODE_AF,
       GPIO_MODE_ANALOG };

void system_clock_init(void) {
    // 1. Enable FPU and Flash Latency (Keep original guide code here)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);  // Flash latency, prefetch

    // 2. Enable HSE
    RCC->CR |= (1 << 16);            // Set HSEON
    while (!(RCC->CR & (1 << 17)));  // Wait for HSERDY

    // 3. Configure PLL
    // Clear and set M, N, P, and importantly: Set Bit 22 to select HSE as source
    RCC->PLLCFGR =
        (PLL_M << 0) | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | (1 << 22);

    // 4. Enable PLL
    RCC->CR |= (1 << 24);            // Set PLLON
    while (!(RCC->CR & (1 << 25)));  // Wait for PLLRDY

    // 5. Select PLL as System Clock
    RCC->CFGR &= ~(uint32_t)3;                   // Clear SW bits
    RCC->CFGR |= 2;                              // Select PLL (0b10)
    while ((RCC->CFGR & (3 << 2)) != (2 << 2));  // Wait for SWS to indicate PLL
}

static inline void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;  // Set reload register
    SYSTICK->CVR = 0;          // Clear current value register
    SYSTICK->CSR =
        BIT(0) | BIT(1) | BIT(2);  // Enable SysTick, use processor clock
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

    system_clock_init();

    systick_init(SYS_FREQUENCY / 1000);  // 1ms SysTick (assuming 16MHz clock)

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

void systick_handler(void) {
    ++s_ticks;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long* dst = &_sbss; dst < &_ebss; dst++)
        *dst = 0;

    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;)
        *dst++ = *src++;

    main();

    for (;;)
        (void)0;  // Infinite loop - should never be reached
}

extern void _estack(void);  // Defined in link.ld

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

// vim: set ts=4 sw=4 et:
