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

#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum {
    WAVEFORM_OK,
    WAVEFORM_ERROR
}WAVEFORM_StatusTypeDef;

typedef struct WAVEFORM{
    uint16_t dutyCycleArray[40001];
    uint32_t dutyCycleSize;

    uint16_t timerCounter;
    double timerFreq;
    double pwmPeriod;
	double pwmFreq;
}WAVEFORM_HandleTypeDef;

/*User Functions*/
WAVEFORM_StatusTypeDef initWAVEFORM(struct WAVEFORM *WAVEFORM);
uint16_t* calculateDutyArray(struct WAVEFORM *WAVEFORM,uint8_t waveType, double waveAmp, uint32_t freq, uint32_t *waveArraySize);

/*Waveform Functions*/
double rampWave(double time, double waveAmp, uint32_t freq);
double triangleWave(double time, double waveAmp, uint32_t freq);
double sinWave(double time, double waveAmp, uint32_t freq);
double squareWave(double time, double waveAmp, uint32_t freq);
double whiteNoiseWave(double time, double waveAmp, uint32_t freq);

/*Optional*/
extern WAVEFORM_HandleTypeDef WAVEFORM;

#ifdef __cplusplus
}
#endif

#endif