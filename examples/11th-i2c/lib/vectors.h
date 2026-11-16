/**
 *
 * Interrupt vector definitions for STM32F4xx.
 *
 * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 *
 */

#ifndef VECTORS_H
#define VECTORS_H

// IRQ numbers (position in the NVIC, i.e. vector table index - 16)
enum {
    WWDG_IRQ = 0,                  // Window Watchdog
    PVD_IRQ = 1,                   // PVD through EXTI Line detect
    TAMP_STAMP_IRQ = 2,            // Tamper and TimeStamp
    RTC_WKUP_IRQ = 3,              // RTC Wakeup
    FLASH_IRQ = 4,                 // Flash
    RCC_IRQ = 5,                   // RCC
    EXTI0_IRQ = 6,                 // EXTI Line 0
    EXTI1_IRQ = 7,                 // EXTI Line 1
    EXTI2_IRQ = 8,                 // EXTI Line 2
    EXTI3_IRQ = 9,                 // EXTI Line 3
    EXTI4_IRQ = 10,                // EXTI Line 4
    DMA1_STREAM0_IRQ = 11,         // DMA1 Stream 0
    DMA1_STREAM1_IRQ = 12,         // DMA1 Stream 1
    DMA1_STREAM2_IRQ = 13,         // DMA1 Stream 2
    DMA1_STREAM3_IRQ = 14,         // DMA1 Stream 3
    DMA1_STREAM4_IRQ = 15,         // DMA1 Stream 4
    DMA1_STREAM5_IRQ = 16,         // DMA1 Stream 5
    DMA1_STREAM6_IRQ = 17,         // DMA1 Stream 6
    ADC_IRQ = 18,                  // ADC1, ADC2 and ADC3
    CAN1_TX_IRQ = 19,              // CAN1 TX
    CAN1_RX0_IRQ = 20,             // CAN1 RX0
    CAN1_RX1_IRQ = 21,             // CAN1 RX1
    CAN1_SCE_IRQ = 22,             // CAN1 SCE
    EXTI9_5_IRQ = 23,              // EXTI Line [9:5]
    TIM1_BRK_TIM9_IRQ = 24,        // TIM1 Break and TIM9
    TIM1_UP_TIM10_IRQ = 25,        // TIM1 Update and TIM10
    TIM1_TRG_COM_TIM11_IRQ = 26,   // TIM1 Trigger/Commutation and TIM11
    TIM1_CC_IRQ = 27,              // TIM1 Capture Compare
    TIM2_IRQ = 28,                 // TIM2
    TIM3_IRQ = 29,                 // TIM3
    TIM4_IRQ = 30,                 // TIM4
    I2C1_EV_IRQ = 31,              // I2C1 Event
    I2C1_ER_IRQ = 32,              // I2C1 Error
    I2C2_EV_IRQ = 33,              // I2C2 Event
    I2C2_ER_IRQ = 34,              // I2C2 Error
    SPI1_IRQ = 35,                 // SPI1
    SPI2_IRQ = 36,                 // SPI2
    USART1_IRQ = 37,               // USART1
    USART2_IRQ = 38,               // USART2
    USART3_IRQ = 39,               // USART3
    EXTI15_10_IRQ = 40,            // EXTI Line [15:10]
    RTC_ALARM_IRQ = 41,            // RTC Alarm (A and B)
    OTG_FS_WKUP_IRQ = 42,          // USB OTG FS Wakeup
    TIM8_BRK_TIM12_IRQ = 43,       // TIM8 Break and TIM12
    TIM8_UP_TIM13_IRQ = 44,        // TIM8 Update and TIM13
    TIM8_TRG_COM_TIM14_IRQ = 45,   // TIM8 Trigger/Commutation and TIM14
    TIM8_CC_IRQ = 46,              // TIM8 Capture Compare
    DMA1_STREAM7_IRQ = 47,         // DMA1 Stream 7
    FSMC_IRQ = 48,                 // FSMC
    SDIO_IRQ = 49,                 // SDIO
    TIM5_IRQ = 50,                 // TIM5
    SPI3_IRQ = 51,                 // SPI3
    UART4_IRQ = 52,                // UART4
    UART5_IRQ = 53,                // UART5
    TIM6_DAC_IRQ = 54,             // TIM6 and DAC1&2 underrun
    TIM7_IRQ = 55,                 // TIM7
    DMA2_STREAM0_IRQ = 56,         // DMA2 Stream 0
    DMA2_STREAM1_IRQ = 57,         // DMA2 Stream 1
    DMA2_STREAM2_IRQ = 58,         // DMA2 Stream 2
    DMA2_STREAM3_IRQ = 59,         // DMA2 Stream 3
    DMA2_STREAM4_IRQ = 60,         // DMA2 Stream 4
    ETH_IRQ = 61,                  // Ethernet
    ETH_WKUP_IRQ = 62,             // Ethernet Wakeup
    CAN2_TX_IRQ = 63,              // CAN2 TX
    CAN2_RX0_IRQ = 64,             // CAN2 RX0
    CAN2_RX1_IRQ = 65,             // CAN2 RX1
    CAN2_SCE_IRQ = 66,             // CAN2 SCE
    OTG_FS_IRQ = 67,               // USB OTG FS
    DMA2_STREAM5_IRQ = 68,         // DMA2 Stream 5
    DMA2_STREAM6_IRQ = 69,         // DMA2 Stream 6
    DMA2_STREAM7_IRQ = 70,         // DMA2 Stream 7
    USART6_IRQ = 71,               // USART6
    I2C3_EV_IRQ = 72,              // I2C3 event
    I2C3_ER_IRQ = 73,              // I2C3 error
    OTG_HS_EP1_OUT_IRQ = 74,       // USB OTG HS End Point 1 Out
    OTG_HS_EP1_IN_IRQ = 75,        // USB OTG HS End Point 1 In
    OTG_HS_WKUP_IRQ = 76,          // USB OTG HS Wakeup
    OTG_HS_IRQ = 77,               // USB OTG HS
    DCMI_IRQ = 78,                 // DCMI
    CRYP_IRQ = 79,                 // CRYP crypto
    HASH_RNG_IRQ = 80,             // Hash and Rng
    FPU_IRQ = 81,                  // FPU
    UART7_IRQ = 82,                // UART7
    UART8_IRQ = 83,                // UART8
    SPI4_IRQ = 84,                 // SPI4
    SPI5_IRQ = 85,                 // SPI5
    SPI6_IRQ = 86,                 // SPI6
    SAI1_IRQ = 87,                 // SAI1
    LCD_TFT_IRQ = 88,              // LCD-TFT
    LCD_TFT_ERR_IRQ = 89,          // LCD-TFT Error
    DMA2D_IRQ = 90,                // DMA2D
};

// Forward prototype declarations of all interrupt handlers

// Standard System Exceptions
void nmi_handler(void);
void hard_fault_handler(void);
void mem_manage_handler(void);
void bus_fault_handler(void);
void usage_fault_handler(void);
void svc_handler(void);
void debug_mon_handler(void);
void pendsv_handler(void);
void systick_handler(void);

// Peripheral Interrupts
void wwdg_irq_handler(void);
void pvd_irq_handler(void);
void tamp_stamp_irq_handler(void);
void rtc_wkup_irq_handler(void);
void flash_irq_handler(void);
void rcc_irq_handler(void);
void exti0_irq_handler(void);
void exti1_irq_handler(void);
void exti2_irq_handler(void);
void exti3_irq_handler(void);
void exti4_irq_handler(void);
void dma1_stream0_irq_handler(void);
void dma1_stream1_irq_handler(void);
void dma1_stream2_irq_handler(void);
void dma1_stream3_irq_handler(void);
void dma1_stream4_irq_handler(void);
void dma1_stream5_irq_handler(void);
void dma1_stream6_irq_handler(void);
void adc_irq_handler(void);
void can1_tx_irq_handler(void);
void can1_rx0_irq_handler(void);
void can1_rx1_irq_handler(void);
void can1_sce_irq_handler(void);
void exti9_5_irq_handler(void);
void tim1_brk_tim9_irq_handler(void);
void tim1_up_tim10_irq_handler(void);
void tim1_trg_com_tim11_irq_handler(void);
void tim1_cc_irq_handler(void);
void tim2_irq_handler(void);
void tim3_irq_handler(void);
void tim4_irq_handler(void);
void i2c1_ev_irq_handler(void);
void i2c1_er_irq_handler(void);
void i2c2_ev_irq_handler(void);
void i2c2_er_irq_handler(void);
void spi1_irq_handler(void);
void spi2_irq_handler(void);
void usart1_irq_handler(void);
void usart2_irq_handler(void);
void usart3_irq_handler(void);
void exti15_10_irq_handler(void);
void rtc_alarm_irq_handler(void);
void otg_fs_wkup_irq_handler(void);
void tim8_brk_tim12_irq_handler(void);
void tim8_up_tim13_irq_handler(void);
void tim8_trg_com_tim14_irq_handler(void);
void tim8_cc_irq_handler(void);
void dma1_stream7_irq_handler(void);
void fsmc_irq_handler(void);
void sdio_irq_handler(void);
void tim5_irq_handler(void);
void spi3_irq_handler(void);
void uart4_irq_handler(void);
void uart5_irq_handler(void);
void tim6_dac_irq_handler(void);
void tim7_irq_handler(void);
void dma2_stream0_irq_handler(void);
void dma2_stream1_irq_handler(void);
void dma2_stream2_irq_handler(void);
void dma2_stream3_irq_handler(void);
void dma2_stream4_irq_handler(void);
void eth_irq_handler(void);
void eth_wkup_irq_handler(void);
void can2_tx_irq_handler(void);
void can2_rx0_irq_handler(void);
void can2_rx1_irq_handler(void);
void can2_sce_irq_handler(void);
void otg_fs_irq_handler(void);
void dma2_stream5_irq_handler(void);
void dma2_stream6_irq_handler(void);
void dma2_stream7_irq_handler(void);
void usart6_irq_handler(void);
void i2c3_ev_irq_handler(void);
void i2c3_er_irq_handler(void);
void otg_hs_ep1_out_irq_handler(void);
void otg_hs_ep1_in_irq_handler(void);
void otg_hs_wkup_irq_handler(void);
void otg_hs_irq_handler(void);
void dcmi_irq_handler(void);
void cryp_irq_handler(void);
void hash_rng_irq_handler(void);
void fpu_irq_handler(void);
void uart7_irq_handler(void);
void uart8_irq_handler(void);
void spi4_irq_handler(void);
void spi5_irq_handler(void);
void spi6_irq_handler(void);
void sai1_irq_handler(void);
void lcd_tft_irq_handler(void);
void lcd_tft_err_irq_handler(void);
void dma2d_irq_handler(void);

#endif /* VECTORS_H */

// vim: et ts=4 nowrap autoindent
