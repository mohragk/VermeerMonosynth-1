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

	// KORG specific
	// set filter types
	va_LPF1.setFilterType(VAOnePole::FilterType::LPF1);
	va_LPF2.setFilterType(VAOnePole::FilterType::LPF1);
	va_HPF1.setFilterType(VAOnePole::FilterType::HPF1);
	va_HPF2.setFilterType(VAOnePole::FilterType::HPF1);
}

Filter::~Filter()
{
}

void Filter::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
	sampleRate = newSampleRate;
	numSamples = newSamplesPerBlock;

	multiplier = std::max(1.0, 96000.0 / sampleRate);

	va_LPF1.SetSampleRate(sampleRate);
	va_LPF2.SetSampleRate(sampleRate);
	va_HPF1.SetSampleRate(sampleRate);
	va_HPF2.SetSampleRate(sampleRate);
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
		{
			x = (MOOG_PI * cutoff.get()) / (sampleRate * multiplier);
			g = 4.0 * MOOG_PI * VT * cutoff.get() * (1.0 - x) / (1.0 + x);
			break;
		}
		case KORG_MODEL: 
		{
			double currentResonance = map(resonance.get(), 0.0, 1.0, 0.01, 2.0);

			//prewarp for BZT
			double wd = 2 * MOOG_PI * cutoff.get();
			double T = 1 / sampleRate;
			double wa = (2 / T) * tan(wd * T / 2);
			double g = wa * T / 2;


			// feedforward coeff, same for LPF as HPF
			double G = g / (1.0 + g);

			va_LPF1.Alpha = G;
			va_LPF2.Alpha = G;
			va_HPF1.Alpha = G;
			va_HPF2.Alpha = G;

			// set Alpha0; same for LPF as HPF
			Alpha0 = 1.0 / (1.0 - currentResonance * G + currentResonance * G * G);

			if (type == VAOnePole::FilterType::LPF2)
			{
				va_LPF2.Beta = (currentResonance - currentResonance * G) / (1.0 + g);
				va_HPF1.Beta = -1.0 / (1.0 + g);
			}
			else
			{
				va_HPF2.Beta = -1.0 * G / (1.0 + g);
				va_LPF1.Beta = 1.0 / (1.0 + g);
			}
		}
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
	FloatType value = sample;

	update();

	switch (model)
	{
		case MOOG_MODEL: 
		{

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
		case KORG_MODEL: 
		{
			FloatType y = 0.0;
			FloatType currentDrive = map(drive.get(), 1.0, 5.0, 1.0, 2.0);
			FloatType currentResonance = map(resonance.get(), 0.0, 1.0, 0.01, 2.0);
			const int pos = 0;

			if (type == VAOnePole::FilterType::LPF2)
			{
				FloatType y1 = va_LPF1.doFilter(sample, pos);

				FloatType S35 = va_HPF1.getFeedbackOutput() +
					va_LPF2.getFeedbackOutput();

				FloatType u = Alpha0 * y1 + S35;

				u = saturation(currentDrive * u);

				y = currentResonance * va_LPF2.doFilter(u, pos);

				va_HPF1.doFilter(y, pos);
			}
			else
			{
				FloatType y1 = va_HPF1.doFilter(sample, pos);

				FloatType S35 = va_HPF2.getFeedbackOutput() +
					va_LPF1.getFeedbackOutput();

				FloatType u = Alpha0 * y1 + S35;

				y = currentResonance * u;

				y = saturation(currentDrive * y);

				va_LPF1.doFilter(va_HPF2.doFilter(y, pos), pos);
			}

			if (currentResonance > 0)
				y *= 1 / currentResonance;

			value = y;
		}

		default:
			break;
	}
	return value;
}
