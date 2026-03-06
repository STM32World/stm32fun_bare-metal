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

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // VECTORS_H