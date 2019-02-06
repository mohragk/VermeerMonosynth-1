/*
 ==============================================================================
 
 ThreeFiveModel.h
 Created: 21 Jan 2018 1:38:47pm
 Author:  Sander Vermeer
 
 ==============================================================================
 */

#ifndef THREE_FIVE_MODEL_H
#define THREE_FIVE_MODEL_H

#include "LadderFilterBase.h"
#include "VAOnePole.h"

class ThreeFiveModel : public LadderFilterBase
{
public:
    ThreeFiveModel() : LadderFilterBase(), sampleRate(44100.0), type(LPF2), Alpha0(0.0), oldCutoff(1000.0)
    {
		SetCutoff(1000.0);
        SetResonance( 0.0 );
        
        // set filter types
        va_LPF1.setFilterType(LPF1);
        va_LPF2.setFilterType(LPF1);
        va_HPF1.setFilterType(HPF1);
        va_HPF2.setFilterType(HPF1);
        
        
        Reset();
        
    }
    
    virtual ~ThreeFiveModel() {}
    
    
    virtual void Reset() override
    {
        va_LPF1.Reset();
        va_LPF2.Reset();
        va_HPF1.Reset();
        va_HPF2.Reset();
    }
    
    
    virtual void Update() override
    {
        //prewarp for BZT
        double wd = 2 * MOOG_PI * cutoff.get();
        double T  = 1 / sampleRate;
        double wa = ( 2 / T ) * tan( wd * T / 2 );
        double g  = wa * T / 2;
        
        
        // feedforward coeff, same for LPF as HPF
        double G = g / (1.0 + g);
        
        va_LPF1.Alpha = G;
        va_LPF2.Alpha = G;
        va_HPF1.Alpha = G;
        va_HPF2.Alpha = G;
        
        // set Alpha0; same for LPF as HPF
        Alpha0 = 1.0 / ( 1.0 - resonance.get() * G + resonance.get() * G * G );
        
        if (type == LPF2)
        {
            va_LPF2.Beta = (resonance.get() - resonance.get() * G ) / ( 1.0 + g );
            va_HPF1.Beta = -1.0 / ( 1.0 + g);
        }
        else
        {
            va_HPF2.Beta = -1.0 * G / (1.0 + g);
            va_LPF1.Beta = 1.0 / (1.0 + g);
        }
    }
    
    
    virtual void Process(float* samples, size_t n) noexcept override
    {
        for (uint32_t i = 0; i < n; i++)
        {
            samples[i] = doFilter(samples[i], i);
        }
        
    }
	virtual void Process(double* samples, size_t n) noexcept override
	{
		for (uint32_t i = 0; i < n; i++)
		{
			samples[i] = doFilter(samples[i], i);
		}
        
	}
    
    virtual void ProcessRamp(float* samples, size_t n, float beginCutoff, float endCutoff) override
    {
        const auto increment = (endCutoff - beginCutoff) / static_cast<float> ( n );
        
        for (uint32_t i = 0; i < n; i++)
        {
            SetCutoff(beginCutoff);
            samples[i] = doFilter(samples[i], i);
            beginCutoff += increment;
        }
        
    }
    
    
    virtual void ProcessRamp(double* samples, size_t n, double beginCutoff, double endCutoff) override
    {
		const auto increment = (endCutoff - beginCutoff) / static_cast<double> (n);
        
        for (uint32_t i = 0; i < n; i++)
        {
            SetCutoff(beginCutoff);
            samples[i] = doFilter(samples[i], i);
            beginCutoff += increment;
        }
        
    }
    
    
	template <typename FloatType>
	inline FloatType doFilter(FloatType sample, int pos )
    {
        UpdateParameters(pos);

		FloatType y = 0.0;
		
		if (type == LPF2)
		{
			FloatType y1 = va_LPF1.doFilter(sample, pos);

			FloatType S35 = va_HPF1.getFeedbackOutput() +
				va_LPF2.getFeedbackOutput();

			FloatType u = Alpha0 * y1 + S35;

			u = saturationLUT(drive * u);

			y = resonance.get() * va_LPF2.doFilter(u, pos);

			va_HPF1.doFilter(y, pos);
		}
		else
		{
			FloatType y1 = va_HPF1.doFilter(sample, pos);

			FloatType S35 = va_HPF2.getFeedbackOutput() +
				va_LPF1.getFeedbackOutput();

			FloatType u = Alpha0 * y1 + S35;

			y = resonance.get() * u;

			y = saturationLUT(drive * y);

			va_LPF1.doFilter(va_HPF2.doFilter(y, pos), pos);
		}

		if (resonance.get() > 0)
			y *= 1 / resonance.get();
				
		return y;
    }
    
    virtual void SetSampleRate (double sr) override
    {
		jassert(!isnan(sr));

        sampleRate = sr;

		va_LPF1.SetSampleRate(sr);
		va_LPF2.SetSampleRate(sr);
		va_HPF1.SetSampleRate(sr);
		va_HPF2.SetSampleRate(sr);
    }
    
    virtual void SetResonance(double r) override
    {
		if (isnan(r))
			r = 0.0;

		jassert(r >= 0 && r <= 1.0);

		double remapped = map(r, 0.0, 1.0, 0.01, 2.0);

        resonance.set( remapped ); // remap
    }
    
    virtual bool SetCutoff(double c) override
    {

        if (isnan(c))
            c = 1000.0;
        
		c = clamp(c, 40.0, 20000.0);

        
        cutoff.set(c);

		if (oldCutoff != c)
		{
			Update();
		}

		oldCutoff = c;
        
        return true;
    }
    
    virtual void SetDrive (double d ) override
    {
        drive = map(d, 1.0, 5.0, 1.0, 2.0);
    }
  
    
    
    private :
    
    double sampleRate;
    
    VAOnePole va_LPF1, va_LPF2, va_HPF1, va_HPF2;
    
    FilterType type;
    
    double Alpha0;
    
	double oldCutoff;
    
};

#endif // THREE_FIVE_MODEL_H

