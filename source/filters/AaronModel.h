// Copyright (c) 2012, Aaron Krajeski All rights reserved.

#pragma once

#ifndef AARON_LADDER_H
#define AARON_LADDER_H

#include "LadderFilterBase.h"
#include "util.h"

/*
This class implements Tim Stilson's MoogVCF filter
using 'compromise' poles at z = -0.3

Several improments are built in, such as corrections
for cutoff and resonance parameters, removal of the
necessity of the separation table, audio rate update
of cutoff and resonance and a smoothly saturating
tanh() function, clamping output and creating inherent
nonlinearities.
 
*/

class AaronMoog : public LadderFilterBase
{	
public:
	
	AaronMoog()
        : LadderFilterBase()
	{
		zeromem(state, sizeof(state));
		zeromem(delay, sizeof(delay));
		
		drive = 1.0;
		gComp = 1.0;
		
		SetCutoff(1000.0f);
		SetResonance(0.1f);
	}
	
	~AaronMoog()
	{
		
	}
	
	void Process(float * samples, uint32_t n)
    {
		for (int s = 0; s < n; ++s)
		{
			state[0] = tanh(drive * (samples[s] - 4 * gRes * (state[4] - gComp * samples[s])));
			
			for(int i = 0; i < 4; i++)
			{
				state[i+1] = g * (0.3 / 1.3 * state[i] + 1 / 1.3 * delay[i] - state[i + 1]) + state[i + 1];
				delay[i] = state[i];
			}
			samples[s] = state[4];
		}
	}
	
	void SetResonance(float r)
	{
		resonance = r;
		gRes = resonance * (1.0029 + 0.0526 * wc - 0.926 * pow(wc, 2) + 0.0218 * pow(wc, 3));
	}
	
    void SetSampleRate(float sr)
    {
        sampleRate = sr;
    }
    
	void SetCutoff(float c)
	{
		cutoff = c;
		wc = 2 * MOOG_PI * cutoff / sampleRate;
		g = 0.9892 * wc - 0.4342 * pow(wc, 2) + 0.1381 * pow(wc, 3) - 0.0202 * pow(wc, 4);
	}
	
     void SetDrive (float d)
    {
        drive = d;
        gComp = d;
    }
    
private:
	
	double state[5];
	double delay[5];
	double wc; // The angular frequency of the cutoff.
	double g; // A derived parameter for the cutoff frequency
	double gRes; // A similar derived parameter for resonance.
	double gComp; // Compensation factor.
	double drive; // A parameter that controls intensity of nonlinearities.
    double sampleRate;
    double cutoff;
	
};

#endif
