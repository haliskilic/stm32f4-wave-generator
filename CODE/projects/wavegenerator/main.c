#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Inc/waveform.h"

/*************************************************
* function declarations
*************************************************/
int main(void);
void Default_Handler(void);

/*************************************************
* Defines
*************************************************/
#define BUFSIZE 24

/*************************************************
* Structs
*************************************************/
typedef struct __BUFFER
{
    uint32_t in_indx;       //next index for writing
    uint32_t out_indx;      //next index for reading
    uint8_t buf[BUFSIZE];
} BUFFER;

typedef struct __WAVEGEN
{
    uint8_t wavetype;       // WaveType [1]:Sine [2]:Square [3]:Triangular [4]:Sawtooth(ramp) [5]:WhiteNoise
    uint32_t amp;           // Amplitude 0mV-3300mV
    uint32_t freq;          // Range 0hz-1MHz
    uint8_t genstatus; // [0]: Wave is generating [1]: Waiting WaveType [2]: Waiting amp [3]: Waiting freq
} WAVEGEN;

/*************************************************
* Users 
*************************************************/
BUFFER BUF;
WAVEGEN WAVE;
uint32_t arraySize;
/*************************************************
* function declarations
*************************************************/
void Default_Handler(void);
int main(void);
void Usart2_Handler(void);
void set_usart2_settings(void);
void set_blink_settings(void);
void buffer_write(BUFFER *, uint8_t);
void buffer_write_decimal2array(BUFFER *, uint32_t);
void buffer_read(BUFFER *, uint8_t*);
void buffer_read_array2decimal(BUFFER *, uint32_t*);
void send_usart2_arrmessage(uint8_t *array, uint8_t size);

/*************************************************
* Vector Table
*************************************************/
// get the stack pointer location from linker
typedef void (* const intfunc)(void);
extern unsigned long __stack;

// attribute puts table in beginning of .vectors section
//   which is the beginning of .text section in the linker script
// Add other vectors -in order- here
// Vector table can be found on page 372 in RM0090
__attribute__ ((section(".vectors")))
void (* const vector_table[])(void) = {
	(intfunc)((unsigned long)&__stack), /* 0x000 Stack Pointer */
	Reset_Handler,                      /* 0x004 Reset         */
	Default_Handler,                    /* 0x008 NMI           */
	Default_Handler,                    /* 0x00C HardFault     */
	Default_Handler,                    /* 0x010 MemManage     */
	Default_Handler,                    /* 0x014 BusFault      */
	Default_Handler,                    /* 0x018 UsageFault    */
	0,                                  /* 0x01C Reserved      */
	0,                                  /* 0x020 Reserved      */
	0,                                  /* 0x024 Reserved      */
	0,                                  /* 0x028 Reserved      */
	Default_Handler,                    /* 0x02C SVCall        */
	Default_Handler,                    /* 0x030 Debug Monitor */
	0,                                  /* 0x034 Reserved      */
	Default_Handler,                    /* 0x038 PendSV        */
	Default_Handler,                    /* 0x03C SysTick       */
	0,                                  /* 0x040 Window WatchDog Interrupt                                         */
	0,                                  /* 0x044 PVD through EXTI Line detection Interrupt                         */
	0,                                  /* 0x048 Tamper and TimeStamp interrupts through the EXTI line             */
	0,                                  /* 0x04C RTC Wakeup interrupt through the EXTI line                        */
	0,                                  /* 0x050 FLASH global Interrupt                                            */
	0,                                  /* 0x054 RCC global Interrupt                                              */
	0,                                  /* 0x058 EXTI Line0 Interrupt                                              */
	0,                                  /* 0x05C EXTI Line1 Interrupt                                              */
	0,                                  /* 0x060 EXTI Line2 Interrupt                                              */
	0,                                  /* 0x064 EXTI Line3 Interrupt                                              */
	0,                                  /* 0x068 EXTI Line4 Interrupt                                              */
	0,                                  /* 0x06C DMA1 Stream 0 global Interrupt                                    */
	0,                                  /* 0x070 DMA1 Stream 1 global Interrupt                                    */
	0,                                  /* 0x074 DMA1 Stream 2 global Interrupt                                    */
	0,                                  /* 0x078 DMA1 Stream 3 global Interrupt                                    */
	0,                                  /* 0x07C DMA1 Stream 4 global Interrupt                                    */
	0,                                  /* 0x080 DMA1 Stream 5 global Interrupt                                    */
	0,                                  /* 0x084 DMA1 Stream 6 global Interrupt                                    */
	0,                                  /* 0x088 ADC1, ADC2 and ADC3 global Interrupts                             */
	0,                                  /* 0x08C CAN1 TX Interrupt                                                 */
	0,                                  /* 0x090 CAN1 RX0 Interrupt                                                */
	0,                                  /* 0x094 CAN1 RX1 Interrupt                                                */
	0,                                  /* 0x098 CAN1 SCE Interrupt                                                */
	0,                                  /* 0x09C External Line[9:5] Interrupts                                     */
	0,                                  /* 0x0A0 TIM1 Break interrupt and TIM9 global interrupt                    */
	0,                                  /* 0x0A4 TIM1 Update Interrupt and TIM10 global interrupt                  */
	0,                                  /* 0x0A8 TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
	0,                                  /* 0x0AC TIM1 Capture Compare Interrupt                                    */
	0,                       			/* 0x0B0 TIM2 global Interrupt                                             */
	0,                                  /* 0x0B4 TIM3 global Interrupt                                             */
	tim4_handler,                       /* 0x0B8 TIM4 global Interrupt                                             */
	0,                                  /* 0x0BC I2C1 Event Interrupt                                              */
	0,                                  /* 0x0C0 I2C1 Error Interrupt                                              */
	0,                                  /* 0x0C4 I2C2 Event Interrupt                                              */
	0,                                  /* 0x0C8 I2C2 Error Interrupt                                              */
	0,                                  /* 0x0CC SPI1 global Interrupt                                             */
	0,                                  /* 0x0D0 SPI2 global Interrupt                                             */
	0,                                  /* 0x0D4 USART1 global Interrupt                                           */
	Usart2_Handler,                     /* 0x0D8 USART2 global Interrupt                                           */
	0,                                  /* 0x0DC USART3 global Interrupt                                           */
	0,                                  /* 0x0E0 External Line[15:10] Interrupts                                   */
	0,                                  /* 0x0E4 RTC Alarm (A and B) through EXTI Line Interrupt                   */
	0,                                  /* 0x0E8 USB OTG FS Wakeup through EXTI line interrupt                     */
	0,                                  /* 0x0EC TIM8 Break Interrupt and TIM12 global interrupt                   */
	0,                                  /* 0x0F0 TIM8 Update Interrupt and TIM13 global interrupt                  */
	0,                                  /* 0x0F4 TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
	0,                                  /* 0x0F8 TIM8 Capture Compare global interrupt                             */
	0,                                  /* 0x0FC DMA1 Stream7 Interrupt                                            */
	0,                                  /* 0x100 FSMC global Interrupt                                             */
	0,                                  /* 0x104 SDIO global Interrupt                                             */
	0,                                  /* 0x108 TIM5 global Interrupt                                             */
	0,                                  /* 0x10C SPI3 global Interrupt                                             */
	0,                                  /* 0x110 UART4 global Interrupt                                            */
	0,                                  /* 0x114 UART5 global Interrupt                                            */
	0,                                  /* 0x118 TIM6 global and DAC1&2 underrun error  interrupts                 */
	0,                                  /* 0x11C TIM7 global interrupt                                             */
	0,                                  /* 0x120 DMA2 Stream 0 global Interrupt                                    */
	0,                                  /* 0x124 DMA2 Stream 1 global Interrupt                                    */
	0,                                  /* 0x128 DMA2 Stream 2 global Interrupt                                    */
	0,                                  /* 0x12C DMA2 Stream 3 global Interrupt                                    */
	0,                                  /* 0x130 DMA2 Stream 4 global Interrupt                                    */
	0,                                  /* 0x134 Ethernet global Interrupt                                         */
	0,                                  /* 0x138 Ethernet Wakeup through EXTI line Interrupt                       */
	0,                                  /* 0x13C CAN2 TX Interrupt                                                 */
	0,                                  /* 0x140 CAN2 RX0 Interrupt                                                */
	0,                                  /* 0x144 CAN2 RX1 Interrupt                                                */
	0,                                  /* 0x148 CAN2 SCE Interrupt                                                */
	0,                                  /* 0x14C USB OTG FS global Interrupt                                       */
	0,                                  /* 0x150 DMA2 Stream 5 global interrupt                                    */
	0,                                  /* 0x154 DMA2 Stream 6 global interrupt                                    */
	0,                                  /* 0x158 DMA2 Stream 7 global interrupt                                    */
	0,                                  /* 0x15C USART6 global interrupt                                           */
	0,                                  /* 0x160 I2C3 event interrupt                                              */
	0,                                  /* 0x164 I2C3 error interrupt                                              */
	0,                                  /* 0x168 USB OTG HS End Point 1 Out global interrupt                       */
	0,                                  /* 0x16C USB OTG HS End Point 1 In global interrupt                        */
	0,                                  /* 0x170 USB OTG HS Wakeup through EXTI interrupt                          */
	0,                                  /* 0x174 USB OTG HS global interrupt                                       */
	0,                                  /* 0x178 DCMI global interrupt                                             */
	0,                                  /* 0x17C RNG global Interrupt                                              */
	0                                   /* 0x180 FPU global interrupt                                              */
};

/*************************************************
* default interrupt handler
*************************************************/
void Default_Handler(void)
{
	for (;;);  // Wait forever
}

/*************************************************
* timer 4 interrupt handler
*************************************************/
void tim4_handler(void)
{
	double x;
	static uint32_t t = 0;
	const uint32_t f = 10*period;
	// generate new value each time
	x = f/2 * ( sin(2 * M_PI * (float)t / (float)f) + 1);
	if (t == f)
	{
		t = 0;
	} else
	{
		++t;
	}

	// set new duty cycle
	TIM4->CCR1 = (uint16_t)(x/10);
}

/*************************************************
* set TIM4 settings
*************************************************/


/*************************************************
* default Usart2 interrupt handler
*************************************************/
void Usart2_Handler(void)
{
    USART2->SR &= ~USART_SR_RXNE;	// clear interrupt //RM0090/page1007
    // [FIX] check for transmit status ?
    buffer_write(&BUF, (uint8_t)USART2->DR);
    /*if(USART2->DR == 13)
    {
        uint32_t outdata;
        buffer_read_array2decimal(&BUF,&outdata);
        USART2->DR = outdata;
    }*/
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

    // enable usart2 tx & rx interrupt

    USART2->CR1 |= (1 << 5);        //RXNEIE TXEIE [RM0090 -> page1012]
    // enable usart2 - UE, bit 13
    USART2->CR1 |= (1 << 13);


    NVIC->IP[USART2_IRQn] = 0x10;   // Priority level 1
    NVIC_EnableIRQ(USART2_IRQn);
}


/*************************************************
* set Blink settings
*************************************************/
void set_blink_settings(void)
{
    RCC->AHB1ENR |= 0x00000008;     // enable gipod clock
    GPIOD->MODER &= 0xFCFFFFFF;     // Reset bits 25:24 to clear old values
    GPIOD->MODER |= 0x01000000;     // Set MODER bits 25:24 to 01
    GPIOD->ODR |= 0x1000;
}
/*************************************************
* Buffer Write Function
*************************************************/
void buffer_write(BUFFER *buf, uint8_t indata)
{
    buf->buf[buf->in_indx] = indata;
    if(buf->in_indx == BUFSIZE) 
        buf->in_indx=0;
    else
        buf->in_indx++;
    //check
    if(buf->in_indx == buf->out_indx)   // Buffer overflow
    {
        GPIOD->ODR ^= (1 << 12);		// Toggle LED
        buf->in_indx = 0;
        buf->out_indx = 0;
    }
}
/*************************************************
* Decimal Elements Convert to U8Array for BUFFER
* Used CR(13) for separating 
*************************************************/
void buffer_write_decimal2array(BUFFER *buf, uint32_t indecimal)
{
}

/*************************************************
* Buffer Read Function
*************************************************/
void buffer_read(BUFFER *buf, uint8_t* outdata)
{
    *outdata = buf->buf[buf->out_indx]; // [FIX] başlangıçta NULL dönmeli.
    if(buf->out_indx == BUFSIZE)
        buf->out_indx=0;
    else
        buf->out_indx++;
    //check
    if(buf->out_indx == buf->in_indx) // Buffer cleared
    {
        buf->in_indx = 0;
        buf->out_indx = 0;
        *outdata = 0;
    }

}
/*************************************************
* Buffer Elements Convert to Decimal
* Used CR(13) for separating 
*************************************************/
void buffer_read_array2decimal(BUFFER *buf, uint32_t *outdecimal)
{
    uint8_t outdata='0';
    *outdecimal=0;
    while(outdata != 13 && outdata != 0)
    {
        *outdecimal = *outdecimal*10 + (uint32_t)(outdata - '0');   //[FIX] 1cycle Gereksiz islem var.
        buffer_read(buf,&outdata);
    }
}

/*************************************************
* Send Array to USART2
*************************************************/
void send_usart2_arrmessage(uint8_t *arr, uint8_t size)
{

}

/*************************************************
* main code starts from here
*************************************************/

int main(void)
{
	/* set system clock to 168 Mhz */
	set_sysclk_to_168();
    set_usart2_settings();
    set_blink_settings();
	initWAVEFORM(&WAVEFORM);

    uint8_t q1[] = "\r\n- Choose Wave Form ?\r\nSine         (1)\r\nPWM          (2)\r\nTriangular   (3)\r\nRamp         (4)\r\nWhiteNoise   (5)\r\n#:";
    uint8_t q2[] = "\r\n- Set Freq. (500-1000000hz)\r\n#:";
    uint8_t q3[] = "\r\n- Set Amp. (0-3300mV)\r\n#:";
    //const uint8_t warning[] = "\r\n- Warning out of range number.\r\n";
    WAVE.genstatus=0;

    while(1)
    {
        if(WAVE.genstatus==0)
        {
            for (uint8_t i=0; i<sizeof(q1)-1; i++)
            {
                // send character
                USART2->DR = (uint32_t)q1[i];
                // wait for transmit complete
                while(!(USART2->SR & (1 << 6)));
            }
            while(1)
                if(BUF.in_indx != 0 && BUF.buf[BUF.in_indx-1] == 13)   // [FIX] 0. elemanda  girmez!
                {
                    uint32_t outdecimal;
                    buffer_read_array2decimal(&BUF, &outdecimal);
                    WAVE.wavetype=(uint8_t)outdecimal;
                    WAVE.genstatus++;
                    BUF.buf[BUF.in_indx-1] = 0;
                    break;
                }
        }
        else if(WAVE.genstatus==1)
        {
            for (uint8_t i=0; i<sizeof(q2)-1; i++)
            {
                // send character
                USART2->DR = (uint32_t)q2[i];
                // wait for transmit complete
                while(!(USART2->SR & (1 << 6)));
            }
            while(1)
                if(BUF.in_indx != 0 && BUF.buf[BUF.in_indx-1] == 13)   // [FIX] 0. elemanda  girmez!
                {
                    uint32_t outdecimal;
                    buffer_read_array2decimal(&BUF, &outdecimal);
                    WAVE.freq=outdecimal;
                    WAVE.genstatus++;
                    BUF.buf[BUF.in_indx-1] = 0;
                    break;
                }
        }
        else if(WAVE.genstatus==2)
        {
            for (uint8_t i=0; i<sizeof(q3)-1; i++)
            {
                // send character
                USART2->DR = (uint32_t)q3[i];
                // wait for transmit complete
                while(!(USART2->SR & (1 << 6)));
            }
            while(1)
                if(BUF.in_indx != 0 && BUF.buf[BUF.in_indx-1] == 13)   // [FIX] 0. elemanda  girmez!
                {
                    uint32_t outdecimal;
                    buffer_read_array2decimal(&BUF, &outdecimal);
                    WAVE.amp=outdecimal;
                    WAVE.genstatus=0;
                    BUF.buf[BUF.in_indx-1] = 0;
                    break;
                }
        }
		calculateDutyArray(&WAVEFORM,WAVE.wavetype,(double)(WAVE.amp/1000),WAVE.freq,&arraySize);
        // wait for transmit complete
        //while(!(USART2->SR & (1 << 6)));
        // slow down
        //for(int i=0; i<1000000; i++);
        //[IDEA] add wwdg
    }
    return 0;
}
