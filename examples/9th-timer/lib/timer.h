/**
 * * TIMER interface for STM32 microcontrollers.
 * * Copyright (c) 2026 STM32World <lth@stm32world.com>
 * See LICENSE for details.
 * */

#ifndef __TIMER_H
#define __TIMER_H

#include "f4x.h"

struct timer {
    union { // CR1: Control Register 1
        volatile uint32_t CR1;
        struct {
            volatile uint32_t CEN : 1;  // Counter enable
            volatile uint32_t UDIS : 1; // Update disable
            volatile uint32_t URS : 1;  // Update request source
            volatile uint32_t OPM : 1;  // One-pulse mode
            volatile uint32_t DIR : 1;  // Direction (0: up, 1: down)
            volatile uint32_t CMS : 2;  // Center-aligned mode selection
            volatile uint32_t ARPE : 1; // Auto-reload preload enable
            volatile uint32_t CKD : 2;  // Clock division
            volatile uint32_t : 22;
        } CR1_b;
    };
    union { // CR2: Control Register 2
        volatile uint32_t CR2;
        struct {
            volatile uint32_t : 3;
            volatile uint32_t CCDS : 1; // Capture/compare DMA selection
            volatile uint32_t MMS : 3;  // Master mode selection
            volatile uint32_t TI1S : 1; // TI1 selection
            volatile uint32_t : 24;
        } CR2_b;
    };
    union { // SMCR: Slave Mode Control Register
        volatile uint32_t SMCR;
        struct {
            volatile uint32_t SMS : 3; // Slave mode selection
            volatile uint32_t : 1;
            volatile uint32_t TS : 3;   // Trigger selection
            volatile uint32_t MSM : 1;  // Master/Slave mode
            volatile uint32_t ETF : 4;  // External trigger filter
            volatile uint32_t ETPS : 2; // External trigger prescaler
            volatile uint32_t ECE : 1;  // External clock enable
            volatile uint32_t ETP : 1;  // External trigger polarity
            volatile uint32_t : 16;
        } SMCR_b;
    };
    union { // DIER: DMA/Interrupt Enable Register
        volatile uint32_t DIER;
        struct {
            volatile uint32_t UIE : 1;   // Update interrupt enable
            volatile uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable
            volatile uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable
            volatile uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable
            volatile uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable
            volatile uint32_t COMIE : 1; // COM interrupt enable
            volatile uint32_t TIE : 1;   // Trigger interrupt enable
            volatile uint32_t BIE : 1;   // Break interrupt enable
            volatile uint32_t UDE : 1;   // Update DMA request enable
            volatile uint32_t CC1DE : 1; // CC1 DMA request enable
            volatile uint32_t CC2DE : 1; // CC2 DMA request enable
            volatile uint32_t CC3DE : 1; // CC3 DMA request enable
            volatile uint32_t CC4DE : 1; // CC4 DMA request enable
            volatile uint32_t COMDE : 1; // COM DMA request enable
            volatile uint32_t TDE : 1;   // Trigger DMA request enable
            volatile uint32_t : 17;
        } DIER_b;
    };
    union { // SR: Status Register
        volatile uint32_t SR;
        struct {
            volatile uint32_t UIF : 1;   // Update interrupt flag
            volatile uint32_t CC1IF : 1; // CC1 interrupt flag
            volatile uint32_t CC2IF : 1; // CC2 interrupt flag
            volatile uint32_t CC3IF : 1; // CC3 interrupt flag
            volatile uint32_t CC4IF : 1; // CC4 interrupt flag
            volatile uint32_t COMIF : 1; // COM interrupt flag
            volatile uint32_t TIF : 1;   // Trigger interrupt flag
            volatile uint32_t BIF : 1;   // Break interrupt flag
            volatile uint32_t : 1;
            volatile uint32_t CC1OF : 1; // CC1 overcapture flag
            volatile uint32_t CC2OF : 1; // CC2 overcapture flag
            volatile uint32_t CC3OF : 1; // CC3 overcapture flag
            volatile uint32_t CC4OF : 1; // CC4 overcapture flag
            volatile uint32_t : 19;
        } SR_b;
    };
    union { // EGR: Event Generation Register
        volatile uint32_t EGR;
        struct {
            volatile uint32_t UG : 1;   // Update generation
            volatile uint32_t CC1G : 1; // CC1 generation
            volatile uint32_t CC2G : 1; // CC2 generation
            volatile uint32_t CC3G : 1; // CC3 generation
            volatile uint32_t CC4G : 1; // CC4 generation
            volatile uint32_t COMG : 1; // COM generation
            volatile uint32_t TG : 1;   // Trigger generation
            volatile uint32_t BG : 1;   // Break generation
            volatile uint32_t : 24;
        } EGR_b;
    };
    union { // CCMR1: Capture/Compare Mode Register 1 (Ch 1 & 2)
        volatile uint32_t CCMR1;
        struct {
            // Output Compare Mode (simplification)
            volatile uint32_t CC1S : 2;  // Capture/Compare 1 selection
            volatile uint32_t OC1FE : 1; // Output Compare 1 fast enable
            volatile uint32_t OC1PE : 1; // Output Compare 1 preload enable
            volatile uint32_t OC1M : 3;  // Output Compare 1 mode
            volatile uint32_t OC1CE : 1; // Output Compare 1 clear enable
            volatile uint32_t CC2S : 2;  // Capture/Compare 2 selection
            volatile uint32_t OC2FE : 1; // Output Compare 2 fast enable
            volatile uint32_t OC2PE : 1; // Output Compare 2 preload enable
            volatile uint32_t OC2M : 3;  // Output Compare 2 mode
            volatile uint32_t OC2CE : 1; // Output Compare 2 clear enable
            volatile uint32_t : 16;
        } CCMR1_b;
    };
    union { // CCMR2: Capture/Compare Mode Register 2 (Ch 3 & 4)
        volatile uint32_t CCMR2;
        struct {
            volatile uint32_t CC3S : 2;  // Capture/Compare 3 selection
            volatile uint32_t OC3FE : 1; // Output Compare 3 fast enable
            volatile uint32_t OC3PE : 1; // Output Compare 3 preload enable
            volatile uint32_t OC3M : 3;  // Output Compare 3 mode
            volatile uint32_t OC3CE : 1; // Output Compare 3 clear enable
            volatile uint32_t CC4S : 2;  // Capture/Compare 4 selection
            volatile uint32_t OC4FE : 1; // Output Compare 4 fast enable
            volatile uint32_t OC4PE : 1; // Output Compare 4 preload enable
            volatile uint32_t OC4M : 3;  // Output Compare 4 mode
            volatile uint32_t OC4CE : 1; // Output Compare 4 clear enable
            volatile uint32_t : 16;
        } CCMR2_b;
    };
    union { // CCER: Capture/Compare Enable Register
        volatile uint32_t CCER;
        struct {
            volatile uint32_t CC1E : 1;  // Capture/Compare 1 output enable
            volatile uint32_t CC1P : 1;  // Capture/Compare 1 output polarity
            volatile uint32_t CC1NE : 1; // Capture/Compare 1 complementary output enable
            volatile uint32_t CC1NP : 1; // Capture/Compare 1 complementary output polarity
            volatile uint32_t CC2E : 1;  // Capture/Compare 2 output enable
            volatile uint32_t CC2P : 1;  // Capture/Compare 2 output polarity
            volatile uint32_t CC2NE : 1; // Capture/Compare 2 complementary output enable
            volatile uint32_t CC2NP : 1; // Capture/Compare 2 complementary output polarity
            volatile uint32_t CC3E : 1;  // Capture/Compare 3 output enable
            volatile uint32_t CC3P : 1;  // Capture/Compare 3 output polarity
            volatile uint32_t CC3NE : 1; // Capture/Compare 3 complementary output enable
            volatile uint32_t CC3NP : 1; // Capture/Compare 3 complementary output polarity
            volatile uint32_t CC4E : 1;  // Capture/Compare 4 output enable
            volatile uint32_t CC4P : 1;  // Capture/Compare 4 output polarity
            volatile uint32_t CC4NE : 1; // Capture/Compare 4 complementary output enable
            volatile uint32_t CC4NP : 1; // Capture/Compare 4 complementary output polarity
            volatile uint32_t : 16;
        } CCER_b;
    };
    volatile uint32_t CNT;  // Counter Register
    volatile uint32_t PSC;  // Prescaler
    volatile uint32_t ARR;  // Auto-reload Register
    volatile uint32_t RCR;  // Repetition Counter (TIM1/TIM8 only)
    volatile uint32_t CCR1; // Capture/compare register 1
    volatile uint32_t CCR2; // Capture/compare register 2
    volatile uint32_t CCR3; // Capture/compare register 3
    volatile uint32_t CCR4; // Capture/compare register 4
    volatile uint32_t BDTR; // Break and dead-time register
    volatile uint32_t DCR;  // DMA control register
    volatile uint32_t DMAR; // DMA address for full transfer
};

// Base addresses for STM32F407 Timers
#define TIMER1 ((volatile struct timer *)0x40010000)  // Advanced control timer
#define TIMER2 ((volatile struct timer *)0x40000000)  // General-purpose timer
#define TIMER3 ((volatile struct timer *)0x40000400)  // General-purpose timer
#define TIMER4 ((volatile struct timer *)0x40000800)  // General-purpose timer
#define TIMER5 ((volatile struct timer *)0x40000C00)  // General-purpose timer
#define TIMER6 ((volatile struct timer *)0x40001000)  // Basic timer, no capture/compare
#define TIMER7 ((volatile struct timer *)0x40001400)  // Basic timer, no capture/compare
#define TIMER8 ((volatile struct timer *)0x40010400)  // Advanced control timer
#define TIMER9 ((volatile struct timer *)0x40014000)  // General-purpose timer
#define TIMER10 ((volatile struct timer *)0x40014400) // General-purpose timer
#define TIMER11 ((volatile struct timer *)0x40014800) // General-purpose timer

// Helper Enums
enum {
    TIM_MODE_FROZEN = 0x0,
    TIM_MODE_ACTIVE_ON_MTCH = 0x1,
    TIM_MODE_INACTIVE_ON_MTCH = 0x2,
    TIM_MODE_TOGGLE = 0x3,
    TIM_MODE_FORCE_LOW = 0x4,
    TIM_MODE_FORCE_HIGH = 0x5,
    TIM_MODE_PWM1 = 0x6,
    TIM_MODE_PWM2 = 0x7,
};

void timer_init(volatile struct timer *tim, uint32_t hz);
void timer_setup_interrupt(volatile struct timer *tim, uint32_t hz, uint8_t irq_num);
void timer_enable(volatile struct timer *tim);
void timer_disable(volatile struct timer *tim);

#endif /* __TIMER_H */

// vim: ts=4 et nowrap