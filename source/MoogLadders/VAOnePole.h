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
    
        VAOnePole() : LadderFilterBase(),
    
                        Alpha(1.0),
                        Beta(0.0),
                        Gamma(1.0),
                        Delta(0.0),
                        Epsilon(0.0),
                        a0(1.0),
                        Z1(0.0),
                        Feedback(0.0),
                        type(LPF1),
                        sampleRate(44100.0),
						oldCutoff(1000.0)
        {
            Reset();
        }
        
        virtual ~VAOnePole()
        {}
    
        
        virtual void Update() override
        {
            double wd = 2 * MOOG_PI * cutoff.get();
            double T = 1 / sampleRate;
            double wa = ( 2 / T ) * tan( wd * T / 2 );
            double g = wa * T / 2;
            
            Alpha = g / ( 1.0 + g );
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
            
            auto linToLogLambda = [](float start, float end, float linVal) {
                float result = std::pow(10.0f, (std::log10(end / start) * linVal + std::log10(start)));
                if (result > end)
                    result = end;
                if (result < start)
                    result = start;
                return result;
            };
            auto logToLinLambda = [](float start, float end, float logVal) {
                float result =  (std::log10(logVal / start) / std::log10(end / start));
                if (result > 1.0)
                    result = 1.0;
                if (result < 0.0)
                    result = 0.0;
                return result;
            };
            
            float beginCutoffLinear = logToLinLambda(40.0, 20000.0, beginCutoff);
            float endCutoffLinear = logToLinLambda(40.0, 20000.0, endCutoff);
            float currentCutoffLinear = beginCutoffLinear;
            const auto increment = (endCutoffLinear - beginCutoffLinear) / static_cast<float> (n);
            
            
            for (uint32_t i = 0; i < n; i++)
            {
                float currentCutoff = linToLogLambda(40.0, 20000.0, currentCutoffLinear);
                SetCutoff(currentCutoff);
                samples[i] = doFilter(samples[i], i);
                currentCutoffLinear += increment;
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
        inline FloatType doFilter( FloatType sample, int pos )
        {
            //UpdateParameters(pos);
            
            sample = sample * Gamma + Feedback + Epsilon * getFeedbackOutput();
            
			FloatType vn = ( a0 * sample - Z1 ) * Alpha;
            
            //LPF
			FloatType lpf = vn + Z1;
            
            //Update memory
            Z1 = vn + lpf;
            
			FloatType hpf = sample - lpf;
            
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
            resonance.set( r * 10.0);
		}
    
		virtual bool SetCutoff(double c) override
		{
            
			cutoff.set(c);
            
			if ( !approximatelyEqual(oldCutoff, c) )
			{
				Update();
			}

			oldCutoff = c;
            return true;
		}
    
		virtual void SetDrive (double d ) override
		{
			drive = d;
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
		double oldCutoff;
};



#endif //VA_ONEPOLE_H
