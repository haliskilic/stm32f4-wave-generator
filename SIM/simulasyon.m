clc
clear
close all
format short

freq    = 10*10^3;      %Signal Freq.
period  = 1/freq;
amp     = 3.3;          %Signal Amp.
%% 
timerFreq = 168*10^6;
timerPeriod = 1/timerFreq;
timerCounter = 1000;
pwmPeriod = (1/timerFreq)*timerCounter; 
pwmFreq = 1 / pwmPeriod;

t = 0:timerPeriod:period;
arrayNumber = round(period/pwmPeriod);

index = 1;

for t = pwmPeriod:pwmPeriod:period
    ampArray(index) = triangle(t,freq);
    index = index+1;
end