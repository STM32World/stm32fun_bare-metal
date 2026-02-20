/**
 * Systick Blink 16 MHz HSE bare-metal STM32 example
 */

#include <stdbool.h>
#include <stdint.h>

// Bit manipulation macros
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


// RCC peripheral structure
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

#define RCC ((struct rcc*)0x40023800) // RCC base address

struct flash {
    volatile uint32_t ACR;      // Access control register
    volatile uint32_t KEYR;     // Key register
    volatile uint32_t OPTKEYR;  // Option key register
    volatile uint32_t SR;       // Status register
    volatile uint32_t CR;       // Control register
    volatile uint32_t OPTCR;    // Option control register
};

#define FLASH ((struct flash*)0x40023C00) // Flash base address

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

// SysTick peripheral structure
struct systick {
    volatile uint32_t CSR;    // Control and Status Register (Enable/Interrupt/Source/Flag)
    volatile uint32_t RVR;    // Reload Value Register (The start value for the countdown)
    volatile uint32_t CVR;    // Current Value Register (Read to see current time/Write to clear)
    volatile uint32_t CALIB;  // Calibration Value Register
};

#define SYSTICK ((struct systick*)0xe000e010) // SysTick base address

// GPIO peripheral structure
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

#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank))) // GPIO base address

// Enum values are per datasheet: 0, 1, 2, 3
enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
};

void system_clock_init(void) {

    // FPU shares the same clock as the core the following code just enables full access to it 
    // 0b11(3) gives full access to the FPU 
    // RM B3-613 B3-614
    //SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    //Enable Flash Latency (Keep original guide code here)
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

// Initialize SysTick timer to generate interrupts every 'ticks' clock cycles
static inline void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;                 // Set reload register
    SYSTICK->CVR = 0;                         // Clear current value register
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);  // Enable SysTick, use processor clock
    // We dont need to enable anything in RCC since its a core peripheral and is clocked by the core
    // RCC->APB2ENR |= BIT(14);                  // Enable SYSTICK clock
}

// Set GPIO pin mode
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio* gpio = GPIO(PINBANK(pin));  // GPIO bank
    int n = PINNO(pin);                      // Pin number
    gpio->MODER &= ~(3U << (n * 2));         // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2);    // Set new mode
}

// Write value to GPIO pin
static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio* gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

// Global tick counter (in milliseconds)
static volatile uint32_t s_ticks = 0;

// Main function
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

// Systick interrupt handler
void systick_handler(void) {
    ++s_ticks;  // Will increase every 1 ms
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {  // Naked might cause a warning but is allowed in gcc
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
