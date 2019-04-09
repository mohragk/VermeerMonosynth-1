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

class Monosynthesiser;

//=======================================================================================
class MonosynthListener
{
public:
    MonosynthListener() noexcept {}
    virtual ~MonosynthListener()   {}
    
    
    
    virtual void isSynthesiserPlaying(Monosynthesiser* source, bool isPlaying) = 0;
    
    virtual void handleSynthNoteOn  (Monosynthesiser* source, int midiChannel, int midiNoteNumber) = 0;
    virtual void handleSynthNoteOff (Monosynthesiser* source, int midiChannel, int midiNoteNumber) = 0;
};


//=======================================================================================
class Monosynthesiser : public Synthesiser
{
public:
    
    Monosynthesiser() {}
    ~Monosynthesiser() {}
    
    virtual void noteOn (int midiChannel, int midiNoteNumber, float velocity)
    {
        Synthesiser::noteOn(midiChannel, midiNoteNumber, velocity);
        
        for (int i = listeners.size(); --i >= 0;)
            listeners.getUnchecked(i)->handleSynthNoteOn(this, midiChannel, midiNoteNumber);
    }
    
    virtual void noteOff (int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
    {
        Synthesiser::noteOff(midiChannel, midiNoteNumber, velocity, allowTailOff);
        
        for (int i = listeners.size(); --i >= 0;)
            listeners.getUnchecked(i)->handleSynthNoteOff(this, midiChannel, midiNoteNumber);
    }
    
    void addListener(MonosynthListener* listener)
    {
        listeners.addIfNotAlreadyThere(listener);
    }
    
    void removeListener(MonosynthListener* listener)
    {
        listeners.removeFirstMatchingValue(listener);
    }
private:
    Array<MonosynthListener*> listeners;
};






//=======================================================================================
class MonosynthSound : public SynthesiserSound
{
public:
	MonosynthSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
    
};



//=======================================================================================
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
        glideTimeMillis(0),
        sampleCounter(0),
        lfoValue(0.0),
        egValue(0.0),
        pitchModulation(0.0),
        pitchBendOffset(0.0),
        midiFrequency(0.0),
        maxFreq(0.0), minFreq(0.0),
        pitchModAmount(0.0),
        hardSync(false),
        lastNotePlayed(60)
    {
        pitchEnvelope.reset ( new ADSR );
        
        
		for (int n = 0; n < numOscillators; n++)
        {
            modAmountPW[n] = 0.0;
            oscDetuneAmount[n] = 0.0;
			osc[n].reset( new Oscillator );
        }
    }
    
    ~MonosynthVoice()
    {
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
        
		for (int n = 0; n < numOscillators; n++)
		{
			osc[n]->setSampleRate(sr);
			osc[n]->setVelocityFactor(velocity);
			

			// MAKE GUI FOR SETTING RETRIGGERING
			auto retrigger = false;
			if (retrigger)
				osc[n]->resetPhaseInterpolated();
		}
        
        midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        
        pitchEnvelope.get()->gate(true);

		lastNotePlayed = midiNoteNumber;
    
        isPlaying = true;
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        pitchEnvelope.get()->gate(false);
        
        isPlaying = false;
                
        clearCurrentNote();
    }
    
    void pitchWheelMoved (const int newValue) override
    {
        const double range = 2.0;
        const float v = newValue - 8192.0;
        
        pitchBendOffset =  range * (v / 8192.0);
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
    
    int getNumOscillators();
    int getLastNotePlayed();
    int getLowestPitchedOscillatorIdx();
    int getLowestOscillatorRephaseIndex();
    double getLowestPitchedOscFreq();
    double getPitchEnvelopeValue();

    void setEnvelopeSampleRate( double sr );
    void setPitchEnvelopeParameters (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve);
    void sendLFO(LFO& lfo);
    void setGlideTime(int timeInMillis);
    void setPitchEnvelopeAmount (const float pitchMod );
    void setPitchModulation(const double amt);
    void setGainForOscillator(const float g, const int oscillator);
    void setDetuneAmountForOscillator(const double fine, int coarse, int oscillator);
    void setModeForOscillator(const int mode, const int oscillator);
    void setHardSync(bool sync);
    void setPulsewidthModAmountForOscillator(double amt, int n);
    void setPulsewidthForOscillator(double pw, int n);
    
    bool isPlaying = false;
    
private:
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
		{
            FloatType* dataLeft = outputBuffer.getWritePointer(0);
           
			while (--numSamples >= 0)
			{
                sampleCounter = startSample;
				FloatType sample = 0.0;

				//Get Pitch Envelope Amount
				pitchEnvelopeVal = pitchEnvelope->process();

				//Apply Pitch Envelope and PitchBend Amount, deviated from current pitch
                FloatType newFreq = midiFrequency + (pitchEnvelopeVal * pitchModAmount);

				//Calculate new frequencies after detuning by knob and/or LFO and/or pitchbend wheel
                for (int i = 0; i < numOscillators; i++)
                {
                    targetFrequency[i] = semitoneOffsetToFreq(oscDetuneAmount[i] + pitchModulation + pitchBendOffset, newFreq);
                    updateGlidedFrequency(targetFrequency[i], currentFrequency[i], glideTimeMillis);
                    osc[i]->setFrequency(currentFrequency[i]);
                }
                               
                if (osc[0]->isRephase() && hardSync)
                    osc[1]->setPhase(0.0);

				// Calculate samples and divide by number of oscillators
				for (int n = 0; n < numOscillators; n++)
					sample += osc[n]->nextSample();

				sample /= numOscillators;

                // get amplitude envelope

                //sample *= ampEnvelope.get()->process();
                
                
                dataLeft[startSample] += sample;

				++startSample;
			}
		}
    }
    
    
    void updateGlidedFrequency (double targetFreq, double& currentFreq,  int glideT);
    
    
    double softClip(double s);
    double softClipParam(double val);
    double semitoneOffsetToFreq(const double semitones, const double freq);

    double sampleRate;

    double phase = 0.0;

    double targetFrequency[3];
    double currentFrequency[3];
    
	int envState;
    
    int numOscillators = 3;
    
    int initialNote = 0;
    int noteOffset;
    int glideTimeMillis;
    
    int sampleCounter;

    double lfoValue, egValue;
    double modAmountPW[3];
    
    double pitchModulation;
    double oscDetuneAmount[3];
   
    
    double pitchBendOffset = 0;
  
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
    double pitchEnvelopeVal;
    
    bool hardSync = false;
	int lastNotePlayed = 60;
    
    Random rand;
    
    std::unique_ptr<ADSR> pitchEnvelope;
    std::unique_ptr<Oscillator> osc[3];
    
    JUCE_LEAK_DETECTOR (MonosynthVoice)
};


//=======================================================================================
inline int MonosynthVoice::getNumOscillators()
{
    return numOscillators;
}


inline int MonosynthVoice::getLastNotePlayed()
{
    return lastNotePlayed;
}


inline int MonosynthVoice::getLowestPitchedOscillatorIdx()
{
    double lowest = getLowestPitchedOscFreq();

	for (int i = 0; i < numOscillators; i++) {
		
		if (std::abs( osc[i].get()->getFrequency() - lowest) < DBL_EPSILON) return i;
	}
    
    return 0;
    
}


inline int MonosynthVoice::getLowestOscillatorRephaseIndex()
{
    double lowest = getLowestPitchedOscFreq();
    int idx = 0;
    for (int i = 0; i < 3; i++)
    {
        if (std::abs( osc[i].get()->getFrequency() - lowest) < DBL_EPSILON )
            if (osc[i].get()->isRephase())
            {
                idx = sampleCounter + 1;
            }
    }
    
    return idx;
}

inline double MonosynthVoice::getPitchEnvelopeValue()
{
    return pitchEnvelopeVal;
}

inline void MonosynthVoice::setEnvelopeSampleRate( double sr )
{
    pitchEnvelope.get()->setSampleRate(sr);
    
}


inline void MonosynthVoice::setPitchEnvelopeParameters (const float attack, const float decay, const float sustain, const float release, const float attackCurve, const float decRelCurve)
{
    pitchEnvelope.get()->setAttackRate(attack);
    pitchEnvelope.get()->setDecayRate(decay);
    pitchEnvelope.get()->setSustainLevel(sustain);
    pitchEnvelope.get()->setReleaseRate(release);
    pitchEnvelope.get()->setTargetRatioA(attackCurve);
    pitchEnvelope.get()->setTargetRatioDR(decRelCurve);
}


inline double MonosynthVoice::getLowestPitchedOscFreq()
{
    return jmin( osc[0].get()->getFrequency(), osc[1].get()->getFrequency(), osc[2].get()->getFrequency() );
}


inline void MonosynthVoice::sendLFO(LFO& lfo)
{
    lfoValue = lfo.nextSample();
}


inline void MonosynthVoice::setGlideTime(int timeInMillis)
{
    glideTimeMillis = timeInMillis;
}


inline void MonosynthVoice::setPitchEnvelopeAmount (const float pitchMod )
{
    pitchModAmount = pitchMod;
}


inline void MonosynthVoice::setPitchModulation(const double amt)
{
    const double rangeSemitones = 24.0;
    
    pitchModulation = amt * rangeSemitones;
}


inline void MonosynthVoice::setGainForOscillator(const float gain, const int oscillator)
{
    osc[oscillator]->setGain(gain);
}


inline void MonosynthVoice::setDetuneAmountForOscillator(const double fine, int coarse, int oscillator)
{
    osc[oscillator].get()->setDetuneAmount(fine , coarse);
}


inline void MonosynthVoice::setModeForOscillator(const int mode, const int oscillator)
{
    osc[oscillator]->setMode(mode);
}


inline void MonosynthVoice::setHardSync(bool sync)
{
    hardSync = sync;
}


inline void MonosynthVoice::setPulsewidthModAmountForOscillator(double amt, int oscillator)
{
    modAmountPW[oscillator] = amt;
}


inline void MonosynthVoice::setPulsewidthForOscillator(double pw, int n)
{
    double newPW = pw + ( modAmountPW[n] * ((lfoValue + 1.0) / 2.0) );
    
	newPW = std::max(0.0, std::min(newPW, 1.0));

    osc[n]->setPulsewidth( newPW  );
}


inline void MonosynthVoice::updateGlidedFrequency (double targetFreq, double& currentFreq,  int glideT)
{
    if (glideTimeMillis > 0)
    {
        auto increment = ( targetFreq - currentFreq) / ((static_cast<double>(glideT) / 1000.0) * sampleRate);
        currentFreq = currentFreq + increment;
    }
    else
    {
        currentFreq = targetFreq;
    }
}


inline double MonosynthVoice::semitoneOffsetToFreq(const double semitones, const double freq)
{
    double power = semitones / 12.0;
    return pow(2.0, power) * freq;
}


inline double MonosynthVoice::softClip(double s)
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


inline double MonosynthVoice::softClipParam(double val)
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

#endif // MONOSYNTH_H
