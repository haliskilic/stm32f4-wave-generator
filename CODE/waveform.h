/**
 * @file     waveform.h																									
 * @author   Mete Can Gazi																										
 * @brief    Waveform Library							
 * @revision 03.04.2019
 **/

#ifndef _WAVEFORM_H_
#define _WAVEFORM_H_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stm32f407xx.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
    WAVEFORM_OK,
    WAVEFORM_ERROR
}WAVEFORM_StatusTypeDef;

typedef struct WAVEFORM{
    uint16_t dutyCycleArray[40000];
    uint32_t dutyCycleSize;

    uint16_t timerCounter;
    float timerFreq;
    float pwmPeriod;
		float pwmFreq;
}WAVEFORM_HandleTypeDef;

WAVEFORM_StatusTypeDef initWAVEFORM(struct WAVEFORM *WAVEFORM);
uint16_t* calculateDutyArray(struct WAVEFORM *WAVEFORM,uint8_t waveType, float waveAmp, uint32_t freq, uint32_t *waveArraySize);

/*Waveform Functions*/
float rampWave(float time, float waveAmp, uint32_t freq);
float triangleWave(float time, float waveAmp, uint32_t freq);
float sinWave(float time, float waveAmp, uint32_t freq);
float squareWave(float time, float waveAmp, uint32_t freq);
float whiteNoiseWave(float time, float waveAmp, uint32_t freq);

extern WAVEFORM_HandleTypeDef WAVEFORM;

#ifdef __cplusplus
}
#endif

#endif
