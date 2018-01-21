/*
  ==============================================================================

    SEMModel.h
    Created: 20 Jan 2018 4:57:55pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef SEM_MODEL_H
#define SEM_MODEL_H

#include "LadderFilterBase.h"

class SEMModel : public LadderFilterBase
{
    public :
    
		SEMModel() : LadderFilterBase(), sampleRate(44100.0)
		{
			Alpha0 = 1.0;
			Alpha = 1.0;
			Rho = 1.0;
        
		   // drive = 1.0f;
        
			SetCutoff(1000.0f); // normalized cutoff frequency
			SetResonance(0.1f); // [0, 4]
			reset();
		
		}
    
		virtual ~SEMModel()
		{

		}
    
		void reset()
		{
			Z11 = 0.0; Z12 = 0.0;
		}
    
    
		void update()
		{
        
			// prewarp cutoff, billinear transform
			double wd = 2 * MOOG_PI * cutoff;
			double T  = 1 / sampleRate;
			double wa = ( 2 / T ) * tan( wd * T / 2 );
			double g  = wa * T / 2;

			// R is damping factor
			double R  = 1.0 / ( 2.0 * resonance );
        
        
			// set Coeffs
			Alpha0 = 1.0 / (1.0 + 2.0 * R * g + g * g);
			Alpha = g;
			Rho = 2.0 * R + g;
        
		}
    
		virtual void Process(float* samples, uint32_t n) noexcept override
		{
		
			 for (int i = 0; i < n; i++)
			{
				update();

				// form the HPF output first
				double hpf = Alpha0 * ( samples[i] - Rho * Z11 - Z12 );
            
				// BPF out
				double bpf = Alpha * hpf + Z11;
            
				// non-linear
				//if (NLP)
				//    bpf = tanh( Saturation * bpf );
            
				// LPF
				double lpf = Alpha * bpf + Z12;
            
				double R = 1.0 / ( 2.0 * resonance );
            
				double bsf = samples[i] - 2.0 * R * bpf;
            
				// update memory
				Z11 = Alpha * hpf + bpf;
				Z12 = Alpha * bpf + lpf;
            
            
				samples[i] = lpf;
        
			}
        
		}
        
		virtual void SetSampleRate (float sr) override
		{
			sampleRate = sr;                
		}
	
		virtual void SetResonance(float r) override
		{
			resonance = (25.0 - 0.5) * (r - 0.0) / (1.0 - 0.0) + 0.5;
		}
    
		virtual void SetCutoff(float c) override
		{
			cutoff = c;
		}
    
		virtual void SetDrive ( float d ) override
		{
			drive = d;
		}
	
		double GetSampleRate() override
		{
			return sampleRate;
		}
    
		double GetCutoff() override
		{
			return cutoff;
		}
    
    
    private :
    
		double sampleRate;
    
		double Alpha, Alpha0;
		double Rho;
		double Z11, Z12;
};

#endif // SEM_MODEL_H