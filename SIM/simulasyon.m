clc
clear
close all
format short


%% 
timerFreq = 168*10^6;
timerPeriod = 1/timerFreq;
timerCounter = 84;
pwmPeriod = (1/timerFreq)*timerCounter; 
pwmFreq = 1 / pwmPeriod;
%%
freq    = 17*10^3;      %Signal Freq.
amp     = 3.3;          %Signal Amp.

%% Calculating Duty Array;

tempArraySize = int(((1/freq)/(pwmPeriod)));