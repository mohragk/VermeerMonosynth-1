/*
  ==============================================================================

    Chorus.h
    Created: 11 Apr 2019 2:35:48pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef CHORUS_H
#define CHORUS_H

#include "../JuceLibraryCode/JuceHeader.h"


class Chorus {
public: 
	Chorus() : 
		delayTime(30.0), 
		width(30.0),
		depth(30.0), 
		numVoices(3),
		frequency(0.05)

	{}
	~Chorus() {}

	void setDelay(double delTime) {
		delayTime = delTime;
	}

	void setWidth(double w) {
		width = w;
	}

	void setDepth(double d) {
		depth = d;
	}

	void setNumVoices(int num) {
		numVoices = num;
	}

	void prepareToPlay(double sampleRate, int samplesPerBlock) {
		samplerate = sampleRate;
		numsamples = samplesPerBlock;

		double maxDelayTime = 50.0 + 50.0; //max delayTime + max Width
		delayBufferSamples = (int)(maxDelayTime * samplerate) + 1;
		if (delayBufferSamples < 1)
			delayBufferSamples = 1;

		delayBufferChannels = 2;
		delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
		delayBuffer.clear();


		delayWritePosition = 0;
		lfoPhase = 0.0;
		inverseSampleRate = 1.0 / sampleRate;

		twoPi = 2.0 * double_Pi;
	}

	void processBlock(AudioBuffer<double>& buffer);

private:
	AudioBuffer<double> delayBuffer;
	int delayBufferSamples;
	int delayBufferChannels; //maybe
	int delayWritePosition;

	double lfoPhase;
	double inverseSampleRate;
	double twoPi;

	double simpleLFO(double phase, int waveform = 0);

	double delayTime;
	double width;
	double depth;
	int numVoices;
	double frequency;

	double samplerate;
	int numsamples;

	JUCE_LEAK_DETECTOR(Chorus)

};


inline void Chorus::processBlock(AudioBuffer<double>& buffer) {
	ScopedNoDenormals noDenormals;

	const int numInputChannels = buffer.getNumChannels();
	const int numOutputChannels = numInputChannels;
	const int numSamples = buffer.getNumSamples();

	double currentDelay = delayTime;
	double currentWidth = width;
	double currentDepth = depth;
	double currentFrequency = frequency;

	int localWritePosition;
	double phase;

	for (int channel = 0; channel < numInputChannels; ++channel) {
		double* channelData = buffer.getWritePointer(channel);
		double* delayData = delayBuffer.getWritePointer(channel); //hmmm
		localWritePosition = delayWritePosition;

		for (int sample = 0; sample < numSamples; ++sample) {
			const double in = channelData[sample];
			double out = 0.0;
			double phaseOffset = 0.0;
			double weight;

			for (int voice = 0; voice < numVoices - 1; voice++) {
				if (numVoices > 2) {
					weight = (double)voice / (double)(numVoices - 2);
					if (channel != 0)
						weight = 1.0 - weight;
				}
				else {
					weight = 1.0;
				}

				double localDelayTime = (
					currentDelay + 
					currentWidth * 
					simpleLFO(phase + phaseOffset) * 
					samplerate
					);
				double readPosition = fmodf(
					(double) localWritePosition - localDelayTime,
					(double) delayBufferSamples
				);

				int localReadPosition = floorf(readPosition);

				int interpolationType = 0;
				switch (interpolationType) {
					case 0: {
						double closestSample = delayData[localReadPosition % delayBufferSamples];
						out = closestSample;
						break;
					}
				}

				if (numVoices == 2)
					channelData[sample] = (channel == 0) ? in : out * currentDepth;
				else
					channelData[sample] += out * currentDepth * weight;

				if (numVoices == 3)
					phaseOffset += 0.25;
				else if (numVoices > 3)
					phaseOffset += 1.0 / (double)(numVoices - 1);
			}

			delayData[localWritePosition] = in;

			if (++localWritePosition >= delayBufferSamples)
				localWritePosition -= delayBufferSamples;

			phase += currentFrequency * inverseSampleRate;
			if (phase >= 1.0)
				phase -= 1.0;
		}
	}

	delayWritePosition = localWritePosition;
	lfoPhase = phase;

	/*
	for (int channel = numInputChannels; channel < numOutputChannels; ++channel)
		buffer.clear(channel, 0, numSamples);
	*/

}


inline double Chorus::simpleLFO(double phase, int waveform = 0) {
	double out = 0.0f;

	switch (waveform) {
	case 0: {
		out = 0.5 + 0.5 * sinf(twoPi * phase);
		break;
	}
	}
	return out;
}

#endif //CHORUS_H
