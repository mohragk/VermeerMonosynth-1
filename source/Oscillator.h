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
    Oscillator() : sampleRate(44100.0), phaseIncrement(0.0), level(1.0), pulsewidth(0.5), rephase(false)
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
        frequency.set(f);
    }
    
    double getFrequency()
    {
        return frequency.get();
    }
    
    void setPhase(const double ph)
    {
        phase.set(ph);
    }

	
	void resetPhaseInterpolated()
	{
		phaseRemaining = 1.0 - phase.get();
		targetReached = false;
	}
    
    
    
    void setGain(const double g)
    {
        gain.set(g);
    }
    
    void setMode(const int m)
    {
        if      (m == 0) mode = OSCILLATOR_MODE_SINE;
        else if (m == 1) mode = OSCILLATOR_MODE_SAW;
        else if (m == 2) mode = OSCILLATOR_MODE_SQUARE;
        else             mode = OSCILLATOR_MODE_NOISE;
    }
    
    void setDetuneAmount(double fine, int coarse)
    {
        detuneAmount = fine + (double)coarse;
 
        oldDetuneAmount = detuneAmount;
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
        double value = 0.0;
        double t = phase.get();

		if (phase.get() == 0.0) targetReached = true;
        
        double newFreq = calculateDetunedFrequency(detuneAmount, frequency.get());
        frequency.set(newFreq);

        phaseIncrement = updatePhaseIncrement(frequency.get());
        
        /*
		if (targetReached)
		{
			phaseIncrement = updatePhaseIncrement(frequency.get());
		}
		else
		{
			phaseIncrement = getTargetPhaseIncrement(phaseRemaining);
		}
         */
        
        
        
		if (phaseIncrement == 0.0)
			return value;
        
        switch (mode)
        {
            case OSCILLATOR_MODE_SINE:
                value = naiveWaveFormForMode(mode, phase.get());
                break;
            
            case OSCILLATOR_MODE_SAW:
                value = naiveWaveFormForMode(mode, phase.get());
				value += poly_blep( t, phaseIncrement );
                break;
                
            case OSCILLATOR_MODE_SQUARE:
                //value = naiveWaveFormForMode(mode, phase.get());
                //value = dsp::FastMathApproximations::sinh(value * 3.0) / (3.0 * double_Pi);
				value = getAlternativeSquare(phase.get(), pulsewidth, frequency.get());
				value += poly_blep( t, phaseIncrement );
				value -= poly_blep( fmod( t + (1.0 - pulsewidth), 1.0 ), phaseIncrement );

                break;
                
            case OSCILLATOR_MODE_NOISE:
                value = naiveWaveFormForMode(mode, phase.get());
                break;
                
            default:
                break;
                
        }
        
        
        phase.set( phase.get() + phaseIncrement );
        
        rephase = false;
        
        if(phase.get() >= 1.0)
        {
            phase.set(0.0);
            rephase = true;
        }
    
       
        
        return value * level * gain.get();
    }
    
	// TEST
	bool shouldBlep = false;
    
private:
    
    double inline calculateDetunedFrequency(const double semitones, const double freq)
    {
        double power = semitones / 12.0;
        return pow(2.0, power) * freq;
    }
    
    double naiveWaveFormForMode(const OscillatorMode m, double phs)
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;
       
        switch (m)
        {
            case OSCILLATOR_MODE_SINE:
                value = sin(phs * two_Pi);
                break;
                
            case OSCILLATOR_MODE_SAW:
				// value = ( 3.0 * phs ) ;
				// value = 2.0 * dsp::FastMathApproximations::tanh(value) - 1.0;
				// value *= getTermForSaw(phs, frequency.get());
				// value *= -1;
				value = getAlternativeSaw(phs, frequency.get());
                break;
                
            case OSCILLATOR_MODE_SQUARE:
                if (phs <=  pulsewidth) { 
                    value = 1.0 - (1.0 * phs);
                } else {
                    value = (0.5 * (phs - pulsewidth) / 0.5 ) - 1.0;
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

	inline double getAlternativeSaw(double phase, double freq)
	{
		// see desmos for waveform calculation
		// https://www.desmos.com/calculator/l6cc64mqhk

		auto getNterms = [](double f) { 
			if (f > 440)
				return 2;

			if (f <= 440 && f > 220)
				return 4;

			if (f <= 220 && f > 110)
				return 6;

			return 8;
		};

		int n = 4;// getNterms(freq);

		double val = 2.0 * std::pow((phase - 1), double(n)) - 1.0;
		val *= getTermForSaw(phase, freq);

		return val;
	}
    
	inline double getTermForSaw(double phase, double freq)
	{
		// see desmos for demo
		// https://www.desmos.com/calculator/tok7j246bk

		if (freq == 0)
			freq = 1;
		// overall strength of the distortion based on frequency
		// higher means less strength
		double strength = 50 / freq;
		double strength_sq = strength * strength * strength * strength;

		double denom = (strength_sq * 100 * phase * phase * phase * phase * phase * phase) + 1;
		return 1 / denom;

	}

	inline double getAlternativeSquare(double phase, double pw, double freq)
	{
		double strength = 220 / (freq * -1.0);
		auto h = [](double x, double n)
					{
						return std::pow(x + 1.0, n);
					};

		return phase <= pw ? h(phase, strength) : h(phase - pw, strength) * -1.0;
	}
    
    double poly_blep (double t, const double phaseInc)
    {
	
        const double dt = phaseInc; // normalize phase increment
        
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
        return ( nyFreq ) / sampleRate;
    }
    
	double getTargetPhaseIncrement(double phaseLeft)
	{
		double timeSamples = sampleRate / (1000.0 / phaseInterpolationTime); 
		return phaseLeft / timeSamples;
	}
    

    double sampleRate,  phaseIncrement;
	double velocityFactor;
	Atomic<double> frequency, phase, gain;
    double detuneAmount = 0.0;
    double oldDetuneAmount = 0.0;

    double level;
	double pulsewidth;

	double phaseRemaining = 0.0;
	double phaseInterpolationTime = 5.0;
	bool targetReached = true;
    
    OscillatorMode mode;
    Random random;
    
    bool rephase;
	
    
	JUCE_LEAK_DETECTOR(Oscillator);
};

#endif /* Oscillator_h */
