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
    
		SEMModel() : LadderFilterBase(), sampleRate(44100.0), Alpha0(1.0), Alpha(1.0), Rho(1.0)
		{
			SetCutoff(1000.0);
			SetResonance(0.0);

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
			double wd = 2 * MOOG_PI * cutoff.get();
			double T  = 1 / sampleRate;
			double wa = ( 2 / T ) * tan( wd * T / 2 );
			double g  = wa * T / 2;

			// R is damping factor
			double R  = 1.0 / ( 2.0 * resonance.get() );
        
        
			// set Coeffs
			Alpha0 = 1.0 / (1.0 + 2.0 * R * g + g * g);
			Alpha = g;
			Rho = 2.0 * R + g;
        
		}
    
		virtual void Process(float* samples, size_t n) noexcept override
		{
			 for (uint32_t i = 0; i < n; i++)
			{
				 doFilter(samples[i], i);
			}
		}

		virtual void Process(double* samples, size_t n) noexcept override
		{
			for (uint32_t i = 0; i < n; i++)
			{
				doFilter(samples[i], i);
			}
		}

		template <typename FloatType>
		FloatType doFilter(FloatType sample, int pos)
		{
            UpdateParameters(pos);

			// form the HPF output first
			FloatType hpf = Alpha0 * (sample - Rho * Z11 - Z12);

			// BPF out
			FloatType bpf = Alpha * hpf + Z11;

			// non-linear
			//if (NLP)
			//    bpf = tanh( Saturation * bpf );

			// LPF
			FloatType lpf = Alpha * bpf + Z12;

			//FloatType R = 1.0 / (2.0 * resonance.get());

			//FloatType bsf = sample - 2.0 * R * bpf;

			// update memory
			Z11 = Alpha * hpf + bpf;
			Z12 = Alpha * bpf + lpf;

			//lpf *= 2.0 / resonance;

			lpf = saturationLUT(drive *lpf);

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
			jassert(!isnan(sr));

			sampleRate = sr;                
		}
	
		virtual void SetResonance(double r) override
		{
			if (isnan(r))
				r = 0.0;

			//remap: 0 -> 1 --- 0.5 -> 25
			resonance.set( (25.0 - 0.5) * (r - 0.0) / (1.0 - 0.0) + 0.5 );
			
		}
    
		virtual bool SetCutoff(double c) override
		{

			cutoff.set(c);
            Update();
            
            return true;
		}
    
		virtual void SetDrive (double d ) override
		{
			drive = d;
		}
	
		
    
    
    private :
    
		double sampleRate;
    
		double Alpha0, Alpha;
		double Rho;
		double Z11, Z12;
};

#endif // SEM_MODEL_H
