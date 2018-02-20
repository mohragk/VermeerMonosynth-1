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
#include "PulseClock.h"
#include "lfo.h"
#include "SequencerState.h"

#include "MoogLadders/ImprovedModel.h"
#include "MoogLadders/SEMModel.h"
#include "MoogLadders/VAOnePole.h"
#include "MoogLadders/DiodeLadderModel.h"
#include "MoogLadders/ThreeFiveModel.h"






//==============================================================================
/**
    As the name suggest, this class does the actual audio processing.
*/
class MonosynthPluginAudioProcessor  : public AudioProcessor,
                                        private MidiKeyboardStateListener,
										private SequencerStateListener,
                                        private Timer
{
public:
    //==============================================================================
    MonosynthPluginAudioProcessor();
    ~MonosynthPluginAudioProcessor();

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;
    void timerCallback() override;

    //==============================================================================
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {

        jassert (! isUsingDoublePrecision());
        if (hqOversampling)
            process (buffer, midiMessages, oversamplingFloatHQ);
        else
            process (buffer, midiMessages, oversamplingFloat);
    }
    
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {

        jassert (! isUsingDoublePrecision());
        if (hqOversampling)
            process (buffer, midiMessages, oversamplingDoubleHQ);
        else
            process (buffer, midiMessages, oversamplingDouble);
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

    void toggleHQOversampling(bool q);
    
    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;
	SequencerState sequencerState;
    
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

	void handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
	void handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    
	bool noteIsBeingPlayed();
    
    bool saturationOn();

	bool lfoSynced();
    

	//@Cleanup: unused?
    SynthesiserVoice* getSynthesiserVoice() { return synth.getVoice(0) ; };
      
    
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
    
    AudioParameterInt* oscSyncParam;
    
    AudioParameterFloat* filterCutoffParam;
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
	AudioParameterInt*	lfoSyncParam;

    AudioParameterInt* filterSelectParam;
    
    AudioParameterInt* lfoDivisionParam;
    
    
    
    AudioParameterInt* overSampleParam;
    
    AudioParameterInt* filterOrderParam;
    

	AudioParameterFloat* pulsewidth1Param;
	AudioParameterFloat* pulsewidth2Param;
	AudioParameterFloat* pulsewidth3Param;
    
    AudioParameterFloat* pulsewidthAmount1Param;
    AudioParameterFloat* pulsewidthAmount2Param;
    AudioParameterFloat* pulsewidthAmount3Param;

	AudioParameterFloat* saturationParam;
    AudioParameterInt* waveshapeSwitchParam;
    AudioParameterInt* waveshapeModeParam;
    
    AudioParameterInt* oversampleSwitchParam;
    AudioParameterInt* softClipSwitchParam;
    
    AudioParameterInt* stepPitchParam[8]; //TODO MAKE GLOBAL VALUE
	AudioParameterFloat* stepNoteLengthParam;
	AudioParameterInt* sequencerStepDivisionParam;
	int sequencerStepDivisionVal;

	std::vector<int> noteOffPositions;

	String debugInfo;
private:
	
    
	class SmoothParam {
	public:
		SmoothParam()
		{
			reset();
		}

		~SmoothParam()
		{}

		void init(double sr, double millis) 
		{
			a = exp(-double_Pi / (millis * 0.001 * sr));
			b = 1.0 - a;
			z = 0.0;
		}

		void reset()
		{
			a = 0.0;
			b = 0.0;
			z = 0.0;
		}

		inline double processSmooth(double val)
		{
			z = (val * b) + (z * a);
			return z;
		}

        
        
	private:
		double a, b, z;
		double coeff;
	};
    
	float linToLog(float start, float end, float linVal) { return std::pow(10.0f, (std::log10(end / start) * linVal + std::log10(start))); };
	float logToLin(float start, float end, float logVal) { return (std::log10(logVal / start) / std::log10(end / start)); };

    inline float gainToDb(float gain, float min)    { return gain > 0.0f ? 20.0f * std::log10(gain) : min; };
    inline float dbToGain(float dB, float min)      { return dB > min ? std::pow(10.0f, dB * 0.05f) : 0.0; }

	template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, std::unique_ptr<dsp::Oversampling<FloatType>>& os);

    template <typename FloatType>
    void applyGain (AudioBuffer<FloatType>& buffer);

    template <typename FloatType>
    void applyFilterEnvelope (AudioBuffer<FloatType>& buffer);

    template <typename FloatType>
    void applyFilter (AudioBuffer<FloatType>& buffer, std::unique_ptr<LadderFilterBase> filter[]);

	template <typename FloatType>
	void applyWaveshaper(AudioBuffer<FloatType>& buffer);

    template <typename FloatType>
    void applyAmpEnvelope (AudioBuffer<FloatType>& buffer);
    
	template <typename FloatType>
	void applySequencer(AudioBuffer<FloatType>& buffer);
    

	double getWaveshaped(double sample, double overdrive, int mode)
    {
        
        // tanh
        if (mode == 0)
            return dsp::FastMathApproximations::tanh(overdrive * sample);
        
        
        //Arraya
        return (3.0 * (sample * overdrive) / 2) * (1 - ((sample * overdrive) * (sample * overdrive)) / 3.0);
        
    };

    
	double getLFOSyncedFreq(AudioPlayHead::CurrentPositionInfo posInfo, double division );
    
    void resetSamplerates(double sr);
    void setOversampleQuality(int q);
    
   
    
    std::unique_ptr<dsp::Oversampling<float>> oversamplingFloat;
	std::unique_ptr<dsp::Oversampling<double>> oversamplingDouble;
    
    std::unique_ptr<dsp::Oversampling<float>> oversamplingFloatHQ;
    std::unique_ptr<dsp::Oversampling<double>> oversamplingDoubleHQ;
    
    bool hqOversampling = false;
    bool prevHqOversampling = false;

    Synthesiser synth;
    
	bool noteIsPlaying = false;
    
   // ScopedPointer<LadderFilterBase> filterA[2], filterB[2], filterC[2];
	
    
    enum modTarget {
        modPitch,
        modCutoff,
        off,
    };
    
    void applyModToTarget(int target, double amount);

    std::unique_ptr<ADSR> filterEnvelope;
    std::unique_ptr<ADSR> ampEnvelope;
    LFO lfo;
    
    double modAmount;
    
    int mNumVoices;
    int lastChosenFilter;

    void initialiseSynth();
    void updateCurrentTimeInfoFromHost();

	template <typename FloatType>
    void updateParameters(AudioBuffer<FloatType>& buffer);
    
    template <typename FloatType>
    FloatType softClip(FloatType s);
    
    template <typename FloatType>
    void softClipBuffer(AudioBuffer<FloatType>& buffer);

    float contourVelocity;
	double cutoffModulationAmt; 
    
    double currentCutoff = 40.0, prevCutoff = 40.0;
    
    double sampleRate;
    
	LinearSmoothedValue<double> cutoff, resonance, drive, envGain, switchGain, pulsewidthSmooth1, pulsewidthSmooth2, pulsewidthSmooth3;
	LinearSmoothedValue<double> cutoffFromEnvelope, saturationAmount;
	
	double cutoffRampTimeDefault = 0.0002, cutoffRampTime;
	
    
    int lastNotePlayed;
	int curMidiChannel;
    
    double masterGain = 0.0, masterGainPrev = 0.0;
    
    bool filterOn = true;
    
	std::unique_ptr<SmoothParam> smoothing[6];
    
    std::unique_ptr<LadderFilterBase> filterA[2], filterB[2], filterC[2];
    std::unique_ptr<PulseClock> pulseClock;
    int stepCounter = 0;
	
    
    static BusesProperties getBusesProperties();
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonosynthPluginAudioProcessor)
};
