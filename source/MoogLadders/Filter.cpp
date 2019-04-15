/*
  ==============================================================================

    Filter.cpp
    Created: 15 Apr 2019 6:25:40pm
    Author:  VLSDESK003

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter() : sampleRate(44100.0), numSamples(1024)
{
	cutoff.set(1000.0);
	resonance.set(0.0);
	drive.set(1.0);
	reset();

	currentModel = MOOG_MODEL;
}

Filter::~Filter()
{
}

void Filter::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
	sampleRate = newSampleRate;
	numSamples = newSamplesPerBlock;

	multiplier = std::max(1.0, 96000.0 / sampleRate);
}

void Filter::reset()
{
	//moog reset
	for (int i = 0; i < 4; i++)
	{
		V[i] = 0;
		dV[i] = 0;
		tV[i] = 0;
	}
}

void Filter::update()
{
	switch (currentModel) {
	case MOOG_MODEL:
		x = (MOOG_PI * cutoff.get()) / (sampleRate * multiplier);
		g = 4.0 * MOOG_PI * VT * cutoff.get() * (1.0 - x) / (1.0 + x);
		break;
	}
}
void Filter::processBlock(AudioBuffer<double>& buffer, const int numSamples)
{
	const int blockSize = numSamples;
	double* channelData = buffer.getWritePointer(0);

	for (int i = 0; i < blockSize; i++) {
		double sample = channelData[i];
		double outSample = doFilterForModel(sample, currentModel);
		channelData[i] = outSample;
	}

}

void Filter::processBlock(AudioBuffer<float>& buffer, const int numSamples)
{
	const int blockSize = numSamples;
	float* channelData = buffer.getWritePointer(0);

	for (int i = 0; i < blockSize; i++) {
		float sample = channelData[i];
		float outSample = doFilterForModel(sample, currentModel);
		channelData[i] = outSample;
	}

}



template <typename FloatType>
FloatType Filter::doFilterForModel(FloatType sample, FilterModel model)
{
	FloatType value = 0.0;

	update();

	switch (model)
	{
	case MOOG_MODEL: {

		FloatType dV0, dV1, dV2, dV3;

		FloatType currentResonance = resonance.get() * 4.0;

		dV0 = -g * (saturation((drive.get() * sample + currentResonance * V[3]) / (2.0 * VT)) + tV[0]);
		V[0] += (dV0 + dV[0]) / (2.0 * sampleRate * multiplier);
		dV[0] = dV0;
		tV[0] = saturation(V[0] / (2.0 * VT));

		dV1 = g * (tV[0] - tV[1]);
		V[1] += (dV1 + dV[1]) / (2.0 * sampleRate * multiplier);
		dV[1] = dV1;
		tV[1] = saturation(V[1] / (2.0 * VT));

		dV2 = g * (tV[1] - tV[2]);
		V[2] += (dV2 + dV[2]) / (2.0 * sampleRate * multiplier);
		dV[2] = dV2;
		tV[2] = saturation(V[2] / (2.0 * VT));

		dV3 = g * (tV[2] - tV[3]);
		V[3] += (dV3 + dV[3]) / (2.0 * sampleRate * multiplier);
		dV[3] = dV3;
		tV[3] = saturation(V[3] / (2.0 * VT));

		value = V[3] * -1.0;
		break;
	}
		default:
			break;
	}
	return value;
}
