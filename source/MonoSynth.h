/*
==============================================================================

This file is part of the JUCE library.
Copyright (c) 2017 - ROLI Ltd.

JUCE is an open source library subject to commercial or open-source
licensing.

By using JUCE, you agree to the terms of both the JUCE 5 End-User License
Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
                                     27th April 2017).

End User License Agreement: www.juce.com/juce-5-licence
Privacy Policy: www.juce.com/juce-5-privacy-policy

Or: You may also use this code under the terms of the GPL v3 (see
                                                              www.gnu.org/licenses).

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
DISCLAIMED.

==============================================================================
*/

#include "Oscillator.h"

/** A demo synth sound that's just a basic sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
    
};


//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */

class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice() //: pitchEnvelope(nullptr), osc1(nullptr), osc2(nullptr), osc3(nullptr)
    
    {
		pitchEnvelope = new ADSR();

		osc1 = new Oscillator();
		osc2 = new Oscillator();
		osc3 = new Oscillator();
    }
    
    ~SineWaveVoice()
    {
        pitchEnvelope = nullptr;
        
        osc1 = nullptr;
        osc2 = nullptr;
        osc3 = nullptr;
    }
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        
        double sr = getSampleRate();

		// Might be abundant, but just to be safe
        pitchEnvelope->setSampleRate(sr);
        
        osc1->setSampleRate(sr);
        osc2->setSampleRate(sr);
        osc3->setSampleRate(sr);
               
        midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);

		if (envState == 0)
		{
			osc1->setPhase(0.0);
			osc2->setPhase(0.0);
			osc3->setPhase(0.0);
		}
        
        pitchEnvelope->gate(true);
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        pitchEnvelope->gate(false);
        clearCurrentNote();
    }
    
    void pitchWheelMoved (const int newValue) override
    {
        const double range = 24.0;
        const float v = newValue - 8192.0;
        
        //pitchBendOffset =  range * (v / 8192.0);
		pitchBendOffset = 0;
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        processBlock (outputBuffer, startSample, numSamples);
    }
    
    void renderNextBlock (AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
    {
        processBlock (outputBuffer, startSample, numSamples);
    }
    
    
    // Set pitch envelope parameters.
    void setPitchEnvelope (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve)
    {
        pitchEnvelope->setAttackRate(attack);
        pitchEnvelope->setDecayRate(decay);
        pitchEnvelope->setSustainLevel(sustain);
        pitchEnvelope->setReleaseRate(release);
        pitchEnvelope->setTargetRatioA(attackCurve);
        pitchEnvelope->setTargetRatioDR(decRelCurve);
    }
    
    // Pretty dumb name, but this influences the amount of pitch deviation generated from the envelope.
    void setPitchEnvelopeAmount (const float pitchMod )
    {
        pitchModAmount = pitchMod;
    }
    
    void setPitchModulation(const double amt)
    {
        const double rangeSemitones = 24.0;
        
        pitchModulation = amt * rangeSemitones;
    }
    
    void setOscGains(const float g1, const float g2, const float g3)
    {
        osc1->setGain(g1);
        osc2->setGain(g2);
        osc3->setGain(g3);
    }
    
    void setOsc1DetuneAmount(const double fine, const int coarse)
    {
        oscDetuneAmount[0] = fine + (double) coarse; //Semitones
    }
    
    void setOsc2DetuneAmount(const double fine, const int coarse)
    {
        oscDetuneAmount[1] = fine + (double) coarse; //Semitones
    }
    
    void setOsc3DetuneAmount(const double fine, const int coarse)
    {
        oscDetuneAmount[2] = fine + (double) coarse; //Semitones
    }
    
    void setOscModes(const int mode1, const int mode2, const int mode3)
    {
        osc1->setMode(mode1);
        osc2->setMode(mode2);
        osc3->setMode(mode3);
    }

	void setEnvelopeState(ADSR& envelope)
	{
		envState =  envelope.getState();
	}
    
    void setHardSync(bool sync)
    {
        hardSync = sync;
    }

private:
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        osc1->setSampleRate(getSampleRate());
        osc2->setSampleRate(getSampleRate());
        osc3->setSampleRate(getSampleRate());
        
		if (envState != 0)
		{
			while (--numSamples >= 0)
			{
				double sample = 0.0;

				//Get Pitch Envelope Amount
				double pitchEnvAmt = pitchEnvelope->process();

				//Apply Pitch Envelope and PitchBend Amount, deviated from current pitch
				double newFreq = midiFrequency + (pitchEnvAmt * pitchModAmount);

				//Calculate new frequencies after detuning by knob and/or LFO and/or pitchbend wheel
				double osc1Detuned = semitoneOffsetToFreq(oscDetuneAmount[0] + pitchModulation, newFreq);
				double osc2Detuned = semitoneOffsetToFreq(oscDetuneAmount[1] + pitchModulation, newFreq);
				double osc3Detuned = semitoneOffsetToFreq(oscDetuneAmount[2] + pitchModulation, newFreq);

				//Set the new frequency
				osc1->setFrequency(osc1Detuned);
				osc2->setFrequency(osc2Detuned);
				osc3->setFrequency(osc3Detuned);
                
                               
                if (osc1->isRephase() && hardSync)
                    osc2->setPhase(0.0);

				// Calculate samples and divide by number of oscillators
				sample = (osc1->nextSample() + osc2->nextSample() + osc3->nextSample()) / numOscillators;

				for (int i = 0; i < outputBuffer.getNumChannels(); i++)
				{
					outputBuffer.addSample(i, startSample, sample);
				}

				++startSample;
			}
		}
    }
    
    
    
    double softClip(double s)
    {
		double localSample = s;
        if (localSample > 1.0f)
        {
            localSample = 0.75f;
        }
        else if (localSample < -1.0f)
        {
            localSample = -0.75f;
            
        }
        else
        {
            localSample = localSample - ( ( localSample * localSample * localSample) * 0.25f );
        }
        return localSample;
    }
    
    double inline semitoneOffsetToFreq(const double semitones, const double freq)
    {
        return pow(2.0, (semitones / 12.0)) * freq;
    }

    double sampleRate;

    double phase = 0.0;
    
    ScopedPointer<ADSR> pitchEnvelope;
    ScopedPointer<Oscillator> osc1, osc2, osc3;

	int envState;
    
    int numOscillators = 3;
    
    int initialNote = 0;
    int noteOffset;

    
    double pitchModulation, ampModulation;
    double oscDetuneAmount[3];
    
    double pitchBendOffset = 0;
    double glideTime;
  
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
    
    bool hardSync = false;
};


