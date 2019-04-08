function [output] = rampWave(time,waveAmp,freq)
    wavePeriod = 1/freq;
    output = waveAmp*(time/wavePeriod);
end

