/*
  ==============================================================================

    lfo.h
    Created: 3 Aug 2017 6:13:43pm
    Author:  VLSDESK003

  ==============================================================================
*/

#pragma once

class LFO
{
    public:
        LFO() : phase(0.0)
        {
           
        }
    
        
         enum OscillatorMode {
                OSCILLATOR_MODE_SINE = 0,
                OSCILLATOR_MODE_SAW,
                OSCILLATOR_MODE_SQUARE,
                OSCILLATOR_MODE_NOISE, 
                numOscillatorModes
            };
        
        void setFrequency(double f)
        {
            frequency = f;
        }
        
        void setPhase(double ph)
        {
            phase = ph;
        }
        
        void setMode(int m)
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
            
            phaseIncrement = updatePhaseIncrement(frequency);
            
            const double two_Pi = 2.0 * double_Pi;
            double value = 0.0;
            
            switch (mode) {
                case OSCILLATOR_MODE_SINE:
                    value = sin(phase);
                    break;
                case OSCILLATOR_MODE_SAW:
                    value = (2.0 * phase / two_Pi) - 1.0;
                    break;
                case OSCILLATOR_MODE_SQUARE:
                    if (phase <= double_Pi) {
                        value = 1.0;
                    } else {
                        value = -1.0;
                    }
                    break;
                case OSCILLATOR_MODE_NOISE:   
                    Random r;
                    value = r.nextDouble();
                    break;
                default:
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
    
     
        double updatePhaseIncrement(double freq)
        {
            double nyFreq = fmin( freq, getSampleRate() / 2.0 );
            return ( ( 2.0 * double_Pi ) * nyFreq ) / getSampleRate();
        }
    
        
        double phase, phaseInc, frequency;
        
        OscillatorMode mode;
       
    
    
}
