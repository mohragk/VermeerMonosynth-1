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

template <typename FloatType>
class Chorus {
public: 
	Chorus() : 
		delayTime(30.0), 
		width(30.0),
		depth(0.7), 
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

	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& buffer) {
		ScopedNoDenormals noDenormals;

		const int numInputChannels = buffer.getNumChannels();
		const int numSamples = buffer.getNumSamples();

		FloatType currentDelay = delayTime * 0.001;
		FloatType currentWidth = width * 0.001;
		FloatType currentDepth = depth;
		FloatType currentFrequency = frequency;

		int localWritePosition;
		FloatType phase;

		for (int channel = 0; channel < numInputChannels; ++channel) {
			FloatType* channelData = buffer.getWritePointer(channel);
			FloatType* delayData = delayBuffer.getWritePointer(channel); 

			localWritePosition = delayWritePosition;
			phase = lfoPhase;

			for (int sample = 0; sample < numSamples; ++sample) {
				const FloatType in = channelData[sample];
				FloatType out = 0.0;
				FloatType phaseOffset = 0.0;
				FloatType weight;

				for (int voice = 0; voice < numVoices - 1; ++voice) {
				if (numVoices > 2) {
						weight = (FloatType)voice / (FloatType)(numVoices - 2);
						if (channel != 0)
							weight = 1.0 - weight;
					}
					else {
						weight = 1.0;
					}

					FloatType localDelayTime = (
						currentDelay +
						currentWidth *
						simpleLFO(phase + phaseOffset)
						)  
						* samplerate;
					FloatType readPosition = fmodf(
						(FloatType)localWritePosition - localDelayTime + (FloatType)delayBufferSamples,
						(FloatType)delayBufferSamples
					);

					int localReadPosition = floorf(readPosition);

					int interpolationType = 0;
					switch (interpolationType) {
					case 0: {
						FloatType closestSample = delayData[localReadPosition % delayBufferSamples];
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
						phaseOffset += 1.0 / (FloatType)(numVoices - 1);
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

private:
	AudioBuffer<FloatType> delayBuffer;
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


template <typename FloatType>
inline double Chorus<FloatType>::simpleLFO(double phase, int waveform) {
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
