#include "../Inc/waveform.h"

/*Optional*/
//WAVEFORM_HandleTypeDef WAVEFORM;

/**
 * Brief: This function sets timer parameters and calculates PWM parameters.
 **/
WAVEFORM_StatusTypeDef initWAVEFORM(struct WAVEFORM *WAVEFORM){
    /*Timer Parameters*/
    WAVEFORM->timerCounter  = 84;	//Timer Counter;
    WAVEFORM->timerFreq     = 168000000.0; //Timer Frequency;

    /*Calculation Parameters*/
    WAVEFORM->pwmPeriod     = (double)(1.0/(WAVEFORM->timerFreq))*(double)(WAVEFORM->timerCounter); //Sample Period;
		WAVEFORM->pwmFreq				= 1.0 / WAVEFORM->pwmPeriod;	//Sample Frequency;

    return WAVEFORM_OK;
}

/**
 * Brief: This function returns adress of duty cycle array;
 * waveType:																									
 * 0 - Ramp Waveform;
 * 1 - Triangle Waveform;
 * 2 - Sinus Waveform;
 * 3 - Square Waveform;
 * 4 - whiteNoise Waveform;
 * waveAmp: Amplitude of desired function;
 * waveFreq: Frequencyo of desired function;
 * waveArraySize: The function writes duty cycle size to given adress.					
 **/
uint16_t* calculateDutyArray(struct WAVEFORM *WAVEFORM,uint8_t waveType, double waveAmp, uint32_t waveFreq, uint32_t *waveArraySize){
	
		double wavePeriod 	= 1.0/waveFreq;
		double currentTime 	= 0.0;
	
		uint8_t finishCheck = 1;
		WAVEFORM->dutyCycleSize = 0;
	
		while(finishCheck){
			if(waveType == 0){
				if(currentTime <= wavePeriod){
					WAVEFORM->dutyCycleArray[WAVEFORM->dutyCycleSize] = (uint16_t)(((WAVEFORM->timerCounter/3.3))*(rampWave(currentTime,waveAmp,waveFreq)));
					WAVEFORM->dutyCycleSize ++;
				}else{
					finishCheck = 0;
					*waveArraySize = WAVEFORM->dutyCycleSize;
				}
			}else if(waveType == 1){
				if(currentTime <= wavePeriod){
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

double rampWave(double time, double waveAmp, uint32_t freq){
    double wavePeriod;
    wavePeriod = 1.0/freq;
    return waveAmp*(time/wavePeriod);
}

double squareWave(double time, double waveAmp, uint32_t freq){
		double wavePeriod;
		wavePeriod = 1.0/freq;
		if(time <= (wavePeriod/2)){
			return waveAmp;
		}else{
			return 0;
		}
}

double whiteNoiseWave(double time, double waveAmp, uint32_t freq){
	return rand();
}

double triangleWave(double time, double waveAmp, uint32_t freq){
		double wavePeriod;
		wavePeriod = 1.0/freq;
		double halfPeriod = wavePeriod / 2.0;
	
		if( time <= halfPeriod){
			return waveAmp*(time/halfPeriod);
		}else{
			double m = (waveAmp)/(halfPeriod-wavePeriod);
			return m*(time - wavePeriod);
		}
}

double sinWave(double time, double waveAmp, uint32_t freq){
	double wavePeriod;
	wavePeriod = 1.0/freq;
	double sinAmp = waveAmp/2;
	
	return sinAmp*sin(2*3.141592*freq*time) + sinAmp;
}
