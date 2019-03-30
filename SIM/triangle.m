function [output] = triangle(tIn,freq)
    Amp = 3.3;  %H
    T = 1/freq; %X
    output = Amp*(tIn/T);
end

