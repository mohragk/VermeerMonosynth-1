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
class MonosynthSound : public SynthesiserSound
{
public:
	MonosynthSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
    
};


//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */

class MonosynthVoice  : public SynthesiserVoice
{
public:
	MonosynthVoice() : pitchEnvelope(nullptr)
    {
		pitchEnvelope = new ADSR();

		for (int n = 0; n < numOscillators; n++)
			osc[n] = std::unique_ptr<Oscillator>( new Oscillator );
    }
    
    ~MonosynthVoice()
    {
        pitchEnvelope = nullptr;
        
		for (int n = 0; n < numOscillators; n++)
			osc[n] = nullptr;
    }
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<MonosynthSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        
        double sr = getSampleRate();

		

		// Might be abundant, but just to be safe
        pitchEnvelope->setSampleRate(sr);
        
		for (int n = 0; n < numOscillators; n++)
		{
			osc[n]->setSampleRate(sr);
			osc[n]->setVelocityFactor(velocity);
		}
        
       
               
        midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);

        
        pitchEnvelope->gate(true);
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        pitchEnvelope->gate(false);
        clearCurrentNote();
    }
    
    void pitchWheelMoved (const int newValue) override
    {
        const double range = 2.0;
        const float v = newValue - 8192.0;
        
        pitchBendOffset =  range * (v / 8192.0);
		//pitchBendOffset = 0;
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
    
    int getNumOscillators()
    {
        return numOscillators;
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
        osc[0]->setGain(g1);
        osc[1]->setGain(g2);
        osc[2]->setGain(g3);
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
        osc[0]->setMode(mode1);
        osc[1]->setMode(mode2);
        osc[2]->setMode(mode3);
    }

	void sendEnvelope(ADSR& envelope)
	{
		envState =  envelope.getState();
	}
    
    void sendLFO(LFO& lfo)
    {
        lfoValue = (lfo.nextSample() + 1.0) / 2.0;
    }
    
    void setHardSync(bool sync)
    {
        hardSync = sync;
    }
    
    void setPWAmount(double amt, int osc)
    {
        if (osc == 0)
            modAmountPW1 = amt;
        else if (osc == 1)
            modAmountPW2 = amt;
        else
            modAmountPW3 = amt;
    }
  
	void setPulsewidth(double pw, int n)
	{
		
		if(n == 0)
			osc[0]->setPulsewidth(pw);

		 else if (n == 1)
			osc[1]->setPulsewidth(pw);

		 else 
			osc[2]->setPulsewidth(pw);

	 
	}

private:
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        
       
        
        if (envState == 0)
        {
			for (int n = 0; n < numOscillators; n++)
				osc[n]->setPhase(0.0);
            
        }
        
		if (envState != 0)
		{
			while (--numSamples >= 0)
			{
				FloatType sample = 0.0;

				//Get Pitch Envelope Amount
				FloatType pitchEnvAmt = pitchEnvelope->process();

				//Apply Pitch Envelope and PitchBend Amount, deviated from current pitch
				FloatType newFreq = midiFrequency + (pitchEnvAmt * pitchModAmount);

				//Calculate new frequencies after detuning by knob and/or LFO and/or pitchbend wheel
				FloatType osc1Detuned = semitoneOffsetToFreq(oscDetuneAmount[0] + pitchModulation + pitchBendOffset, newFreq);
				FloatType osc2Detuned = semitoneOffsetToFreq(oscDetuneAmount[1] + pitchModulation + pitchBendOffset, newFreq);
				FloatType osc3Detuned = semitoneOffsetToFreq(oscDetuneAmount[2] + pitchModulation + pitchBendOffset, newFreq);

				//Set the new frequency
				osc[0]->setFrequency(osc1Detuned);
				osc[1]->setFrequency(osc2Detuned);
				osc[2]->setFrequency(osc3Detuned);
                
                               
                if (osc[0]->isRephase() && hardSync)
                    osc[1]->setPhase(0.0);

				// Calculate samples and divide by number of oscillators
				for (int n = 0; n < numOscillators; n++)
					sample += osc[n]->nextSample();

				sample /= numOscillators;

                
                FloatType* dataLeft = outputBuffer.getWritePointer(0);
                FloatType* dataRight = outputBuffer.getWritePointer(1);
                
                dataLeft[startSample] = sample;
                dataRight[startSample] = sample;
                
                /*
				for (int i = 0; i < outputBuffer.getNumChannels(); i++)
				{
                    outputBuffer.addSample(i, startSample, sample);
				}
                */
                
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
        double power = semitones / 12.0;
        return pow(2.0, power) * freq;
    }

    double sampleRate;

    double phase = 0.0;

	int envState;
    
    int numOscillators = 3;
    
    int initialNote = 0;
    int noteOffset;

    double lfoValue, egValue;
    double modAmountPW1, modAmountPW2, modAmountPW3;
    
    double pitchModulation;
    double oscDetuneAmount[3];
   
    
    double pitchBendOffset = 0;
    double glideTime;
  
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
    
    bool hardSync = false;
    
    ScopedPointer<ADSR> pitchEnvelope;
    std::unique_ptr<Oscillator> osc[3];
};


