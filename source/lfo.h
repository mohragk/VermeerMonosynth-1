/*
  ==============================================================================

    lfo.h
    Created: 3 Aug 2017 6:13:43pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef LFO_H
#define LFO_H

class LFO
{
    public:
        LFO() : sampleRate(44100.0), phase(0.0)
        {
           
        }
    
        
         enum OscillatorMode {
                OSCILLATOR_MODE_SINE = 0,
                OSCILLATOR_MODE_SAW,
                OSCILLATOR_MODE_SQUARE,
                OSCILLATOR_MODE_NOISE
         };
        
        void setSampleRate(const double sr)
        {
            sampleRate = sr;
        }
            
        void setFrequency(const double f)
        {
            frequency.set(f);
            
			
        }
        
        void setPhase(const double ph)
        {
            phase = ph;
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
            
        double nextSample()
        {
            const double two_Pi = 2.0 * double_Pi;
            double value = 0.0;
            
            phaseIncrement = getPhaseIncrement(frequency.get(), sampleRate);
            
            switch (mode) 
            {
                case OSCILLATOR_MODE_SINE:
                    value = sin(phase);
                    break;
                case OSCILLATOR_MODE_SAW:
                    value = 1.0 - (phase / two_Pi);
                    break;
                case OSCILLATOR_MODE_SQUARE:
                    if (phase <= double_Pi) {
                        value = 1.0;
                    } else {
                        value = -1.0;
                    }
                    break;
                case OSCILLATOR_MODE_NOISE:   
                    value = r.nextDouble();
                    break;

            }
            
            
            phase += phaseIncrement;
            
            while (phase >= 2.0 * double_Pi)
            {
                phase -= 2.0 * double_Pi;
            }
            
            
            return value;
        }
        
    private:
    
     
        double getPhaseIncrement(double freq, double sr)
        {
            const double nyFreq = jmin( freq, sr / 2.0 );
            return ( ( 2.0 * double_Pi ) * nyFreq ) / sr;
            
        }
    
        
        double sampleRate, phase, phaseIncrement;
        Atomic<double> frequency;
        OscillatorMode mode;

        Random r;
    
		JUCE_LEAK_DETECTOR(LFO);
};

#endif // LFO_H
