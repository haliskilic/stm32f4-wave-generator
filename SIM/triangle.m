function [output] = triangle(tIn,freq,Samp)

T = 1/freq;
m1 = Samp / T;

if tIn == T
    output = Samp;
else
    output = m1*tIn;
end

if output > Samp
    output = Samp;
end

end

