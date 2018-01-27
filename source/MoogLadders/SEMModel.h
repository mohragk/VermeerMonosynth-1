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
    
		SEMModel() : LadderFilterBase(), sampleRate(48000.0)
		{
			Alpha0 = 1.0;
			Alpha = 1.0;
			Rho = 1.0;
        
			Reset();
		
		}
    
		virtual ~SEMModel()
		{

		}
    
		virtual void Reset() override
		{
			Z11 = 0.0; Z12 = 0.0;
		}
    
    
		virtual void Update() override
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
    
		virtual void Process(float* samples, size_t n) noexcept override
		{
			 for (uint32_t i = 0; i < n; i++)
			{
				 doFilter(samples[i]);
			}
		}

		virtual void Process(double* samples, size_t n) noexcept override
		{
			for (uint32_t i = 0; i < n; i++)
			{
				doFilter(samples[i]);
			}
		}

		template <typename FloatType>
		FloatType doFilter(FloatType sample)
		{
			
            if (cutoff <= 0.0 || cutoff > 220000.0)
                return sample;
            
			// form the HPF output first
			FloatType hpf = Alpha0 * (sample - Rho * Z11 - Z12);

			// BPF out
			FloatType bpf = Alpha * hpf + Z11;

			// non-linear
			//if (NLP)
			//    bpf = tanh( Saturation * bpf );

			// LPF
			FloatType lpf = Alpha * bpf + Z12;

			FloatType R = 1.0 / (2.0 * resonance);

			FloatType bsf = sample - 2.0 * R * bpf;

			// update memory
			Z11 = Alpha * hpf + bpf;
			Z12 = Alpha * bpf + lpf;

			//lpf *= 2.0 / resonance;

			lpf = fast_tanh(drive *lpf);

			softClip(lpf);

			return lpf;
		}

		double softClip(double s)
		{
			double localSample = s;
			if (localSample > 1.0f)
			{
				localSample = 0.75f;
			}
			else if (localSample < -1.0f)
			{
				localSample = -0.75f;

			}
			else
			{
				localSample = localSample - ((localSample * localSample * localSample) * 0.25f);
			}
			return localSample;
		}
        
		virtual void SetSampleRate (double sr) override
		{
			sampleRate = sr;                
		}
	
		virtual void SetResonance(double r) override
		{
			//remap: 0 -> 1 --- 0.5 -> 25
			resonance = (25.0 - 0.5) * (r - 0.0) / (1.0 - 0.0) + 0.5;
			
		}
    
		virtual void SetCutoff(double c) override
		{
			cutoff = c;
            Update();

		}
    
		virtual void SetDrive (double d ) override
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
