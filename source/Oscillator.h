//
//  Oscillator.h
//  Vermeer Monosynth
//
//  Created by Sander Vermeer on 10-10-17.
//

#ifndef Oscillator_h
#define Oscillator_h

class Oscillator
{
public:
    Oscillator() : sampleRate(44100.0), phaseIncrement(0.0), level(0.75), pulsewidth(0.5), deviation(0.0), rephase(false)
    {
		frequency.set(0.0);
		phase.set(0.0);
		gain.set(0.0);
		
    }
    
    
    enum OscillatorMode {
        OSCILLATOR_MODE_SINE = 0,
        OSCILLATOR_MODE_SAW,
        OSCILLATOR_MODE_SQUARE,
        OSCILLATOR_MODE_NOISE
    };
    
    void setSampleRate(const double sr) noexcept
    {
        sampleRate = sr;
    }
    
    void setFrequency(const double f)
    {
        frequency.set(f + deviation);
    }
    
    
    void setPhase(const double ph)
    {
        phase.set(ph);
    }
    
    void setGain(const double g)
    {
        gain.set(g);
    }
    
    void setMode(const int m)
    {
        if (m == 0)
            mode = OSCILLATOR_MODE_SINE;
        else if (m == 1)
            mode = OSCILLATOR_MODE_SAW;
        else if (m == 2)
            mode = OSCILLATOR_MODE_SQUARE;
        else
            mode = OSCILLATOR_MODE_NOISE;
    }
    
	

    void setPulsewidth(double pw)
    {
        //pulsewidth = pw * 0.998 + 0.001; //(pw + 1.0) / 2.0;
        pulsewidth = (pw + 1.0) / 2.0;
    }
    
	void setVelocityFactor(float v)
	{
		velocityFactor = static_cast<double>(v);
	}
    
    double getPhase()
    {
        return phase.get();
    }
    
    bool isRephase()
    {
        return rephase;
    }

    double nextSample()
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;
        double t = phase.get() / two_Pi; // normalize period
        
        phaseIncrement = updatePhaseIncrement(frequency.get());

		if (phaseIncrement == 0.0)
			return value;
        
        if ( mode == OSCILLATOR_MODE_SINE)
        {
            value = naiveWaveFormForMode(mode, phase.get());
        }
        else if( mode == OSCILLATOR_MODE_SAW)
        {
            value = naiveWaveFormForMode(mode, phase.get());
            //value = dsp::FastMathApproximations::tanh(value * 3.0);
            value -= poly_blep( t, phaseIncrement );
        }
        else if (mode == OSCILLATOR_MODE_SQUARE)
        {
            value = naiveWaveFormForMode(mode, phase.get());
            value = dsp::FastMathApproximations::sinh(value * 3.0) / (3.0 * double_Pi);
            value += poly_blep( t, phaseIncrement );
            value -= poly_blep( fmod( t + (1.0 - pulsewidth), 1.0 ), phaseIncrement );
        }
        else
        {
            value = naiveWaveFormForMode(mode, phase.get());
        }
        
        phase.set( phase.get() + phaseIncrement); //NOT SURE.....
        
        rephase = false;
        
        if(phase.get() >= two_Pi)
        {
            phase.set(0.0);
            rephase = true;
			deviation *= -1.0;
            
        }
        
        return value * level * gain.get();// * velocityFactor;
    }
    
private:
    
    double naiveWaveFormForMode(const OscillatorMode m, double phs)
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;
       
        if (phs >= two_Pi)
            phs -= two_Pi;
        
        
        switch (m)
        {
            case OSCILLATOR_MODE_SINE:
                value = sin(phs);
                break;
                
            case OSCILLATOR_MODE_SAW:
                //value = tanh(3.0 * value);
                value = (2.0 * phs / two_Pi) - 1.0;
                //value = tanh(2.0 * value);
                break;
                
            case OSCILLATOR_MODE_SQUARE:
                if (phs <=  pulsewidth * two_Pi) { // BUG!!! 0.5 should be pulsewidth
                    value = 1.0 - (1.0 * phs / two_Pi);
                } else {
                    value = (0.5 * (phs - pulsewidth * two_Pi) / double_Pi ) - 1.0;
                }
                break;
                
            case OSCILLATOR_MODE_NOISE:
                value = (random.nextFloat() * 2.0 ) - 1.0;
                break;
                
            default:
                break;
        }
        return value;
    }
    
    
    double poly_blep (double t, const double phaseInc)
    {
        const double dt = phaseInc / (2.0 * double_Pi); // normalize phase increment
        
        if (t < dt)
        {
            t /= dt;
            return t+t - t*t - 1.0;
        }
        else if (t > 1.0 - dt)
        {
            t = (t - 1.0) / dt;
            return t*t + t+t + 1.0;
        }
        else return 0.0;
    }
    
    
    double updatePhaseIncrement(const double freq)
    {
        const double nyFreq = jmin( freq, sampleRate / 2.0 );
        return ( ( 2.0 * double_Pi ) * nyFreq ) / sampleRate;
    }
    

    double sampleRate,  phaseIncrement;
	double velocityFactor;
	Atomic<double> frequency, phase, gain;
    double level;
	double pulsewidth;
    
    OscillatorMode mode;
    Random random;
    double deviation;
    
    bool rephase;
    
};

#endif /* Oscillator_h */
