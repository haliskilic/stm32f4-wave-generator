/**
  ******************************************************************************
  * @file    buffer.c
  * @author  Halis KILIÇ (byHK)
  * @brief   Bu dosya uart uzerinden gelen verilerin islenmesi icin fonksiyonlar
	*						icermektedir.
  @verbatim
  ==============================================================================
                        ##### ???? #####
  ==============================================================================
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 Halis KILIC (byHK)</center></h2>
  *
  ******************************************************************************
  */
#ifndef __BUFFER_H
#define __BUFFER_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <stm32f407xx.h>

/*************************************************
* Defines
*************************************************/
#define BUFSIZE 24 //Erzincan

/*************************************************
* Structs
*************************************************/
typedef enum
{
    BUFFER_ERROR,
    BUFFER_OK,
    BUFFER_BUSY,
    BUFFER_OVERFLOW,
    BUFFER_TIMEOUT
} BUFFER_StatusTypeDef;

typedef struct __BUFFER_HandleTypeDef
{
    uint32_t                in_indx;    //next index for writing
    uint32_t                out_indx;   //next index for reading
    BUFFER_StatusTypeDef 	status_overflow;
    uint32_t                buffer_count;
    uint8_t                 buf[BUFSIZE];
} BUFFER_HandleTypeDef;

typedef struct __WAVEGEN
{
    uint8_t wavetype;       // WaveType [1]:Sine [2]:Square [3]:Triangular [4]:Sawtooth(ramp) [5]:WhiteNoise
    uint32_t amp;           // Amplitude 0mV-3300mV
    uint32_t freq;          // Range 0hz-1MHz
    uint8_t genstatus; // [0]: Wave is generating [1]: Waiting WaveType [2]: Waiting amp [3]: Waiting freq
} WAVEGEN;

/*************************************************
* function declarations
*************************************************/
BUFFER_StatusTypeDef buffer_write(struct __BUFFER_HandleTypeDef *, uint8_t);
BUFFER_StatusTypeDef buffer_write_decimal2array(struct __BUFFER_HandleTypeDef *, uint32_t);
BUFFER_StatusTypeDef buffer_read(struct __BUFFER_HandleTypeDef *, uint8_t*);
BUFFER_StatusTypeDef buffer_read_array2decimal(struct __BUFFER_HandleTypeDef *, uint32_t*);
BUFFER_StatusTypeDef BUFFER_Init(struct __BUFFER_HandleTypeDef *);

uint8_t updateWaveparam(void);

extern BUFFER_HandleTypeDef BUF;
extern WAVEGEN WAVE;

#ifdef __cplusplus
  }
#endif

#endif
