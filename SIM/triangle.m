function [output] = triangle(tIn,freq,Amp)
    T = 1/freq;
    m1 = Amp / T;

    if tIn == T
        output = Amp;
    else
        output = m1*tIn;
    end
    if output > Amp
        output = Amp;
    end
end

