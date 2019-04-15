/*
  ==============================================================================

    VAOnePole.h
    Created: 21 Jan 2018 12:53:25pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef VA_ONEPOLE_H
#define VA_ONEPOLE_H

#include "util.h"

class VAOnePole 
{
    public :
		enum FilterType { LPF1, HPF1, LPF2, HPF2, BPF2, BSF2, LPF4, HPF4, BPF4 } type;
    
        VAOnePole() :
    
                        Alpha(1.0),
                        Beta(0.0),
                        Gamma(1.0),
                        Delta(0.0),
                        Epsilon(0.0),
                        a0(1.0),
                        Z1(0.0),
                        Feedback(0.0),
                        type(LPF1),
                        sampleRate(44100.0)
						
        {
            Reset();
        }
        
        virtual ~VAOnePole()
        {}
    

		void Prepare(double newSampleRate, int newSamplesPerBlock)
		{
			sampleRate = newSampleRate;
			numSamples = newSamplesPerBlock;
		}
        
        void Update() 
        {
            double wd = 2 * MOOG_PI * cutoff.get();
            double T = 1 / sampleRate;
            double wa = ( 2 / T ) * tan( wd * T / 2 );
            double g = wa * T / 2;
            
            Alpha = g / ( 1.0 + g );
        }
        
        void Process(float* samples, size_t n) 
		{
            for (uint32_t i = 0; i < n; i++)
			{
                samples[i] = doFilter(samples[i], i);
			}
		}

		 void Process(double* samples, size_t n)
		{
			for (uint32_t i = 0; i < n; i++)
			{
				samples[i] = doFilter(samples[i], i);
			}
		}
    
       void ProcessRamp(float* samples, size_t n, float beginCutoff, float endCutoff) 
        {
			const auto increment = (endCutoff - beginCutoff) / static_cast<float> (n);
            
            for (uint32_t i = 0; i < n; i++)
            {
                SetCutoff(beginCutoff);
                samples[i] = doFilter(samples[i], i);
                beginCutoff += increment;
            }
        }
    
        void ProcessRamp(double* samples, size_t n, double beginCutoff, double endCutoff) 
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
        
        void Reset()
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
        
        
        void SetSampleRate (double sr)
		{
			sampleRate = sr;                
		}
	
		void SetResonance(double r)
		{
            resonance.set( r * 10.0);
		}
    
		bool SetCutoff(double c)
		{
            
			cutoff.set(c);
           
            return true;
		}
    
		void SetDrive (double d )
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
    
		Atomic<double> cutoff, resonance, drive;
		double sampleRate;
		int numSamples;
    
};



#endif //VA_ONEPOLE_H
