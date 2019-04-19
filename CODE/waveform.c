#include "waveform.h"

WAVEFORM_HandleTypeDef WAVEFORM;

WAVEFORM_StatusTypeDef initWAVEFORM(struct WAVEFORM *WAVEFORM){
    /*Timer Parameters*/
    WAVEFORM->timerCounter  = 168;
    WAVEFORM->timerFreq     = 84000000.0;

    /*Calculation Parameters*/
    WAVEFORM->pwmPeriod     = (float)(1.0/(WAVEFORM->timerFreq))*(float)(WAVEFORM->timerCounter);
		WAVEFORM->pwmFreq				= 1.0 / WAVEFORM->pwmPeriod;

    return WAVEFORM_OK;
}

float rampWave(float time, float waveAmp, uint32_t freq){
    float wavePeriod;
    wavePeriod = 1.0/freq;
    return waveAmp*(time/wavePeriod);
}

float squareWave(float time, float waveAmp, uint32_t freq){
		float wavePeriod;
		wavePeriod = 1.0/freq;
		if(time <= (wavePeriod/2)){
			return waveAmp;
		}else{
			return 0;
		}
}

float whiteNoiseWave(float time, float waveAmp, uint32_t freq){
	return rand();
}

float triangleWave(float time, float waveAmp, uint32_t freq){
		float wavePeriod;
		wavePeriod = 1.0/freq;
		float halfPeriod = wavePeriod / 2.0;
	
		if( time <= halfPeriod){
			return waveAmp*(time/halfPeriod);
		}else{
			float m = (waveAmp)/(halfPeriod-wavePeriod);
			return m*(time - wavePeriod);
		}
}

float sinWave(float time, float waveAmp, uint32_t freq){

	float sinAmp = waveAmp/2;
	
	return sinAmp*sin(2*3.141592*freq*time) + sinAmp;
}

/**
 * waveType:																									
 * 0 - Ramp Waveform;
 * 1 - Triangle Waveform;
 * 2 - Sinus Waveform;
 * 3 - Square Waveform;
 * 4 - whiteNoise Waveform;																									
 **/
uint16_t* calculateDutyArray(struct WAVEFORM *WAVEFORM,uint8_t waveType, float waveAmp, uint32_t waveFreq, uint32_t *waveArraySize){
	
		float wavePeriod 	= 1.0/waveFreq;
		float currentTime 	= 0.0;
	
		uint8_t finishCheck = 1;
		WAVEFORM->dutyCycleSize = 0;
	
		while(finishCheck){
			if(waveType == 0){
				if(currentTime <= (wavePeriod+WAVEFORM->pwmPeriod)){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (((WAVEFORM->timerCounter/3.3))*(rampWave(currentTime,waveAmp,waveFreq)));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize;
				}
			}else if(waveType == 1){
				if(currentTime <=(wavePeriod+WAVEFORM->pwmPeriod)){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (uint16_t)(((WAVEFORM->timerCounter/3.3))*(triangleWave(currentTime,waveAmp,waveFreq)));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize;
				}
			}else if(waveType == 2){
				if(currentTime <= wavePeriod){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (uint16_t)(((WAVEFORM->timerCounter/3.3))*(sinWave(currentTime,waveAmp,waveFreq)));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize-1;
				}
			}else if(waveType == 3){
				if(currentTime <= wavePeriod){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (uint16_t)(((WAVEFORM->timerCounter/3.3))*(squareWave(currentTime,waveAmp,waveFreq)));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize;
				}
			}else if(waveType == 4){
				if(currentTime <= wavePeriod){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (uint16_t)(whiteNoiseWave(currentTime,waveAmp,waveFreq));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize;
				}
			}
			
			currentTime = currentTime + WAVEFORM->pwmPeriod;
		}

    return WAVEFORM->dutyCycleArray;
}
