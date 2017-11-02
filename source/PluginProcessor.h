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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "adsr/ADSR.h"
#include "MoogLadders/ImprovedModel.h"
#include "MoogLadders/HuovilainenModel.h"
#include "MoogLadders/MusicDSPModel.h"
#include "MoogLadders/OberheimVariationModel.h"
#include "MoogLadders/MicrotrackerModel.h"

#include "lfo.h"



//==============================================================================
/**
    As the name suggest, this class does the actual audio processing.
*/
class JuceDemoPluginAudioProcessor  : public AudioProcessor,
                                        private MidiKeyboardStateListener
{
public:
    //==============================================================================
    JuceDemoPluginAudioProcessor();
    ~JuceDemoPluginAudioProcessor();

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    //==============================================================================
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (! isUsingDoublePrecision());
        process (buffer, midiMessages);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages);
    }

    //==============================================================================
    bool hasEditor() const override                                             { return true; }
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override                                       { return JucePlugin_Name; }

    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return true; }

    double getTailLengthSeconds() const override                                { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                               { return 0; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        { return String(); }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}

    //==============================================================================
    void getStateInformation (MemoryBlock&) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;
    
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    

    //intialise filter
    void initFilter(int i);
    
    
    //Set Envelope values
    void setAmpEnvelope(float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve);
    void setPitchEnvelope(float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve);
   

    
    void setPitchEnvelopeAmount( float pitchMod);
    //void setOscOffset(float offset);
    void setOsc1DetuneAmount(float fine, int coarse);
    void setOsc2DetuneAmount(float fine, int coarse);
    void setOsc3DetuneAmount(float fine, int coarse);
    void setOscGains(float osc1Gain, float osc2Gain, float osc3Gain);
    void setOscModes(int osc1Mode, int osc2Mode, int osc3Mode);
    void setSawSaturation(float sat);

	bool noteIsBeingPlayed();
	
  
    
    // Our parameters
    AudioParameterFloat* gainParam;
    
    AudioParameterFloat* osc1GainParam;
    AudioParameterFloat* osc2GainParam;
    AudioParameterFloat* osc3GainParam;
    AudioParameterFloat* osc1DetuneAmountParam;
    AudioParameterFloat* osc2DetuneAmountParam;
    AudioParameterFloat* osc3DetuneAmountParam;
    
    AudioParameterInt* osc1ModeParam;
    AudioParameterInt* osc2ModeParam;
    AudioParameterInt* osc3ModeParam;
    
    AudioParameterFloat* filterParam;
    AudioParameterFloat* filterQParam;
    AudioParameterFloat* filterContourParam;
    AudioParameterFloat* filterDriveParam;


    AudioParameterFloat* pitchModParam;

    AudioParameterInt* oscOffsetParam;
    AudioParameterInt* osc2OffsetParam;
    AudioParameterInt* osc3OffsetParam;

    
    AudioParameterFloat* attackParam1;
    AudioParameterFloat* decayParam1;
    AudioParameterFloat* sustainParam1;
    AudioParameterFloat* releaseParam1;
    AudioParameterFloat* attackCurve1Param;
    AudioParameterFloat* decayRelCurve1Param;


    AudioParameterFloat* attackParam2;
    AudioParameterFloat* decayParam2;
    AudioParameterFloat* sustainParam2;
    AudioParameterFloat* releaseParam2;
    AudioParameterFloat* attackCurve2Param;
    AudioParameterFloat* decayRelCurve2Param;



    AudioParameterFloat* attackParam3;
    AudioParameterFloat* decayParam3;
    AudioParameterFloat* sustainParam3;
    AudioParameterFloat* releaseParam3;
    AudioParameterFloat* attackCurve3Param;
    AudioParameterFloat* decayRelCurve3Param;

    AudioParameterInt*   modTargetParam;

    AudioParameterFloat* lfoRateParam;
    AudioParameterInt*   lfoModeParam;
    AudioParameterFloat* lfoIntensityParam;
    
    AudioParameterFloat* sawSaturationParam;
    AudioParameterInt* filterSelectParam;
    
    AudioParameterInt* lfoDivisionParam;
    
    
private:
    //==============================================================================
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages);
    
	template <typename FloatType>
    void applyGain (AudioBuffer<FloatType>& buffer);
   
	template <typename FloatType>
    void applyFilterEnvelope (AudioBuffer<FloatType>& buffer);
   
	template <typename FloatType>
    void applyFilter (AudioBuffer<FloatType>& buffer);
   
	template <typename FloatType>
    void applyDelay (AudioBuffer<FloatType>& buffer);
    
	template <typename FloatType>
    void applyAmpEnvelope (AudioBuffer<FloatType>& buffer);

	template <typename FloatType>
	void applyAmp (AudioBuffer<FloatType>& buffer);
    
    void calculateLFOSyncedFreq();

    AudioBuffer<float> delayBufferFloat;
    AudioBuffer<double> delayBufferDouble;
    int delayPosition;

    Synthesiser synth;
    

	bool noteIsPlaying = false;
    
    ScopedPointer<LadderFilterBase> filter[2];
 
    
    enum modTarget {
        modPitch,
        modCutoff,
        off,
    };
    
    void applyModToTarget(int target, double amount);

    ScopedPointer<ADSR> filterEnvelope;
    ScopedPointer<ADSR> ampEnvelope;
    LFO lfo;
    
    double lfo_synced_freq, lfo_synced_freq_old;
    double lfo_division;

    double modAmount;
    
    int mNumVoices;
    int lastChosenFilter;

    void initialiseSynth();
    void updateCurrentTimeInfoFromHost();
    void updateParameters();
    
    float softClip(float s);

    float contourVelocity;
	double cutoffModulationAmt; 
    
    float currentCutoff;
    
    double sampleRate;
    
	LinearSmoothedValue<double> cutoff, resonance, drive, envGain, switchGain;
	//
	double cutoffRampTimeDefault = 0.0025, cutoffRampTime;
	
    
    int lastNotePlayed;
    
    
    static BusesProperties getBusesProperties();
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceDemoPluginAudioProcessor)
};
