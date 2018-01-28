/*
  ==============================================================================

    VAOnePole.h
    Created: 21 Jan 2018 12:53:25pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef VA_ONEPOLE_H
#define VA_ONEPOLE_H

#include "LadderFilterBase.h"

class VAOnePole : public LadderFilterBase
{
    public :
    
        VAOnePole() : LadderFilterBase(), sampleRate(44100.0)
        {
            Alpha = 1.0;
            Beta = 0.0;
            Z1 = 0.0;
            Gamma = 1.0;
            Delta = 0.0;
            Epsilon = 0.0;
            a0 = 1.0;
            Feedback = 0.0;
            
            // default to lpf
            type = LPF1;
            
            Reset();
            
        }
        
        virtual ~VAOnePole()
        {}
    
        
        virtual void Update() override
        {
            double wd = 2 * MOOG_PI * cutoff;
            double T = 1 / sampleRate;
            double wa = ( 2 / T ) * tan( wd * T / 2 );
            double g = wa * T / 2;
            
            Alpha = g / ( 1.0 + g );
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
        FloatType doFilter( FloatType sample )
        {
            
            sample = sample * Gamma + Feedback + Epsilon * getFeedbackOutput();
            
            double vn = ( a0 * sample - Z1 ) * Alpha;
            
            //LPF
            double lpf = vn + Z1;
            
            //Update memory
            Z1 = vn + lpf;
            
            double hpf = sample - lpf;
            
            if (type == LPF1)
                return lpf;
            else
                return hpf;
        }
        
        virtual void Reset() override
        {
            Z1 = 0.0; Feedback = 0.0;
        }
        
        void setFeedback( double fb )
        {
            Feedback = fb;
        }
        
        double getFeedbackOutput() 
        { 
            return Beta * ( Z1 + Feedback * Delta );
        }
    
        void setFilterType ( FilterType t )
        {
            type = t;
        }
    
        FilterType getFilterType()
        {
            return type;
        }
        
        
        virtual void SetSampleRate (double sr) override
		{
			sampleRate = sr;                
		}
	
		virtual void SetResonance(double r) override
		{
            resonance = r * 10.0;;
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
    
    double Alpha;
    double Beta;
    double Gamma;
    double Delta;
    double Epsilon;
    
    double a0;
    double Z1;
    
    double Feedback;
    
        
    private :
    
        FilterType type;
        double sampleRate;
};



#endif //VA_ONEPOLE_H
