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

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice() : angleDelta(0.0)
    
    {
        
        
    }
    
    enum OscillatorMode {
        OSCILLATOR_MODE_SINE = 0,
        OSCILLATOR_MODE_SAW,
        OSCILLATOR_MODE_SQUARE,
        OSCILLATOR_MODE_NOISE, //unused
        numOscillatorModes
    };
    
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        
		ampEnvelope.setSampleRate(getSampleRate());
		pitchEnvelope.setSampleRate(getSampleRate());
		
		midiFrequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        
        for ( int i = 0; i < numOscillators; i++)
        {
            phase[i] = 0.0;
            level[i] = 0.2;
			oscFrequency[i].reset(getSampleRate(), 0.0001);

            oscFrequency[i].setValue(midiFrequency);
            phaseIncrement[i] = updatePhaseIncrement(oscFrequency[i].getNextValue());
        }
      
        
        ampEnvelope.gate(true);
		/*
		if (ampEnvelope.getState() == ADSR::env_attack
			|| ampEnvelope.getState() == ADSR::env_decay
			|| ampEnvelope.getState() == ADSR::env_sustain)
		{
			ampEnvelope.resetToAttack();
		}*/


        pitchEnvelope.gate(true);

		/*if (pitchEnvelope.getState() == ADSR::env_attack
			|| pitchEnvelope.getState() == ADSR::env_decay
			|| pitchEnvelope.getState() == ADSR::env_sustain)
		{
			pitchEnvelope.resetToAttack();
		}*/
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        
        ampEnvelope.gate(false);
        pitchEnvelope.gate(false);
        clearCurrentNote();
        
        if (ampEnvelope.getState() != ADSR::env_release)
        {
            for ( int i = 0; i < numOscillators; i++)
                phase[i] = 0.0;
        }
       
    }
    
    void pitchWheelMoved (int newValue) override
    {
		        
		double range = 12.0;
		
		pitchBendOffset = 0; // range / 16383.0;
    
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
        ampEnvelope.setAttackRate(attack);
        ampEnvelope.setDecayRate(decay);
        ampEnvelope.setSustainLevel(sustain);
        ampEnvelope.setReleaseRate(release);
		ampEnvelope.setTargetRatioA(attackCurve);
		ampEnvelope.setTargetRatioDR(decRelCurve);
        
        
    }
    
	// Set pitch envelope parameters.
    void setPitchEnvelope (float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve)
    {
        pitchEnvelope.setAttackRate(attack);
        pitchEnvelope.setDecayRate(decay);
        pitchEnvelope.setSustainLevel(sustain);
        pitchEnvelope.setReleaseRate(release);
		pitchEnvelope.setTargetRatioA(attackCurve);
		pitchEnvelope.setTargetRatioDR(decRelCurve);
        
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
        oscGain[0] = g1;
        oscGain[1] = g2;
        oscGain[2] = g3;
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
        if      (mode1 == 0)
            oscillatorMode[0] = OSCILLATOR_MODE_SINE;
        else if (mode1 == 1)
            oscillatorMode[0] = OSCILLATOR_MODE_SAW;
        else if (mode1 == 2)
            oscillatorMode[0] = OSCILLATOR_MODE_SQUARE;
        else if (mode1 == 3)
            oscillatorMode[0] = OSCILLATOR_MODE_NOISE;
        
        
        if      (mode2 == 0)
            oscillatorMode[1] = OSCILLATOR_MODE_SINE;
        else if (mode2 == 1)
            oscillatorMode[1] = OSCILLATOR_MODE_SAW;
        else if (mode2 == 2)
            oscillatorMode[1] = OSCILLATOR_MODE_SQUARE;
        else if (mode2 == 3)
            oscillatorMode[1] = OSCILLATOR_MODE_NOISE;
        
        
        if      (mode3 == 0)
            oscillatorMode[2] = OSCILLATOR_MODE_SINE;
        else if (mode3 == 1)
            oscillatorMode[2] = OSCILLATOR_MODE_SAW;
        else if (mode3 == 2)
            oscillatorMode[2] = OSCILLATOR_MODE_SQUARE;
        else if (mode3 == 3)
            oscillatorMode[2] = OSCILLATOR_MODE_NOISE;
        
    }
    
private:
    
    template <typename FloatType>
    void processBlock (AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
    {
        
        if (ampEnvelope.getState() != ADSR::env_idle)
        {
           
            while (--numSamples >= 0)
            {
                FloatType oscSample[3] = { 0.0 };
                
                //Get Pitch Envelope Amount
                double pitchEnvAmt = pitchEnvelope.process();
				
                //Apply Pitch Envelope and PitchBend Amount, deviated from current pitch
                double newFreqOsc1 = midiFrequency + ( pitchEnvAmt * pitchModAmount);
                double newFreqOsc2 = midiFrequency + ( pitchEnvAmt * pitchModAmount);
                double newFreqOsc3 = midiFrequency + ( pitchEnvAmt * pitchModAmount);
                
				//Calculate new frequencies after detuning by knob and/or LFO and/or pitchbend wheel
				double osc1Detuned = semitoneOffsetToFreq(oscDetuneAmount[0] + pitchModulation, newFreqOsc1);
				double osc2Detuned = semitoneOffsetToFreq(oscDetuneAmount[1] + pitchModulation, newFreqOsc2);
                double osc3Detuned = semitoneOffsetToFreq(oscDetuneAmount[2] + pitchModulation, newFreqOsc3);
                
                //Set the new frequency
                oscFrequency[0].setValue(osc1Detuned);
                oscFrequency[1].setValue(osc2Detuned);
                oscFrequency[2].setValue(osc3Detuned);
                
                
                
                // Calculate new phase increment and calculate samples
                for ( int osc = 0; osc < numOscillators ; osc++)
                {
                    phaseIncrement[osc] = updatePhaseIncrement(oscFrequency[osc].getNextValue());
                    
                    oscSample[osc] = nextSample(phase[osc], phaseIncrement[osc], oscillatorMode[osc]);
					
                    oscSample[osc] *= (level[osc] * oscGain[osc]);
                }
                
                
                // Combine all Oscillator Samples
                FloatType sample = (oscSample[0] + oscSample[1] + oscSample[2]) / numOscillators;
                sample *= ampEnvelope.process();
                
                
                // Softclip combined samples so signal is always between 0.75 and -0.75
                softClip(sample);
                
                for (int i = 0; i < outputBuffer.getNumChannels(); i++ )
                {
                    
                    outputBuffer.addSample (i, startSample, sample);
                }
        
                for (int i = 0; i < numOscillators; i++)
                {
                    phase[i] += phaseIncrement[i];
                    while (phase[i] >= 2.0 * double_Pi)
                    {
                        phase[i] -= 2.0 * double_Pi;
                    }
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

    

    double naiveWaveformForMode(OscillatorMode mode, double phase)
    {
        const double two_Pi = 2.0 * double_Pi;
        double value = 0.0;
        switch (mode) {
            case OSCILLATOR_MODE_SINE:
                value = sin(phase);
                break;
            case OSCILLATOR_MODE_SAW:
                value = (2.0 * phase / two_Pi) - 1.0;
                break;
            case OSCILLATOR_MODE_SQUARE:
                if (phase <= double_Pi) {
                    value = 1.0;
                } else {
                    value = -1.0;
                }
                break;
            case OSCILLATOR_MODE_NOISE:   

				
                break;
            default:
                break;
        }
        return value;
    }
    
    
    
    
    double nextSample(double phase, double phaseIncrement, OscillatorMode mode)
    {
        const double two_Pi = 2.0 * double_Pi;
        
        double value = 0.0;
        double t = phase / two_Pi;
        
        //oscillatorMode = OSCILLATOR_MODE_SQUARE;
        
        if ( mode == OSCILLATOR_MODE_SINE )
            value = naiveWaveformForMode( OSCILLATOR_MODE_SINE, phase );
        else if ( mode == OSCILLATOR_MODE_SAW )
        {
            value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, phase);
            value -= poly_blep(t, phaseIncrement);
        }
        else if ( mode == OSCILLATOR_MODE_SQUARE )
        {
            value = naiveWaveformForMode( OSCILLATOR_MODE_SQUARE, phase );
            value += poly_blep( t, phaseIncrement );
            value -= poly_blep( fmod( t + 0.5, 1.0 ), phaseIncrement );
            
        }
        else if (mode == OSCILLATOR_MODE_NOISE)
        {
			Random r;
			value = r.nextDouble();
        }
             
       
        return value;
        
    }
    
    double poly_blep (double t, double phaseIncrement)
    {
        double dt = phaseIncrement /  double_Pi;
        
        if (t < dt)
        {
            t /= dt;
            return t+t - t*t - 1.0;
        }
        else if (t > 1.0 - dt)
        {
            t = (t - 1.0) / dt;
            return t*t + t+t + 1.0;
        }
        else return 0.0;
    }
    
    
    
    double updatePhaseIncrement(double freq)
    {
        double nyFreq = fmin( freq, getSampleRate() / 2.0 );
        return ( ( 2.0 * double_Pi ) * nyFreq ) / getSampleRate();
    }
    
  //END OF TRIAL
    
    
    OscillatorMode oscillatorMode[3];
    ADSR ampEnvelope, pitchEnvelope;
    
    int numOscillators = 3;
    
    int initialNote = 0;
    int noteOffset;
    float angleDelta;
    
	double pitchModulation, ampModulation;

    double  phase[3], phaseIncrement[3], lastOutput[3], level[3], oscGain[3], oscDetuneAmount[3];
	double pitchBendOffset;
    
    double midiFrequency;
    double maxFreq = 0, minFreq = 0;
    double pitchModAmount;
//	double osc1DetuneAmount, osc2DetuneAmount;

    LinearSmoothedValue<float> oscFrequency[3];
    
    
};
