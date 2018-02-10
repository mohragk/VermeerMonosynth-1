/*
  ==============================================================================

    DiodeLadderModel.h
    Created: 21 Jan 2018 5:22:41pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef DIODE_LADDER_MODEL_H
#define DIODE_LADDER_MODEL_H

#include "LadderFilterBase.h"


class DiodeLadderModel : public LadderFilterBase
{
    public:
    
    DiodeLadderModel() : LadderFilterBase(), sampleRate(44100.0)
    {
        resonance.set(0.0);
		SetCutoff(1000.0);
        
        Gamma = 0.0;
        
        SG1 = 0.0; SG2 = 0.0; SG3 = 0.0; SG4 = 0.0;
        
        va_LPF1.setFilterType(LPF1);
        va_LPF2.setFilterType(LPF1);
        va_LPF3.setFilterType(LPF1);
        va_LPF4.setFilterType(LPF1);
        
        type = LPF4;
        
        Reset();
        
        
    }
    
    
    virtual ~DiodeLadderModel() {}
    
    virtual void Reset() override
    {
        va_LPF1.Reset();
        va_LPF2.Reset();
        va_LPF3.Reset();
        va_LPF4.Reset();
    }
    
    virtual void Update() override
    {
        double wd = 2 * MOOG_PI * cutoff.get();
        double T  = 1 / sampleRate;
        double wa = ( 2 / T ) * tan( wd * T / 2 );
        double g = wa * T / 2;
        
        double G4 = 0.5 * g / ( 1.0 + g );
        double G3 = 0.5 * g / ( 1.0 + g - 0.5 * g * G4 );
        double G2 = 0.5 * g / ( 1.0 + g - 0.5 * g * G3 );
        double G1 = g / ( 1.0 + g - g * G2 );
        Gamma     = G4 * G3 * G2 * G1;
        
        SG1 = G4 * G3 * G2;
        SG2 = G4 * G3;
        SG3 = G4;
        SG4 = 1.0;
        
        double G = g / (1.0 + g);
        
        //Alphas
        va_LPF1.Alpha = G;
        va_LPF2.Alpha = G;
        va_LPF3.Alpha = G;
        va_LPF4.Alpha = G;
        
        //Betas
        va_LPF1.Beta = 1.0 / ( 1.0 + g - g * G2 );
        va_LPF2.Beta = 1.0 / ( 1.0 + g - 0.5 * g * G3 ) ;
        va_LPF3.Beta = 1.0 / ( 1.0 + g - 0.5 * g * G4 ) ;
        va_LPF4.Beta = 1.0 / ( 1.0 + g );
        
        //Deltas
        va_LPF1.Delta = g;
        va_LPF2.Delta = 0.5 * g;
        va_LPF3.Delta = 0.5 * g;
        va_LPF4.Delta = 0.0;
        
        //Gammas
        va_LPF1.Gamma = 1.0 + G1 * G2;
        va_LPF2.Gamma = 1.0 + G2 * G3;
        va_LPF3.Gamma = 1.0 + G3 * G4;
        va_LPF2.Gamma = 1.0;
        
        //Epsilons
        va_LPF1.Epsilon = G2;
        va_LPF2.Epsilon = G3;
        va_LPF3.Epsilon = G4;
        va_LPF4.Epsilon = 0.0;
        
        // a0s
        va_LPF1.a0 = 1.0;
        va_LPF2.a0 = 0.5;
        va_LPF3.a0 = 0.5;
        va_LPF4.a0 = 0.5;
        
    }
    
	template <typename FloatType>
    FloatType doFilter( FloatType sample )
    {
		if (sampleRate <= 0.0)
			return sample;
	
        va_LPF4.setFeedback( 0.0 );
        va_LPF3.setFeedback( va_LPF4.getFeedbackOutput() );
        va_LPF2.setFeedback( va_LPF3.getFeedbackOutput() );
        va_LPF1.setFeedback( va_LPF2.getFeedbackOutput() );
        
		FloatType Sigma =	SG1 * va_LPF1.getFeedbackOutput() +
							SG2 * va_LPF2.getFeedbackOutput() +
							SG3 * va_LPF3.getFeedbackOutput() +
							SG4 * va_LPF4.getFeedbackOutput();
        
		FloatType U = ( sample - resonance.get() * Sigma ) / ( 1 + resonance.get() * Gamma );
        
        U = fast_tanh(drive * U);
        
        return 2.0 * va_LPF4.doFilter( va_LPF3.doFilter( va_LPF2.doFilter( va_LPF1.doFilter( U ) ) ) ) ;
    }
    
    virtual void Process(float* samples, size_t n) noexcept override
    {
        for (uint32_t i = 0; i < n; i++)
        {
            samples[i] = doFilter(samples[i]);
        }
    }

	virtual void Process(double* samples, size_t n) noexcept override
	{
		for (uint32_t i = 0; i < n; i++)
		{
			samples[i] = doFilter(samples[i]);
		}
	}
    
    virtual void ProcessRamp(float* samples, size_t n, float beginCutoff, float endCutoff) override
    {
        const auto increment = (endCutoff - beginCutoff) / (float) n;
        
        for (uint32_t i = 0; i < n; i++)
        {
            SetCutoff(beginCutoff);
            samples[i] = doFilter(samples[i]);
            beginCutoff += increment;
        }
    }
    
    virtual void ProcessRamp(double* samples, size_t n, double beginCutoff, double endCutoff) override
    {
        const auto increment = (endCutoff - beginCutoff) / (double) n;
        
        for (uint32_t i = 0; i < n; i++)
        {
            SetCutoff(beginCutoff);
            samples[i] = doFilter(samples[i]);
            beginCutoff += increment;
        }
    }
    
    virtual void SetSampleRate (double sr) override
    {
		jassert(!isnan(sr));

        sampleRate = sr;

		va_LPF1.SetSampleRate(sr);
		va_LPF2.SetSampleRate(sr);
		va_LPF3.SetSampleRate(sr);
		va_LPF4.SetSampleRate(sr);
    }
    
    virtual void SetResonance(double r) override
    {
		if (isnan(r))
			r = 0.0;

		jassert(r >= 0 && r <= 1.0);

        resonance.set( 17.0 * r); // remap
    }
    
    virtual bool SetCutoff(double c) override
    {
		if (isnan(c))
			return false;

        jassert (c > 0 && c <= (sampleRate * 0.5));
        
        cutoff.set(c);
        Update();
        
        return true;
    }
    
    virtual void SetDrive (double d ) override
    {
        drive = d;
    }
    

    

    
    private:
    
    
    double sampleRate;
        
    double Gamma;
    
    FilterType type;
    
    VAOnePole va_LPF1, va_LPF2, va_LPF3, va_LPF4;
    
    double SG1, SG2, SG3, SG4;
};


#endif // DIODE_LADDER_MODEL_H
