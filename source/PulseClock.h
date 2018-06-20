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
    PulseClock() : sampleRate(44100.0), sample(0.0),moduloInc(0.0), modulo(0.0),  pulseLength(0.5), isHigh(false)
    {    
    }
    
    ~PulseClock() {}

    void setSampleRate(const double sr)
    {
        sampleRate = sr;
    }
    
    void setFrequency(const double f)
    {
        frequency.set(f);
    }
    
    double getFrequency() { return frequency.get(); };
    
    void setPulseLength(double len)
    {
        pulseLength.set(len);
    }
    
    void resetModulo()
    {
        modulo.set(0.0);
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

		if (modulo.get() == 0.0)
			isHigh = true;

		moduloInc = getModuloIncrement(frequency.get(), sampleRate);

		if (modulo.get() <= pulseLength.get()) 
			value = 1.0;
		else 
			value = 0.0;
		

		modulo.set( modulo.get() + moduloInc );

		while (modulo.get() >= 1.0)
			modulo.set(0.0);
		

		sample = value;
	}
    
private:
    
    double getModuloIncrement(double freq, double sr)
    {
        const double nyFreq = jmin( freq, sr / 2.0 );
        return nyFreq / sr;
        
    }
    
    
    double sampleRate, sample, moduloInc;
    Atomic<double> modulo, pulseLength, frequency;
    
    bool isHigh;
	
	JUCE_LEAK_DETECTOR(PulseClock)
};

#endif /* PULSE_CLOCK_H */
