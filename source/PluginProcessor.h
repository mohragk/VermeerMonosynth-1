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

#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H


#include "../JuceLibraryCode/JuceHeader.h"

#include "adsr/ADSR.h"
#include "lfo.h"
#include "SequencerState.h"
#include "SequencerProcessor.h"
#include "ParamSmoother.h"
#include "TriggeredScope.h"
#include "Arpeggiator.h"


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
        if (*useHQOversamplingParam)
            process (buffer, midiMessages, oversamplingFloatHQ);
        else
            process (buffer, midiMessages, oversamplingFloat);
    }
    
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {

        /*
        jassert (! isUsingDoublePrecision());
        if (*useHQOversamplingParam)
            process (buffer, midiMessages, oversamplingDoubleHQ);
        else
            process (buffer, midiMessages, oversamplingDouble);
         */
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

	bool isSequencerPlaying();
    
    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages

    MidiKeyboardState keyboardState;

    std::unique_ptr<SequencerState> seqState;
    
	TriggeredScope scope;

    
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    int lastSeqPitchValue[8] = {0,0,0,0,0,0,0,0};
    int lastSeqNoteLength = 50;
    int lastSeqDivision = 4;
    
    bool lastOversampleChoice = false;
    bool lastSequencerChoice = false;
    
	bool noteIsBeingPlayed();
    
    bool saturationOn();

	bool lfoSynced();
    

	//@Cleanup: unused?
    SynthesiserVoice* getSynthesiserVoice() { return synth.getVoice(0) ; };
      
    
    // Our parameters
    AudioParameterFloat* gainParam;
    
    AudioParameterFloat* oscGainParam[3];
    AudioParameterFloat* oscDetuneAmountParam[3];
    AudioParameterInt*   oscModeParam[3];
    AudioParameterInt*   oscOffsetParam[3];
    AudioParameterFloat* pulsewidthParam[3];
    AudioParameterFloat* pulsewidthAmountParam[3];
   
    
    AudioParameterInt* oscSyncParam;
    
    AudioParameterFloat* filterCutoffParam;
    AudioParameterFloat* filterQParam;
    AudioParameterFloat* filterContourParam;
    AudioParameterFloat* filterDriveParam;


    AudioParameterFloat* pitchModParam;

    

    
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
    

	

	AudioParameterFloat* saturationParam;
    AudioParameterInt* waveshapeSwitchParam;
    AudioParameterInt* waveshapeModeParam;
    
    AudioParameterInt* oversampleSwitchParam;
    AudioParameterInt* softClipSwitchParam;
    
    AudioParameterInt* stepPitchParam[8];
	AudioParameterFloat* stepNoteLengthParam;
    AudioParameterFloat* stepDivisionFloatParam;
    AudioParameterInt* maxStepsParam;
    
    
    AudioParameterInt* useSequencerParam;
    AudioParameterInt* useHQOversamplingParam;

	AudioParameterBool* useFilterKeyFollowParam;
	

	AudioParameterFloat* arpeggioNoteLengthParam;
	AudioParameterBool*  arpeggioUseParam;


	String debugInfo;
private:
	
	
    Arpeggiator arp;
    
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
    void applyFilter (AudioBuffer<FloatType>& buffer, LadderFilterBase* filter);

	template <typename FloatType>
	void applyWaveshaper(AudioBuffer<FloatType>& buffer);

    template <typename FloatType>
    void updateOscilloscope(AudioBuffer<FloatType>& buffer);
    
	
    dsp::LookupTableTransform<double> tanhLUT   { [](double x) { return std::tanh(x); }, double(-5), double(5), 128 };
    dsp::LookupTableTransform<double> arrayaLUT { [](double x) { return (3.0 * x / 2.0) * (1 - x * x / 3.0); }, double(-5), double(5), 128 };
    

	double getWaveshaped(double sample, double overdrive, int mode)
    {
        
        // tanh
        if (mode == 0)
            return tanhLUT(overdrive * sample);
        
        
        //Arraya
        return arrayaLUT(overdrive * sample);
        
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
	bool filterKeyFollow = true; //TEST

    Synthesiser synth;
    
    
    enum modTarget {
        modPitch,
        modCutoff,
        off,
    };
    
    void applyModToTarget(int target, double amount);

    
    LFO lfo;
    
    double modAmount;
    
    int mNumVoices;

    void initialiseSynth();
    void updateCurrentTimeInfoFromHost();

	template <typename FloatType>
    void updateParameters(AudioBuffer<FloatType>& buffer);
    
    template <typename FloatType>
    FloatType softClip(FloatType s);
    
    template <typename FloatType>
    void softClipBuffer(AudioBuffer<FloatType>& buffer);

    float  contourVelocity;
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
    
	enum smootherForParam {
		CUTOFF_SMOOTHER,
		KEY_CUTOFF_SMOOTHER,
		PW_1_SMOOTHER,
		PW_2_SMOOTHER,
		PW_3_SMOOTHER,
		numSmoothers

	};

	std::unique_ptr<ParamSmoother> smoothing[numSmoothers];
    
    std::unique_ptr<LadderFilterBase> filterA, filterB, filterC;
    
   
    
    
    static BusesProperties getBusesProperties();
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonosynthPluginAudioProcessor)
};

#endif // PLUGIN_PROCESSOR_H
