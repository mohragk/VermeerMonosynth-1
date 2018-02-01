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
    ThreeFiveModel() : LadderFilterBase(), sampleRate(44100.0)
    {
        //init
        resonance.set( 0.01 );
        Alpha0 = 0;
        
        // set filter types
        va_LPF1.setFilterType(LPF1);
        va_LPF2.setFilterType(LPF1);
        va_HPF1.setFilterType(HPF1);
        va_HPF2.setFilterType(HPF1);
        
        
        type = LPF2;
        
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
    
	template <typename FloatType>
	FloatType doFilter(FloatType sample )
    {
               
		FloatType y = 0.0;
        
        if (type == LPF2)
        {
			FloatType y1 = va_LPF1.doFilter(sample);
            
			FloatType S35 =    va_HPF1.getFeedbackOutput() +
                            va_LPF2.getFeedbackOutput();
            
			FloatType u = Alpha0 *  y1 + S35 ;
            
            u = fast_tanh(drive * u);
            
            y = resonance.get() * va_LPF2.doFilter(u);
            
            va_HPF1.doFilter(y);
        }
        else
        {
			FloatType y1 = va_HPF1.doFilter(sample);
            
			FloatType S35 =    va_HPF2.getFeedbackOutput() +
                            va_LPF1.getFeedbackOutput();
            
			FloatType u = Alpha0 * y1 + S35;
            
            y = resonance.get() * u;
            
            y = fast_tanh(drive * y);
            
            va_LPF1.doFilter(va_HPF2.doFilter(y));
        }
        
        if (resonance.get() > 0)
            y *= 1 / resonance.get();
        
        return y;
        
    }
    
    virtual void SetSampleRate (double sr) override
    {
        sampleRate = sr;

		va_LPF1.SetSampleRate(sr);
		va_LPF2.SetSampleRate(sr);
		va_HPF1.SetSampleRate(sr);
		va_HPF2.SetSampleRate(sr);
    }
    
    virtual void SetResonance(double r) override
    {
        resonance.set( (2.0 - 0.01) * (r - 0.0) / (1.0 - 0.0) + 0.01 ); // remap
    }
    
    virtual void SetCutoff(double c) override
    {
        cutoff.set(c);
        Update();
    }
    
    virtual void SetDrive (double d ) override
    {
        drive = d;
    }
  
    
    
    private :
    
    double sampleRate;
    
    VAOnePole va_LPF1, va_LPF2, va_HPF1, va_HPF2;
    
    FilterType type;
    
    double Alpha0;
    
    
};

#endif // THREE_FIVE_MODEL_H

