function [output] = rampWave(time,waveAmp,freq)
		wavePeriod = 1.0/freq;
		halfPeriod = wavePeriod / 2.0;
	
		if time <= halfPeriod
			output =  waveAmp*(time/halfPeriod);
		else
			m = (waveAmp)/(halfPeriod-wavePeriod);
			output =  m*(time - wavePeriod);
        end
end

