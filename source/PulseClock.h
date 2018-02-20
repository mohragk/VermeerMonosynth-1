//
//  PulseClock.h
//  Vermeer Monosynth
//
//  Created by Sander Vermeer on 19-02-18.
//  Copyright © 2018 MRVR inc. All rights reserved.
//

#ifndef PULSE_CLOCK_H
#define PULSE_CLOCK_H

class PulseClock
{
public:
    PulseClock() : sampleRate(44100.0), modulo(0.0), moduloInc(0.0), pulseLength(0.5), sample(0.0), isHigh(false)
    {
        
    }
    
    
   
    
    void setSampleRate(const double sr)
    {
        sampleRate = sr;
    }
    
    void setFrequency(const double f)
    {
        frequency.set(f);
    }
    
    
    void setPulseLength(double len)
    {
        pulseLength = len;
    }
    
    void resetModulo()
    {
        modulo = 0.0;
    }
    
    bool isPulseHigh()
    {
        return isHigh;
    }
    
    double nextSample()
    {
        return sample;
    }


	void update()
	{
		isHigh = false;
		double value = 0.0;

		if (modulo == 0.0)
			isHigh = true;

		moduloInc = getModuloIncrement(frequency.get(), sampleRate);


		if (modulo <= pulseLength) {
			value = 1.0;
		}
		else {
			value = 0.0;
		}





		modulo += moduloInc;

		while (modulo >= 1.0)
		{
			modulo = 0.0;
		}



		sample = value;
	}
    
private:
    
    
    double getModuloIncrement(double freq, double sr)
    {
        const double nyFreq = jmin( freq, sr / 2.0 );
        return nyFreq / sr;
        
    }
    
    
    double sampleRate, modulo, moduloInc, pulseLength, sample;
    Atomic<double> frequency;
    
    bool isHigh;
	
};

#endif /* PULSE_CLOCK_H */
