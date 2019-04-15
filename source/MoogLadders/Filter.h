/*
  ==============================================================================

    Filter.h
    Created: 15 Apr 2019 6:25:40pm
    Author:  VLSDESK003

  ==============================================================================
*/



#ifndef FILTER_H
#define FILTER_H


#include "../JuceLibraryCode/JuceHeader.h"
#include "util.h"


// Thermal voltage (26 milliwats at room temperature)
#define VT 0.312


class Filter {
	

public: 
	enum FilterModel {
		MOOG_MODEL,
		DIODE_MODEL,
		KORG_MODEL
	} currentModel;

	Filter();
	~Filter();



	void prepareToPlay(double newSampleRate, int newSamplesPerBlock);

	
	void processBlock(AudioBuffer<double>& buffer, const int numSamples);
	void processBlock(AudioBuffer<float>& buffer, const int numSamples);

	void reset();

	void update();

	void setModel(FilterModel newModel) { currentModel = newModel; }

	void setResonance(double newResonance) { resonance.set(newResonance); }
	bool setCutoff(double newCutoff) { 
		double c = clamp(newCutoff, 40.0, 20000.0);
		cutoff.set(c); 

		return true;
	}
	void setDrive(double newDrive) { drive.set(newDrive); }

private:
	
	double sampleRate;
	int numSamples;
	Atomic<double> cutoff, resonance, drive;

	template <typename FloatType>
	FloatType doFilterForModel(FloatType sample, FilterModel model);

	dsp::LookupTableTransform<double> saturation{
		[](double x) {
			return std::tanh(x);
		},
		double(-5),
		double(5),
		256
	};

	//moog specific
	std::array<double, 4> V, dV, tV;
	double x, g;
	double multiplier;
};

#endif // FILTER_H
