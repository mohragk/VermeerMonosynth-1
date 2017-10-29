// Copyright (c) 2014, Magnus Jonsson All rights reserved.

#pragma once

#ifndef MICROTRACKER_MODEL_H
#define MICROTRACKER_MODEL_H

#include "LadderFilterBase.h"
#include "util.h"

class MicrotrackerMoog : public LadderFilterBase
{

public:

	MicrotrackerMoog() : LadderFilterBase()
	{
		p0 = p1 = p2 = p3 = p32 = p33 = p34 = 0.0;
		SetCutoff(1000.0f);
		SetResonance(0.10f);
	}

	virtual ~MicrotrackerMoog()
	{
	}

	virtual void Process(float * samples, uint32_t n) noexcept override
	{
		double k = resonance * 4;
		for (int s = 0; s < n; ++s)
		{
			// Coefficients optimized using differential evolution
			// to make feedback gain 4.0 correspond closely to the
			// border of instability, for all values of omega.
			double out = p3 * 0.360891 + p32 * 0.417290 + p33 * 0.177896 + p34 * 0.0439725;

			p34 = p33;
			p33 = p32;
			p32 = p3;

			p0 += (fast_tanh(samples[s] - k * out) - fast_tanh(p0)) * cutoff;
			p1 += (fast_tanh(p0) - fast_tanh(p1)) * cutoff;
			p2 += (fast_tanh(p1) - fast_tanh(p2)) * cutoff;
			p3 += (fast_tanh(p2) - fast_tanh(p3)) * cutoff;

			samples[s] = out;
		}
	}
    
    virtual void SetSampleRate(float sr) override
    {
        sampleRate = sr;
    }
    

	virtual void SetResonance(float r) override
	{
		resonance = r;
	}

	virtual void SetCutoff(float c) override
	{
		cutoff = c * 2 * MOOG_PI / sampleRate;
		cutoff = moog_min(cutoff, 1);
	}
    
    virtual void SetDrive (float dr) override
    {
        drive = dr;
    }
    

private:

	double p0;
	double p1;
	double p2;
	double p3;
	double p32;
	double p33;
	double p34;
    
    double sampleRate;
    double drive;
};

#endif
