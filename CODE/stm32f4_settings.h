/**
  ******************************************************************************
  * @file
  * @author
  * @brief
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#ifndef __STM32F4_SETTINGS_H_
#define __STM32F4_SETTINGS_H_

#ifdef __cplusplus
  extern "C" {
#endif

#include <stm32f407xx.h>
#include "buffer.h"
#include "waveform.h"

/* Clock PLLs for 407 chip */
#if defined (STM32F407xx)
// Main PLL = N * (source_clock / M) / P
// HSE = 8 Mhz
// fCLK =   N * (8Mhz / M) / P
#define PLL_M	8
#define PLL_Q 	7
#define PLL_P 	2
#endif

/* stm32f407 runs at 168Mhz max */
#if defined (STM32F407xx)
#define PLL_N 	336
#endif

/*************************************************
* function declarations
*************************************************/
void Default_Handler(void);
void delay(volatile uint32_t);
void set_usart2_settings(void);
void USART2_IRQHandler(void);
void TIM4_IRQHandler(void);
void set_blink_settings(void);
void reset_clock(void);
void set_sysclk_to_168(void);
void set_tim4_settings(void);
void led_toggle(void);

#ifdef __cplusplus
  }
#endif

#endif
