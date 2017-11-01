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






/** A demo synth sound that's just a basic sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
    
};

#include "Oscillator.h"

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice() //: ampEnvelope(nullptr), pitchEnvelope(nullptr), osc1(nullptr), osc2(nullptr), osc3(nullptr)
    
    {
		ampEnvelope = new ADSR();
		pitchEnvelope = new ADSR();

		osc1 = new Oscillator();
		osc2 = new Oscillator();
		osc3 = new Oscillator();
        
    }
    
    bool hasOscillator()
    {
        return osc1 != nullptr;
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
        ampEnvelope->setSampleRate(sr);
        pitchEnvelope->setSampleRate(sr);
        
        osc1->setSampleRate(sr);
        osc2->setSampleRate(sr);
        osc3->setSampleRate(sr);
       
        
        
       
        
        midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        
        
        if (ampEnvelope->getState() == ADSR::env_idle)
        {
            osc1->setPhase( 0.0 );
            osc2->setPhase( 0.0 );
            osc3->setPhase( 0.0 );
        }
        
        
        ampEnvelope->gate(true);
        /*
         if (ampEnvelope.getState() == ADSR::env_attack
         || ampEnvelope.getState() == ADSR::env_decay
         || ampEnvelope.getState() == ADSR::env_sustain)
         {
         ampEnvelope.resetToAttack();
         }*/
        
        
        pitchEnvelope->gate(true);
        
        /*if (pitchEnvelope.getState() == ADSR::env_attack
         || pitchEnvelope.getState() == ADSR::env_decay
         || pitchEnvelope.getState() == ADSR::env_sustain)
         {
         pitchEnvelope.resetToAttack();
         }*/
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        
        ampEnvelope->gate(false);
        pitchEnvelope->gate(false);
        clearCurrentNote();
        
        if (ampEnvelope->getState() != ADSR::env_release)
        {
            osc1->setPhase( 0.0 );
            osc2->setPhase( 0.0 );
            osc3->setPhase( 0.0 );
                
        }
        
    }
    
    void pitchWheelMoved (int newValue) override
    {
        
        double range = 24.0;
        float v = newValue - 8192;
        
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
    
    
    
    //Set amplitude envelope parameters.
    
    void setAmpEnvelope( float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve)
    {
        ampEnvelope->setAttackRate(attack);
        ampEnvelope->setDecayRate(decay);
        ampEnvelope->setSustainLevel(sustain);
        ampEnvelope->setReleaseRate(release);
        ampEnvelope->setTargetRatioA(attackCurve);
        ampEnvelope->setTargetRatioDR(decRelCurve);
        
        
    }
    
    // Set pitch envelope parameters.
    void setPitchEnvelope (float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve)
    {
        pitchEnvelope->setAttackRate(attack);
        pitchEnvelope->setDecayRate(decay);
        pitchEnvelope->setSustainLevel(sustain);
        pitchEnvelope->setReleaseRate(release);
        pitchEnvelope->setTargetRatioA(attackCurve);
        pitchEnvelope->setTargetRatioDR(decRelCurve);
        
    }
    
    
    
    // Pretty dumb name, but this influences the amount of pitch deviation generated from the envelope.
    void setPitchEnvelopeAmount ( float pitchMod )
    {
        pitchModAmount = pitchMod;
    }
    
    void setAmpModulation(double amt)
    {
        ampModulation = (amt + 1.0) / 2.0; // Normalize
    }
    
    
    void setPitchModulation(double amt)
    {
        
        double rangeSemitones = 24.0;
        
        pitchModulation = amt * rangeSemitones;
        
    }
    
    
    
    void setOscGains(float g1, float g2, float g3)
    {
        osc1->setGain(g1);
        osc2->setGain(g2);
        osc3->setGain(g3);
    }
    
    void setOsc1DetuneAmount(double fine, int coarse)
    {
        oscDetuneAmount[0] = fine + (float) coarse; //Semitones
    }
    
    void setOsc2DetuneAmount(double fine, int coarse)
    {
        
        oscDetuneAmount[1] = fine + (float) coarse; //Semitones
    }
    
    void setOsc3DetuneAmount(double fine, int coarse)
    {
        
        oscDetuneAmount[2] = fine + (float) coarse; //Semitones
    }
    
    void setOscModes(int mode1, int mode2, int mode3)
    {
        osc1->setMode(mode1);
        osc2->setMode(mode2);
        osc3->setMode(mode3);
    }
    
    void setSawSaturation(float sat)
    {
        //sawSaturation = sat;
        
    }
    
private:
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        
        if (ampEnvelope->getState() != ADSR::env_idle)
        {
            
            while (--numSamples >= 0)
            {
                FloatType sample = 0.0;
                
                //Get Pitch Envelope Amount
                double pitchEnvAmt = pitchEnvelope->process();
                
                //Apply Pitch Envelope and PitchBend Amount, deviated from current pitch
                double newFreq = midiFrequency + ( pitchEnvAmt * pitchModAmount);
               
                
                //Calculate new frequencies after detuning by knob and/or LFO and/or pitchbend wheel
                double osc1Detuned = semitoneOffsetToFreq( oscDetuneAmount[0] + pitchModulation + pitchBendOffset, newFreq );
                double osc2Detuned = semitoneOffsetToFreq( oscDetuneAmount[1] + pitchModulation + pitchBendOffset, newFreq );
                double osc3Detuned = semitoneOffsetToFreq( oscDetuneAmount[2] + pitchModulation + pitchBendOffset, newFreq );
                
                //Set the new frequency
                osc1->setFrequency(osc1Detuned);
                osc2->setFrequency(osc2Detuned);
                osc3->setFrequency(osc3Detuned);

                // Calculate new phase increment and calculate samples
                
                sample = (osc1->nextSample() + osc2->nextSample() + osc3->nextSample()) / numOscillators;
                
                //sample *= ampEnvelope.process();
                
                //sample += (oscNoise.nextSample() * 0.25);
                
                
                // Softclip combined samples so signal is always between 0.75 and -0.75
                //softClip(sample);
                
                for (int i = 0; i < outputBuffer.getNumChannels(); i++ )
                {
                    
                    outputBuffer.addSample (i, startSample, sample);
                }
                
                ++startSample;
            }
            
            
        }
        
        
        
    }
    
    
    
    template <typename FloatType>
    FloatType softClip(FloatType s)
    {
        FloatType localSample = s;
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
    
    
    
    
    float inline semitoneOffsetToFreq(double semitones, double freq)
    {
        return pow(2.0, (semitones / 12.0)) * freq;
    }
    double phase = 0.0;
    
    ScopedPointer<ADSR> ampEnvelope, pitchEnvelope;
    ScopedPointer<Oscillator> osc1, osc2, osc3;
    
    int numOscillators = 3;
    
    int initialNote = 0;
    int noteOffset;
    //float angleDelta;
    
    double pitchModulation, ampModulation;
    double oscDetuneAmount[3];
    
    double pitchBendOffset;
    double glideTime;
    //double sawSaturation;
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
    //    double osc1DetuneAmount, osc2DetuneAmount;
    
    
};


