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
        K = 0.01;
        Alpha0 = 0;
        
        // set filter types
        va_LPF1.setFilterType(LPF1);
        va_LPF2.setFilterType(LPF1);
        va_HPF1.setFilterType(HPF1);
        va_HPF2.setFilterType(HPF1);
        
        
        type = LPF2;
        
        reset();
        
    }
    
    virtual ~ThreeFiveModel() {}
    
    
    void reset()
    {
        va_LPF1.reset();
        va_LPF2.reset();
        va_HPF1.reset();
        va_HPF2.reset();
    }
    
    
    void update()
    {
        //prewarp for BZT
        double wd = 2 * MOOG_PI * cutoff;
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
        Alpha0 = 1.0 / ( 1.0 - K * G + K * G * G );
        
        if (type = LPF2)
        {
            va_LPF2.Beta = ( K - K * G ) / ( 1.0 + g );
            va_HPF1.Beta = -1.0 / ( 1.0 + g);
        }
        else
        {
            va_HPF2.Beta = -1.0 * G / (1.0 + g);
            va_LPF1.Beta = 1.0 / (1.0 + g);
        }
    }
    
    
    virtual void Process(float* samples, uint32_t n) noexcept override
    {
        for (int i = 0; i < n; i++)
        {
            samples[i] = doFilter(samples[i]);
        }
    }
    
    double doFilter( double sample )
    {
        
        
        double y = 0.0;
        
        if (type == LPF2)
        {
            double y1 = va_LPF1.doFilter(sample);
            
            double S35 =    va_HPF1.getFeedbackOutput() +
                            va_LPF2.getFeedbackOutput();
            
            double u = Alpha0 *  y1 + S35 ;
            
            u = fast_tanh(drive * u);
            
            y = K * va_LPF2.doFilter(u);
            
            va_HPF1.doFilter(y);
        }
        else
        {
            double y1 = va_HPF1.doFilter(sample);
            
            double S35 =    va_HPF2.getFeedbackOutput() +
                            va_LPF1.getFeedbackOutput();
            
            double u = Alpha0 * y1 + S35;
            
            y = K * u;
            
            y = fast_tanh(drive * y);
            
            va_LPF1.doFilter(va_HPF2.doFilter(y));
        }
        
        if (K > 0)
            y *= 1 / K;
        
        return y;
        
    }
    
    virtual void SetSampleRate (float sr) override
    {
        sampleRate = sr;
    }
    
    virtual void SetResonance(float r) override
    {
        K = (2.0 - 0.01) * (r - 0.0) / (1.0 - 0.0) + 0.01; // remap
		update();
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
    
    VAOnePole va_LPF1, va_LPF2, va_HPF1, va_HPF2;
    
    FilterType type;
    
    double K;
    double Alpha0;
    
    
};

#endif // THREE_FIVE_MODEL_H

