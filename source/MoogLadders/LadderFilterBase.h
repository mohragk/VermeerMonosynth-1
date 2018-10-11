/*
Copyright (c) 2015, Dimitri Diakopoulos All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#ifndef LADDER_FILTER_BASE_H
#define LADDER_FILTER_BASE_H

#include "util.h"
#include "../JuceLibraryCode/JuceHeader.h"

class LadderFilterBase
{
public:
	
    LadderFilterBase() { }
    virtual ~LadderFilterBase() { }
	
    virtual void Process(float * samples, size_t n) noexcept = 0;
    virtual void Process(double * samples, size_t n) noexcept = 0;
    virtual void ProcessRamp(float * samples, size_t n, float beginCutoff, float endCutoff) = 0;
    virtual void ProcessRamp(double * samples, size_t n, double beginCutoff, double endCutoff) = 0;

	virtual void Update() = 0;
    virtual void Reset() = 0;
    virtual void SetSampleRate(double sr) = 0;
    virtual void SetResonance(double r) = 0;
    virtual bool SetCutoff(double c) = 0;
    virtual void SetDrive(double d) = 0;
    virtual void AddModulationValue(double value) { cutoffValues.push_back(value); }
      
    enum FilterType {LPF1,HPF1,LPF2,HPF2,BPF2,BSF2,LPF4,HPF4,BPF4};
    
    virtual double GetResonance() { return resonance.get(); }
    virtual double GetCutoff() { return cutoff.get(); }
    virtual double GetSampleRate() { return sampleRate; }
	
protected:
	
	double sampleRate;
	Atomic<double> cutoff;
	Atomic<double> resonance;
	double drive;

	dsp::LookupTableTransform<double> saturationLUT{ [](double x) { return std::tanh(x); }, double(-5), double(5), 256 };
    std::vector<double> cutoffValues;
};

#endif
