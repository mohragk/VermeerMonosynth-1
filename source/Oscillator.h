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
    Oscillator() : sampleRate(44100.0), phase(0.0), level(0.75)
    {
        
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
        frequency = f;
       // phaseIncrement = updatePhaseIncrement(frequency);
    }
    
    void setPhase(const double ph)
    {
        phase = ph;
    }
    
    void setGain(const double g)
    {
        gain = g;
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
    
    bool isRephase()
    {
        return rephase;
    }

    double nextSample()
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;
        double t = phase / two_Pi; // normalize period
        
        
        
        phaseIncrement = updatePhaseIncrement(frequency);
        
        if( mode == OSCILLATOR_MODE_SINE )
        {
            value = naiveWaveFormForMode(mode);
        }
        else if (mode == OSCILLATOR_MODE_SAW)
        {
            value = naiveWaveFormForMode(mode);
            value -= poly_blep(t, phaseIncrement);
        }
        else if (mode == OSCILLATOR_MODE_SQUARE)
        {
            value = naiveWaveFormForMode(mode);
            value += poly_blep( t, phaseIncrement );
            value -= poly_blep( fmod( t + 0.5, 1.0 ), phaseIncrement );
        } else
        {
            value = naiveWaveFormForMode(mode);
        }
        
        phase += phaseIncrement;
        
        rephase = false;
        
        while (phase >= two_Pi)
        {
            phase -= two_Pi;
            rephase = true;
        }
        
        return value * level * gain;
    }
    
private:
    
    double naiveWaveFormForMode(const OscillatorMode mode)
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;;
        switch (mode)
        {
            case OSCILLATOR_MODE_SINE:
                value = sin(phase);
                break;
            case OSCILLATOR_MODE_SAW:
                value = phase / two_Pi;
                value = tanh(3.0 * value);
                value = 2.0 * value - 1.0;
                break;
                
            case OSCILLATOR_MODE_SQUARE:
                if (phase <= double_Pi) {
                    value = 1.0;
                } else {
                    value = -1.0;
                }
                break;
            case OSCILLATOR_MODE_NOISE:
                // Random r;
                // value = r.nextDouble();
                break;
            default:
                break;
        }
        return value;
    }
    
    
    double poly_blep (double t, const double phaseIncrement)
    {
        const double dt = phaseIncrement / (2.0 * double_Pi); // normalize phase increment
        
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
    

    double sampleRate, phase, phaseIncrement, frequency;
    double level, gain;
    
    OscillatorMode mode;
    
    bool rephase = false;
    
};

#endif /* Oscillator_h */
