/*
  ==============================================================================

    ParamSmoother.h
    Created: 27 Feb 2018 10:46:57pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef PARAM_SMOOTHER_H
#define PARAM_SMOOTHER_H

class ParamSmoother {
	public:
		ParamSmoother()
		{
			reset();
		}

		~ParamSmoother()
		{}

		void init(double sr, double millis) 
		{
			a = exp(-double_Pi / (millis * 0.001 * sr));
			b = 1.0 - a;
			z = 0.0;
		}

		void reset()
		{
			a = 0.0;
			b = 0.0;
			z = 0.0;
		}

		inline double processSmooth(double val)
		{
			z = (val * b) + (z * a);
			return z;
		}

        
        
	private:
		double a, b, z;
		double coeff;
	};
    
#endif // PARAM_SMOOTHER_H