/**
 *
 * Vector table and startup code for STM32F4xx microcontrollers.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#include "f4x.h"

// --- Default Handler ---
__attribute__((section(".text.default_handler"))) void default_handler(void) {
    for (;;)
        (void)0;
}

#define WEAK_DEF(name) void name(void) __attribute__((weak, alias("default_handler")))

// System Exceptions
WEAK_DEF(nmi_handler);
WEAK_DEF(hard_fault_handler);
WEAK_DEF(mem_manage_handler);
WEAK_DEF(bus_fault_handler);
WEAK_DEF(usage_fault_handler);
WEAK_DEF(svc_handler);
WEAK_DEF(debug_mon_handler);
WEAK_DEF(pendsv_handler);
WEAK_DEF(systick_handler);

// Peripheral Interrupts
WEAK_DEF(wwdg_irq_handler);               // Window Watchdog
WEAK_DEF(pvd_irq_handler);                // PVD through EXTI Line detect
WEAK_DEF(tamp_stamp_irq_handler);         // Tamper and TimeStamp
WEAK_DEF(rtc_wkup_irq_handler);           // RTC Wakeup
WEAK_DEF(flash_irq_handler);              // Flash
WEAK_DEF(rcc_irq_handler);                // RCC
WEAK_DEF(exti0_irq_handler);              // EXTI Line 0
WEAK_DEF(exti1_irq_handler);              // EXTI Line 1
WEAK_DEF(exti2_irq_handler);              // EXTI Line 2
WEAK_DEF(exti3_irq_handler);              // EXTI Line 3
WEAK_DEF(exti4_irq_handler);              // EXTI Line 4
WEAK_DEF(dma1_stream0_irq_handler);       // DMA1 Stream 0
WEAK_DEF(dma1_stream1_irq_handler);       // DMA1 Stream 1
WEAK_DEF(dma1_stream2_irq_handler);       // DMA1 Stream 2
WEAK_DEF(dma1_stream3_irq_handler);       // DMA1 Stream 3
WEAK_DEF(dma1_stream4_irq_handler);       // DMA1 Stream 4
WEAK_DEF(dma1_stream5_irq_handler);       // DMA1 Stream 5
WEAK_DEF(dma1_stream6_irq_handler);       // DMA1 Stream 6
WEAK_DEF(adc_irq_handler);                // ADC1, ADC2 and ADC3
WEAK_DEF(can1_tx_irq_handler);            // CAN1 TX
WEAK_DEF(can1_rx0_irq_handler);           // CAN1 RX0
WEAK_DEF(can1_rx1_irq_handler);           // CAN1 RX1
WEAK_DEF(can1_sce_irq_handler);           // CAN1 SCE
WEAK_DEF(exti9_5_irq_handler);            // EXTI Line [9:5]
WEAK_DEF(tim1_brk_tim9_irq_handler);      // TIM1 Break and TIM9
WEAK_DEF(tim1_up_tim10_irq_handler);      // TIM1 Update and TIM10
WEAK_DEF(tim1_trg_com_tim11_irq_handler); // TIM1 Trigger/Commutation and TIM11
WEAK_DEF(tim1_cc_irq_handler);            // TIM1 Capture Compare
WEAK_DEF(tim2_irq_handler);               // TIM2
WEAK_DEF(tim3_irq_handler);               // TIM3
WEAK_DEF(tim4_irq_handler);               // TIM4
WEAK_DEF(i2c1_ev_irq_handler);            // I2C1 Event
WEAK_DEF(i2c1_er_irq_handler);            // I2C1 Error
WEAK_DEF(i2c2_ev_irq_handler);            // I2C2 Event
WEAK_DEF(i2c2_er_irq_handler);            // I2C2 Error
WEAK_DEF(spi1_irq_handler);               // SPI1
WEAK_DEF(spi2_irq_handler);               // SPI2
WEAK_DEF(usart1_irq_handler);             // USART1
WEAK_DEF(usart2_irq_handler);             // USART2
WEAK_DEF(usart3_irq_handler);             // USART3
WEAK_DEF(exti15_10_irq_handler);          // EXTI Line [15:10]
WEAK_DEF(rtc_alarm_irq_handler);          // RTC Alarm (A and B)
WEAK_DEF(otg_fs_wkup_irq_handler);        // USB OTG FS Wakeup
WEAK_DEF(tim8_brk_tim12_irq_handler);     // TIM8 Break and TIM12
WEAK_DEF(tim8_up_tim13_irq_handler);      // TIM8 Update and TIM13
WEAK_DEF(tim8_trg_com_tim14_irq_handler); // TIM8 Trigger/Commutation and TIM14
WEAK_DEF(tim8_cc_irq_handler);            // TIM8 Capture Compare
WEAK_DEF(dma1_stream7_irq_handler);       // DMA1 Stream 7
WEAK_DEF(fsmc_irq_handler);               // FSMC
WEAK_DEF(sdio_irq_handler);               // SDIO
WEAK_DEF(tim5_irq_handler);               // TIM5
WEAK_DEF(spi3_irq_handler);               // SPI3
WEAK_DEF(uart4_irq_handler);              // UART4
WEAK_DEF(uart5_irq_handler);              // UART5
WEAK_DEF(tim6_dac_irq_handler);           // TIM6 and DAC1&2 underrun
WEAK_DEF(tim7_irq_handler);               // TIM7
WEAK_DEF(dma2_stream0_irq_handler);       // DMA2 Stream 0
WEAK_DEF(dma2_stream1_irq_handler);       // DMA2 Stream 1
WEAK_DEF(dma2_stream2_irq_handler);       // DMA2 Stream 2
WEAK_DEF(dma2_stream3_irq_handler);       // DMA2 Stream 3
WEAK_DEF(dma2_stream4_irq_handler);       // DMA2 Stream 4
WEAK_DEF(eth_irq_handler);                // Ethernet
WEAK_DEF(eth_wkup_irq_handler);           // Ethernet Wakeup
WEAK_DEF(can2_tx_irq_handler);            // CAN2 TX
WEAK_DEF(can2_rx0_irq_handler);           // CAN2 RX0
WEAK_DEF(can2_rx1_irq_handler);           // CAN2 RX1
WEAK_DEF(can2_sce_irq_handler);           // CAN2 SCE
WEAK_DEF(otg_fs_irq_handler);             // USB OTG FS
WEAK_DEF(dma2_stream5_irq_handler);       // DMA2 Stream 5
WEAK_DEF(dma2_stream6_irq_handler);       // DMA2 Stream 6
WEAK_DEF(dma2_stream7_irq_handler);       // DMA2 Stream 7
WEAK_DEF(usart6_irq_handler);             // USART6
WEAK_DEF(i2c3_ev_irq_handler);            // I2C3 event
WEAK_DEF(i2c3_er_irq_handler);            // I2C3 error
WEAK_DEF(otg_hs_ep1_out_irq_handler);     // USB OTG HS End Point 1 Out
WEAK_DEF(otg_hs_ep1_in_irq_handler);      // USB OTG HS End Point 1 In
WEAK_DEF(otg_hs_wkup_irq_handler);        // USB OTG HS Wakeup
WEAK_DEF(otg_hs_irq_handler);             // USB OTG HS
WEAK_DEF(dcmi_irq_handler);               // DCMI
WEAK_DEF(cryp_irq_handler);               // CRYP crypto
WEAK_DEF(hash_rng_irq_handler);           // Hash and Rng
WEAK_DEF(fpu_irq_handler);                // FPU
WEAK_DEF(uart7_irq_handler);              // UART7
WEAK_DEF(uart8_irq_handler);              // UART8
WEAK_DEF(spi4_irq_handler);               // SPI4
WEAK_DEF(spi5_irq_handler);               // SPI5
WEAK_DEF(spi6_irq_handler);               // SPI6
WEAK_DEF(sai1_irq_handler);               // SAI1
WEAK_DEF(lcd_tft_irq_handler);            // LCD-TFT
WEAK_DEF(lcd_tft_err_irq_handler);        // LCD-TFT Error
WEAK_DEF(dma2d_irq_handler);              // DMA2D

// --- Startup code ---

_weak int main(void) {
    for (;;)
        (void)0;
}

extern void _estack(void);

__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for (long *dst = &_sbss; dst < &_ebss; dst++)
        *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;)
        *dst++ = *src++;

    main();

    for (;;)
        (void)0;
}

// --- The Vector Table ---

__attribute__((section(".vectors"))) void (*const tab[])(void) = {
    [0] = _estack,             // Initial Stack Pointer
    [1] = _reset,              // Reset Handler
    [2] = nmi_handler,         // NMI Handler
    [3] = hard_fault_handler,  // Hard Fault Handler
    [4] = mem_manage_handler,  // MPU Fault Handler
    [5] = bus_fault_handler,   // Bus Fault Handler
    [6] = usage_fault_handler, // Usage Fault Handler
    [7] = 0,                   // Reserved
    [8] = 0,                   // Reserved
    [9] = 0,                   // Reserved
    [10] = 0,                  // Reserved
    [11] = svc_handler,        // SVCall Handler
    [12] = debug_mon_handler,  // Debug Monitor Handler
    [13] = 0,                  // Reserved
    [14] = pendsv_handler,     // PendSV Handler
    [15] = systick_handler,    // SysTick Handler

    // External Interrupts
    [16] = wwdg_irq_handler,               // Window Watchdog
    [17] = pvd_irq_handler,                // PVD through EXTI Line detect
    [18] = tamp_stamp_irq_handler,         // Tamper and TimeStamp
    [19] = rtc_wkup_irq_handler,           // RTC Wakeup
    [20] = flash_irq_handler,              // Flash
    [21] = rcc_irq_handler,                // RCC
    [22] = exti0_irq_handler,              // EXTI Line 0
    [23] = exti1_irq_handler,              // EXTI Line 1
    [24] = exti2_irq_handler,              // EXTI Line 2
    [25] = exti3_irq_handler,              // EXTI Line 3
    [26] = exti4_irq_handler,              // EXTI Line 4
    [27] = dma1_stream0_irq_handler,       // DMA1 Stream 0
    [28] = dma1_stream1_irq_handler,       // DMA1 Stream 1
    [29] = dma1_stream2_irq_handler,       // DMA1 Stream 2
    [30] = dma1_stream3_irq_handler,       // DMA1 Stream 3
    [31] = dma1_stream4_irq_handler,       // DMA1 Stream 4
    [32] = dma1_stream5_irq_handler,       // DMA1 Stream 5
    [33] = dma1_stream6_irq_handler,       // DMA1 Stream 6
    [34] = adc_irq_handler,                // ADC1, ADC2 and ADC3
    [35] = can1_tx_irq_handler,            // CAN1 TX
    [36] = can1_rx0_irq_handler,           // CAN1 RX0
    [37] = can1_rx1_irq_handler,           // CAN1 RX1
    [38] = can1_sce_irq_handler,           // CAN1 SCE
    [39] = exti9_5_irq_handler,            // EXTI Line [9:5]
    [40] = tim1_brk_tim9_irq_handler,      // TIM1 Break and TIM9
    [41] = tim1_up_tim10_irq_handler,      // TIM1 Update and TIM10
    [42] = tim1_trg_com_tim11_irq_handler, // TIM1 Trigger/Commutation and TIM11
    [43] = tim1_cc_irq_handler,            // TIM1 Capture Compare
    [44] = tim2_irq_handler,               // TIM2
    [45] = tim3_irq_handler,               // TIM3
    [46] = tim4_irq_handler,               // TIM4
    [47] = i2c1_ev_irq_handler,            // I2C1 Event
    [48] = i2c1_er_irq_handler,            // I2C1 Error
    [49] = i2c2_ev_irq_handler,            // I2C2 Event
    [50] = i2c2_er_irq_handler,            // I2C2 Error
    [51] = spi1_irq_handler,               // SPI1
    [52] = spi2_irq_handler,               // SPI2
    [53] = usart1_irq_handler,             // USART1
    [54] = usart2_irq_handler,             // USART2
    [55] = usart3_irq_handler,             // USART3
    [56] = exti15_10_irq_handler,          // EXTI Line [15:10]
    [57] = rtc_alarm_irq_handler,          // RTC Alarm (A and B)
    [58] = otg_fs_wkup_irq_handler,        // USB OTG FS Wakeup
    [59] = tim8_brk_tim12_irq_handler,     // TIM8 Break and TIM12
    [60] = tim8_up_tim13_irq_handler,      // TIM8 Update and TIM13
    [61] = tim8_trg_com_tim14_irq_handler, // TIM8 Trigger/Commutation and TIM14
    [62] = tim8_cc_irq_handler,            // TIM8 Capture Compare
    [63] = dma1_stream7_irq_handler,       // DMA1 Stream 7
    [64] = fsmc_irq_handler,               // FSMC
    [65] = sdio_irq_handler,               // SDIO
    [66] = tim5_irq_handler,               // TIM5
    [67] = spi3_irq_handler,               // SPI3
    [68] = uart4_irq_handler,              // UART4
    [69] = uart5_irq_handler,              // UART5
    [70] = tim6_dac_irq_handler,           // TIM6 and DAC1&2 underrun
    [71] = tim7_irq_handler,               // TIM7
    [72] = dma2_stream0_irq_handler,       // DMA2 Stream 0
    [73] = dma2_stream1_irq_handler,       // DMA2 Stream 1
    [74] = dma2_stream2_irq_handler,       // DMA2 Stream 2
    [75] = dma2_stream3_irq_handler,       // DMA2 Stream 3
    [76] = dma2_stream4_irq_handler,       // DMA2 Stream 4
    [77] = eth_irq_handler,                // Ethernet
    [78] = eth_wkup_irq_handler,           // Ethernet Wakeup
    [79] = can2_tx_irq_handler,            // CAN2 TX
    [80] = can2_rx0_irq_handler,           // CAN2 RX0
    [81] = can2_rx1_irq_handler,           // CAN2 RX1
    [82] = can2_sce_irq_handler,           // CAN2 SCE
    [83] = otg_fs_irq_handler,             // USB OTG FS
    [84] = dma2_stream5_irq_handler,       // DMA2 Stream 5
    [85] = dma2_stream6_irq_handler,       // DMA2 Stream 6
    [86] = dma2_stream7_irq_handler,       // DMA2 Stream 7
    [87] = usart6_irq_handler,             // USART6
    [88] = i2c3_ev_irq_handler,            // I2C3 event
    [89] = i2c3_er_irq_handler,            // I2C3 error
    [90] = otg_hs_ep1_out_irq_handler,     // USB OTG HS End Point 1 Out
    [91] = otg_hs_ep1_in_irq_handler,      // USB OTG HS End Point 1 In
    [92] = otg_hs_wkup_irq_handler,        // USB OTG HS Wakeup
    [93] = otg_hs_irq_handler,             // USB OTG HS
    [94] = dcmi_irq_handler,               // DCMI
    [95] = cryp_irq_handler,               // CRYP crypto
    [96] = hash_rng_irq_handler,           // Hash and Rng
    [97] = fpu_irq_handler,                // FPU
    [98] = uart7_irq_handler,              // UART7
    [99] = uart8_irq_handler,              // UART8
    [100] = spi4_irq_handler,              // SPI4
    [101] = spi5_irq_handler,              // SPI5
    [102] = spi6_irq_handler,              // SPI6
    [103] = sai1_irq_handler,              // SAI1
    [104] = lcd_tft_irq_handler,           // LCD-TFT
    [105] = lcd_tft_err_irq_handler,       // LCD-TFT Error
    [106] = dma2d_irq_handler              // DMA2D
};

// vim: et nowrap autoindent: