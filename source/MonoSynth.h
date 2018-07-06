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
#ifndef MONOSYNTH_H
#define MONOSYNTH_H

#include "Oscillator.h"
#include "lfo.h"
#include "adsr/ADSR.h"
#include "ParamSmoother.h"

#define NUM_OSCILLATORS 3


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
    MonosynthVoice() :
        sampleRate(44100.0),
        phase(0.0),
        envState(0),
        numOscillators(3),
        initialNote(0),
        noteOffset(0),
        lfoValue(0.0),
        egValue(0.0),
        pitchModulation(0.0),
        pitchBendOffset(0.0),
        glideTime(0.0),
        midiFrequency(0.0),
        maxFreq(0.0), minFreq(0.0),
        pitchModAmount(0.0),
        hardSync(false)
    {
        pitchEnvelope = std::unique_ptr<ADSR> ( new ADSR );
        ampEnvelope   = std::unique_ptr<ADSR> ( new ADSR );
        filterEnvelope   = std::unique_ptr<ADSR> ( new ADSR );
        
		for (int n = 0; n < numOscillators; n++)
        {
            modAmountPW[n] = 0.0;
            oscDetuneAmount[n] = 0.0;
			osc[n] = std::unique_ptr<Oscillator>( new Oscillator );
        }
    }
    
    ~MonosynthVoice()
    {
        
		for (int n = 0; n < numOscillators; n++)
			osc[n] = nullptr;
    }
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<MonosynthSound*> (sound) != nullptr;
    }
    
    
    void setEnvelopeSampleRate( double sr )
    {
        pitchEnvelope.get()->setSampleRate(sr);
        ampEnvelope.get()->setSampleRate(sr);
        
        
        
    }
    
    void setFilterEnvelopeSampleRate(double sr)
    {
        filterEnvelope.get()->setSampleRate(sr);
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        double sr = getSampleRate();
        
		// Might be abundant, but just to be safe
        //pitchEnvelope->setSampleRate(sr);
        
		for (int n = 0; n < numOscillators; n++)
		{
			osc[n]->setSampleRate(sr);
			osc[n]->setVelocityFactor(velocity);
		}
        
        midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        
        pitchEnvelope.get()->gate(true);
        ampEnvelope.get()->gate(true);
        filterEnvelope.get()->gate(true);

		lastNotePlayed = midiNoteNumber;
        
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        pitchEnvelope.get()->gate(false);
        ampEnvelope.get()->gate(false);
        filterEnvelope.get()->gate(false);
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
    

	int getLastNotePlayed()
	{
		return lastNotePlayed;
	}
    
    // Set pitch envelope parameters.
    void setPitchEnvelope (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve)
    {
        pitchEnvelope.get()->setAttackRate(attack);
        pitchEnvelope.get()->setDecayRate(decay);
        pitchEnvelope.get()->setSustainLevel(sustain);
        pitchEnvelope.get()->setReleaseRate(release);
        pitchEnvelope.get()->setTargetRatioA(attackCurve);
        pitchEnvelope.get()->setTargetRatioDR(decRelCurve);
    }
    
    void setAmpEnvelope (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve)
    {
        ampEnvelope.get()->setAttackRate(attack);
        ampEnvelope.get()->setDecayRate(decay);
        ampEnvelope.get()->setSustainLevel(sustain);
        ampEnvelope.get()->setReleaseRate(release);
        ampEnvelope.get()->setTargetRatioA(attackCurve);
        ampEnvelope.get()->setTargetRatioDR(decRelCurve);
    }
    
    void filterAmpEnvelope (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve)
    {
        filterEnvelope.get()->setAttackRate(attack);
        filterEnvelope.get()->setDecayRate(decay);
        filterEnvelope.get()->setSustainLevel(sustain);
        filterEnvelope.get()->setReleaseRate(release);
        filterEnvelope.get()->setTargetRatioA(attackCurve);
        filterEnvelope.get()->setTargetRatioDR(decRelCurve);
    }
    
    
    double getFilterEnvelopeValue()
    {
        return filterEnvelope.get()->process();
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
    
    void setGainForOscillator(const float g, const int o)
    {
        osc[o]->setGain(g);
    }
    
    void setDetuneAmountForOscillator(const double fine, int coarse, int osc)
    {
        oscDetuneAmount[osc] = fine + (double) coarse;
    }
    
    
    
    void setModeForOscillator(const int mode, const int o)
    {
        osc[o]->setMode(mode);
    }


    double getLowestPitchedOscFreq()
    {
        return jmin( osc[0].get()->getFrequency(), osc[1].get()->getFrequency(), osc[2].get()->getFrequency() );

    }
    
    int getLowestOscillatorRephaseIndex()
    {
        double lowest = getLowestPitchedOscFreq();
        int idx = 0;
        for (int i = 0; i < 3; i++)
        {
            if ( osc[i].get()->getFrequency() == lowest )
                if (osc[i].get()->isRephase())
                {
                    idx = sampleCounter + 1;
                }
            
        }
        
        return idx;
    }
    
    void sendLFO(LFO& lfo)
    {
        lfoValue = lfo.nextSample();
    }
    
    void setHardSync(bool sync)
    {
        hardSync = sync;
    }
    
    void setPulsewidthModAmountForOscillator(double amt, int n)
    {
        modAmountPW[n] = amt;
    }
  
	void setPulsewidthForOscillator(double pw, int n)
	{
        double newPW = pw + ( modAmountPW[n] * ((lfoValue + 1.0) / 2.0) );
        
        if (newPW > 1.0)
            newPW = 1.0;
        
        if(newPW < 0.0)
            newPW = 0.0;
        
		osc[n]->setPulsewidth( newPW  );
	}

private:
    
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        
        if (ampEnvelope.get()->getState() == ADSR::env_idle)
        {
			for (int n = 0; n < numOscillators; n++)
				osc[n]->setPhase(0.0);
        }
        
        if (ampEnvelope.get()->getState() != ADSR::env_idle)
		{
           
			while (--numSamples >= 0)
			{
                sampleCounter = startSample;
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

                // get amplitude envelope
                sample *= ampEnvelope.get()->process();
                
                FloatType* dataLeft = outputBuffer.getWritePointer(0);
                FloatType* dataRight = outputBuffer.getWritePointer(1);
                
                dataLeft[startSample] = sample;
                dataRight[startSample] = sample;
                
                
                
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
    
    double softClipParam(double val)
    {
        double localVal = val;
        
        if (localVal > 1.0)
            localVal = 1.0;
        else if (localVal < 0.0)
            localVal = 0.0;
        else
            localVal = localVal - (localVal * localVal * localVal);
        
        return localVal;
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
    
    int sampleCounter;

    double lfoValue, egValue;
    double modAmountPW[3];
    
    double pitchModulation;
    double oscDetuneAmount[3];
   
    
    double pitchBendOffset = 0;
    double glideTime;
  
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
    
    bool hardSync = false;
	int lastNotePlayed = 60;
    
    Random rand;
    
    std::unique_ptr<ADSR> pitchEnvelope;
    std::unique_ptr<ADSR> ampEnvelope;
    std::unique_ptr<ADSR> filterEnvelope;
    
    std::unique_ptr<Oscillator> osc[3];
        
    JUCE_LEAK_DETECTOR (MonosynthVoice)
};

#endif // MONOSYNTH_H
