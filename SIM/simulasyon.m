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
timerCounter = 100;
pwmPeriod = (1/timerFreq)*timerCounter; 
pwmFreq = 1 / pwmPeriod;

t = 0:timerPeriod:period;
arrayNumber = round(period/pwmPeriod);
pwmArray = 0;

index = 1;

% for s=1:1:arrayNumber
%   
%   currentAmp = triangle(pwmPeriod*s,freq,amp);
%   hightolow = round(timerCounter*currentAmp/3.3);
%   
%   for m = 1:1:1001
%       if (m-1)>hightolow
%         pwmArray(index) = 0;
%       else
%         pwmArray(index) = amp;
%       end
%       index = index + 1;
%   end
% end
