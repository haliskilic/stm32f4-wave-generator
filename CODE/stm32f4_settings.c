#include "stm32f4_settings.h"

/*************************************************
* default interrupt handler
*************************************************/
void Default_Handler(void)
{
	for (;;);  // Wait forever
}

/*************************************************
* set Usart2 settings
*************************************************/
void set_usart2_settings(void)
{
    // enable GPIOA clock, bit 0 on AHB1ENR
    RCC->AHB1ENR |= (1 << 0);
    // enable USART2 clock, bit 17 on APB1ENR
    RCC->APB1ENR |= (1 << 17);
    // set pin modes as alternate mode 7 (pins 2 and 3)
    GPIOA->MODER &= 0xFFFFFF0F;     // Reset bits 10-15 to clear old values
    GPIOA->MODER |= 0x000000A0;     // Set pin 2/3 to alternate func. mode (0b10)
    // set pin modes as high speed
    GPIOA->OSPEEDR |= 0x000000A0;   // Set pin 2/3 to high speed mode (0b10)
    // choose AF7 for USART2 in Alternate Function registers
    GPIOA->AFR[0] |= (0x7 << 8);    // for pin 2
    GPIOA->AFR[0] |= (0x7 << 12);   // for pin 3
        // usart2 word length M, bit 12
    //USART2->CR1 |= (0 << 12);     // 0 - 1,8,n

    // usart2 parity control, bit 9
    //USART2->CR1 |= (0 << 9);  // 0 - no parity

    // usart2 tx enable, TE bit 3
    USART2->CR1 |= (1 << 3);

    // usart2 rx enable, RE bit 2
    USART2->CR1 |= (1 << 2);

    // baud rate = fCK / (8 * (2 - OVER8) * USARTDIV)
    //   for fCK = 42 Mhz, baud = 115200, OVER8 = 0
    //   USARTDIV = 42Mhz / 115200 / 16
    //   = 22.7864 22.8125
    // we can also look at the table in RM0090
    //   for 42 Mhz PCLK, OVER8 = 0 and 115.2 KBps baud
    //   we need to program 22.8125
    // Fraction : 16*0.8125 = 13 (multiply fraction with 16)
    // Mantissa : 22
    // 12-bit mantissa and 4-bit fraction
    USART2->BRR |= (22 << 4);
    USART2->BRR |= 13;

    // enable usart2 - UE, bit 13
    USART2->CR1 |= (1 << 13);
		
    // enable usart2 tx & rx interrupt
    USART2->CR1 |= (1 << 5);// | (1 << 7);        //RXNEIE TXEIE [RM0090 -> page1012]
    NVIC->IP[USART2_IRQn] = 0x05;   // Priority level 1
    NVIC_EnableIRQ(USART2_IRQn);
}
/*************************************************
* set Usart2 Handler
*************************************************/
void USART2_IRQHandler(void)
{
	// check if the source is transmit interrupt
	if (USART2->SR & (1 << 5)) 
	{
		// clear interrupt
		USART2->SR &= (uint32_t)~(1 << 5);
		buffer_write(&BUF, (uint8_t)USART2->DR);
	}
    // check if the source is transmit interrupt
}

/*************************************************
* set Blink settings
*************************************************/
void set_blink_settings(void)
{
    //RCC->AHB1ENR |= 0x00000008;     // enable gipod clock
    GPIOD->MODER &= 0xFCFFFFFF;     // Reset bits 25:24 to clear old values
    GPIOD->MODER |= 0x01000000;     // Set MODER bits 25:24 to 01
    GPIOD->ODR |= 0x1000;
}

/*************************************************
* configure system clock to 168 Mhz
* this is only tested on stm32f4 discovery board
*************************************************/
void set_sysclk_to_168(void)
{
	reset_clock();

	/* Enable HSE (CR: bit 16) */
	RCC->CR |= ((uint32_t) (1 << 16));
	/* Wait till HSE is ready (CR: bit 17) */
	while(!(RCC->CR & (1 << 17)));

	/* Enable power interface clock (APB1ENR:bit 28) */
	RCC->APB1ENR |= (1 << 28);

	/* set voltage scale to 1 for max frequency (PWR_CR:bit 14)
	 * (0b0) scale 2 for fCLK <= 144 Mhz
	 * (0b1) scale 1 for 144 Mhz < fCLK <= 168 Mhz
	 */
	PWR->CR |= (1 << 14);

	/* set AHB prescaler to /1 (CFGR:bits 7:4) */
	RCC->CFGR |= (0 << 4);
	/* set ABP low speed prescaler to /4 (APB1) (CFGR:bits 12:10) */
	RCC->CFGR |= (0x5 << 10);
	/* set ABP high speed prescaper to /2 (ABP2) (CFGR:bits 15:13) */
	RCC->CFGR |= (0x4 << 13);

	/* Set M, N, P and Q PLL dividers
	 * PLLCFGR: bits 5:0 (M), 14:6 (N), 17:16 (P), 27:24 (Q)
	 * Set PLL source to HSE, PLLCFGR: bit 22, 1:HSE, 0:HSI
	 */
	RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
				   (PLL_Q << 24) | (1 << 22);
	/* Enable the main PLL (CR: bit 24) */
	RCC->CR |= (1 << 24);
	/* Wait till the main PLL is ready (CR: bit 25) */
	while(!(RCC->CR & (1 << 25)));
	/* Configure Flash
	 * prefetch enable (ACR:bit 8)
	 * instruction cache enable (ACR:bit 9)
	 * data cache enable (ACR:bit 10)
	 * set latency to 5 wait states (ARC:bits 2:0)
	 *   see Table 10 on page 80 in RM0090
	 */
	FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (0x5 << 0);

	/* Select the main PLL as system clock source, (CFGR:bits 1:0)
	 * 0b00 - HSI
	 * 0b01 - HSE
	 * 0b10 - PLL
	 */
	RCC->CFGR &= (uint32_t)~(0x3 << 0);
	RCC->CFGR |= (0x2 << 0);
	/* Wait till the main PLL is used as system clock source (CFGR:bits 3:2) */
	while (!(RCC->CFGR & (uint32_t)(0x2 << 2)));
}

/*************************************************
* reset clock to HSI
*************************************************/
void reset_clock(void)
{
	/* Reset the RCC clock configuration to the default reset state */
	/* Set HSION bit */
	RCC->CR |= (1U << 0);

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON (16), CSSON (19) and PLLON (24) bits */
	RCC->CR &= ~(uint32_t)((1U << 16) | (1U << 19) | (1U << 24));

	/* Reset PLLCFGR register to reset value*/
	RCC->PLLCFGR = 0x24003010UL;

	/* Reset HSEBYP bit */
	RCC->CR &= ~(uint32_t)(1U << 18);

	/* Disable all clock interrupts */
	RCC->CIR = 0x00000000UL;
}

/*************************************************
* delay Function
*************************************************/
void delay(volatile uint32_t s)
{
	for(s; s>0; s--){
		// Do nothing
	}
}
/*************************************************
* Green LED Toggle Function
*************************************************/
void led_toggle(void)
{
	GPIOD->ODR ^= (1 << 12);  // Toggle LED for debug
}
/*************************************************
* TIM4 Global Interrupt Settings Function
*************************************************/
void set_tim4_settings(void)
{
	// enable GPIOD clock
	RCC->AHB1ENR |= (1 << 3);
	GPIOD->MODER &= 0x00FFFFFF;   // Reset bits 31-24 to clear old values
	GPIOD->MODER |= 0x02000000;   // Set pin 12 to alternate func. mode (0b10)

	// Choose Timer4 as Alternative Function for pin 12 led
	GPIOD->AFR[1] |= (0x2 << 16);

	// enable TIM4 clock (bit2)
	RCC->APB1ENR |= (1 << 2);

	// set prescaler to 167
	//   it will increment counter every prescalar cycles
	// fCK_PSC / (PSC[15:0] + 1)
	// 168 Mhz / 167 + 1 = 1 Mhz timer clock speed
	TIM4->PSC = 0;

	// set period
	TIM4->ARR = 168;

	// set duty cycle on channel 1
	TIM4->CCR1 = 42;

	// enable channel 1 in capture/compare register
	// set oc1 mode as pwm (0b110 or 0x6 in bits 6-4)
	TIM4->CCMR1 |= (0x6 << 4);
	// enable oc1 preload bit 3
	TIM4->CCMR1 |= (1 << 3);

	// enable capture/compare ch1 output
	TIM4->CCER |= (1 << 0);

	// enable interrupt CC1IE (bit 1)
	TIM4->DIER |= (1 << 0);

	// enable TIM4 IRQ from NVIC
	NVIC->IP[TIM4_IRQn] = 0x07;   // Priority level 1
	NVIC_EnableIRQ(TIM4_IRQn);

	// Enable Timer 4 module (CEN, bit0)
	TIM4->CR1 |= (1 << 0);

    
}
/*************************************************
* TIM4 Global Interrupt Handler Function
*************************************************/
void TIM4_IRQHandler(void)
{
		TIM4->SR = (uint16_t)(~(1 << 0));
    static uint32_t arrCount = 0;
    static uint32_t arrSize =0;
    if(arrSize != WAVEFORM.dutyCycleSize)
    {
        arrSize = WAVEFORM.dutyCycleSize;
        arrCount = 0;
    }
    if(arrCount == WAVEFORM.dutyCycleSize) arrCount = 0;
    TIM4->CCR1 = WAVEFORM.dutyCycleArray[arrCount];
    arrCount++;
}

