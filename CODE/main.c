#include <stm32f407xx.h>
#include <system_stm32f4xx.h>
#include "buffer.h"
#include "waveform.h"
#include "stm32f4_settings.h"

int main(void)
{
	set_sysclk_to_168();
	//set_blink_settings();
	set_usart2_settings();

	BUFFER_Init(&BUF);
	initWAVEFORM(&WAVEFORM);

	while(1)
    {
        if(updateWaveparam() == 0)  // 0: Parameters Ready for using
        {
					calculateDutyArray(&WAVEFORM,WAVE.wavetype-1,(((float)WAVE.amp)/1000)/0.79,WAVE.freq,&(WAVEFORM.dutyCycleSize));        
					set_tim4_settings();
					//led_toggle();
					
        }
    }
}
