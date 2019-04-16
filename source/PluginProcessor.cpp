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

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "adsr/ADSR.h"
#include <math.h>
#include <iostream>

#define MIN_INFINITY_DB -96.0f
#define CUTOFF_MIN 40.0f
#define CUTOFF_MAX 20000.0f

AudioProcessor* JUCE_CALLTYPE createPluginFilter();

struct defaultValues {
	// oscillators
	double oscGain[3]     = { Decibels::decibelsToGain(-6.0), Decibels::decibelsToGain(-6.0),Decibels::decibelsToGain(-6.0) };
	double oscDetune[3]   = { 0, -0.1, 0.1 };
	int oscType[3]        = { 1, 1, 1 };
	int oscOffset[3]      = { -24, -12, -8 };

		
};
//==============================================================================
MonosynthPluginAudioProcessor::MonosynthPluginAudioProcessor()
: AudioProcessor ( getBusesProperties() ),
lastUIWidth (72 * 8 + 24 * 10),
lastUIHeight (48 + 460 + 140),

gainParam (nullptr),

oscSyncParam(nullptr),

filterCutoffParam(nullptr),
filterQParam(nullptr),
filterContourParam(nullptr),
filterDriveParam(nullptr),

pitchModParam(nullptr),



attackParam1(nullptr),
decayParam1(nullptr),
sustainParam1(nullptr),
releaseParam1(nullptr),
attackCurve1Param(nullptr),
decayRelCurve1Param(nullptr),

attackParam2(nullptr),
decayParam2(nullptr),
sustainParam2(nullptr),
releaseParam2(nullptr),
attackCurve2Param(nullptr),
decayRelCurve2Param(nullptr),

attackParam3(nullptr),
decayParam3(nullptr),
sustainParam3(nullptr),
releaseParam3(nullptr),
attackCurve3Param(nullptr),
decayRelCurve3Param(nullptr),

modTargetParam(nullptr),

lfoRateParam(nullptr),
lfoModeParam(nullptr),
lfoIntensityParam(nullptr),
lfoSyncParam(nullptr),

filterSelectParam(nullptr),
lfoDivisionParam(nullptr),

saturationParam(nullptr),
waveshapeSwitchParam(nullptr),
waveshapeModeParam(nullptr),


overSampleParam(nullptr),
oversampleSwitchParam(nullptr),

softClipSwitchParam(nullptr),

stepNoteLengthParam(nullptr),
stepDivisionFloatParam(nullptr),
maxStepsParam(nullptr),
swingParam(nullptr),

useSequencerParam(nullptr),
useHQOversamplingParam(nullptr),

useFilterKeyFollowParam(nullptr),


arpeggioNoteLengthParam(nullptr),
arpeggioUseParam(nullptr),

glideTimeParam(nullptr),

skipChorusParam(nullptr)



{
    lastPosInfo.resetToDefault();


	defaultValues defaultVals;
    
    // This creates our parameters. We'll keep some raw pointers to them in this class,
    // so that we can easily access them later, but the base class will take care of
    // deleting them for us.

	auto decibelToGainLambda = [](auto min, auto end, auto dB)  { return dB > min ? std::pow(10.0f, dB * 0.05f) / Decibels::decibelsToGain(end) : 0.0; };
	//auto gainToDecibelLambda = [](auto min, auto end, auto gain){ return gain > 0.0f ? 20.0f * std::log10(gain * Decibels::decibelsToGain(end)) : -std::numeric_limits<float>::infinity(); };
    
    auto gainToDecibelLambda = [](auto min, auto end, auto gain) { return gain > 0.0f ? 20.0f * std::log10(gain * Decibels::decibelsToGain(end) ) : -std::numeric_limits<float>::infinity(); };
	
	auto linToLogLambda = [](float start, float end, float linVal) { 
		float result = std::pow(10.0f, (std::log10(end / start) * linVal + std::log10(start)));
		if (result > end)
			result = end;
		if (result < start)
			result = start;
		return result;
	};
	auto logToLinLambda = [](float start, float end, float logVal) { 
		float result =  (std::log10(logVal / start) / std::log10(end / start));
		if (result > 1.0)
			result = 1.0;
		if (result < 0.0)
			result = 0.0;
		return result;
	};


    
	

    addParameter (gainParam = new AudioParameterFloat("volume", "Volume" , NormalisableRange<float>(MIN_INFINITY_DB, 6.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
	//*gainParam = 6.0f;
   
    
    
    for (int osc = 0; osc < 3; osc++)
    {
        //addParameter (oscGainParam[osc]  = new AudioParameterFloat ("oscGain" + std::to_string(osc),  "OSC" + std::to_string(osc + 1) + " Gain", NormalisableRange<float>(MIN_INFINITY_DB, 0.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
        addParameter (oscGainParam[osc]  = new AudioParameterFloat ("oscGain" + std::to_string(osc),  "OSC" + std::to_string(osc + 1) + " Gain", NormalisableRange<float>(0.0f, 1.0f, 0.0f, 0.5f, false), defaultVals.oscGain[osc]));
        addParameter (oscDetuneAmountParam[osc] = new AudioParameterFloat("oscDetuneAmount" + std::to_string(osc), "OSC" + std::to_string(osc + 1) + " Tune", NormalisableRange<float>(-0.5f, 0.5f, 0.0f), defaultVals.oscDetune[osc]));
        addParameter (oscModeParam[osc] = new AudioParameterInt("oscModeChoice" + std::to_string(osc), "OSC" + std::to_string(osc + 1) + " Waveform", 0, 3, defaultVals.oscType[osc]));
        addParameter (oscOffsetParam[osc] = new AudioParameterInt("oscOffset" + std::to_string(osc), "OSC"+std::to_string(osc + 1)+" Offset", -24, 24, defaultVals.oscOffset[osc]));
        
        // PWM
        addParameter(pulsewidthParam[osc] = new AudioParameterFloat("pulsewidthParam" + std::to_string(osc), "PW"+std::to_string(osc + 1), 0.1f, 1.0f, 0.1f));
        addParameter(pulsewidthAmountParam[osc] = new AudioParameterFloat("pulsewidthAmountParam" + std::to_string(osc), "PWM" +std::to_string(osc + 1)+" Amt", 0.0f, 1.0f, 0.0f));
    }
    
    
    addParameter (pitchModParam = new AudioParameterFloat("pitchMod", "Pitch Modulation", NormalisableRange<float> (0.0f, 2000.0f, 0.0f, 0.5f, false), 0.0f));
    addParameter (oscSyncParam = new AudioParameterInt("oscSync", "osc2>osc1 sync", 0, 1, 0));
    
    
    
	NormalisableRange<float> cutoffRange = NormalisableRange<float>(CUTOFF_MIN, CUTOFF_MAX, linToLogLambda, logToLinLambda);
	//NormalisableRange<float> cutoffRange = NormalisableRange<float>(CUTOFF_MIN, CUTOFF_MAX, 0.0f, 0.25f, false);
	NormalisableRange<float> contourRange = NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f, false);
	//NormalisableRange<float> contourRange = NormalisableRange<float>(CUTOFF_MIN, CUTOFF_MAX, linToLogLambda, logToLinLambda);
	//NormalisableRange<float> contourRange = NormalisableRange<float>(CUTOFF_MIN, CUTOFF_MAX, 0.0f, 0.25f, false);
    
	addParameter (filterCutoffParam = new AudioParameterFloat("filter", "Filter Cutoff", cutoffRange, CUTOFF_MAX));

	addParameter (filterQParam = new AudioParameterFloat("filterQ", "Filter Reso.",                 NormalisableRange<float> (0.0f, 1.0f, 0.0f, 1.0f, false), 0.0f));
    addParameter (filterContourParam = new AudioParameterFloat("filterContour", "Filter Contour",	contourRange, CUTOFF_MIN));

	addParameter (filterDriveParam = new AudioParameterFloat("filterDrive", "Filter Drive",         NormalisableRange<float> (1.0f, 5.0f, 0.0f, 1.0f, false), 1.0f));
    
    // Filter Select Parameter
    addParameter (filterSelectParam = new AudioParameterInt("filterSelect", "Switch Filter", 0, 2, 2));
	addParameter (useFilterKeyFollowParam = new AudioParameterBool("useFilterKeyFollowParam", "Key follow", false));

    
    
    
    
    //ENV 1
    addParameter (attackParam1 = new AudioParameterFloat ("attack1", "Amp Attack",       NormalisableRange<float>(0.001f, 11.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter (decayParam1  = new AudioParameterFloat ("decay1", "Amp Decay",         NormalisableRange<float>(0.001f, 11.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter (sustainParam1  = new AudioParameterFloat ("sustain1", "Amp Sustain",   NormalisableRange<float>(MIN_INFINITY_DB, 0.0f, gainToDecibelLambda, decibelToGainLambda ), -0.75f));
    addParameter (releaseParam1  = new AudioParameterFloat ("release1", "Amp Release",   NormalisableRange<float>(0.001f, 11.0f, 0.0f, 0.5f, false), 0.01f));
    
    addParameter (attackCurve1Param = new AudioParameterFloat("attackCurve1", "Attack Curve",           NormalisableRange<float>(0.001f, 1.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter (decayRelCurve1Param = new AudioParameterFloat("decRelCurve1", "Decay-Release Curve",  NormalisableRange<float>(0.00001f, 1.0f, 0.0f, 0.5f, false), 0.00001f));
    
    //ENV 2
    addParameter (attackParam2 = new AudioParameterFloat ("attack2", "Pitch Attack",     NormalisableRange<float>(0.001f, 11.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter (decayParam2  = new AudioParameterFloat ("decay2", "Pitch Decay",       NormalisableRange<float>(0.0f, 11.0f, 0.0f, 0.5f, false), 0.0f));
    addParameter (sustainParam2  = new AudioParameterFloat ("sustain2", "Pitch Sustain", NormalisableRange<float>(0.0f, 1.0f,  0.0f, 0.5f, false), 0.0f));
    addParameter (releaseParam2  = new AudioParameterFloat ("release2", "Pitch Release", NormalisableRange<float>(0.0f, 11.0f, 0.0f, 0.5f, false), 0.0f));
    
    addParameter(attackCurve2Param = new AudioParameterFloat("attackCurve2", "Attack Curve",			NormalisableRange<float>(0.001f, 1.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter(decayRelCurve2Param = new AudioParameterFloat("decRelCurve2", "Decay-Release Curve",	NormalisableRange<float>(0.00001f, 1.0f, 0.0f, 0.5f, false), 0.00001f));
    
    //ENV 3
	addParameter(attackParam3 = new AudioParameterFloat("attack3", "Filter Attack",		 NormalisableRange<float>(0.001f, 11.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter (decayParam3  = new AudioParameterFloat ("decay3", "Filter Decay",      NormalisableRange<float>(0.0f, 11.0f, 0.0f, 0.5f, false), 0.0f));
    addParameter (sustainParam3  = new AudioParameterFloat ("sustain3", "Filter Sustain",NormalisableRange<float>(0.0f, 1.0f,  0.0f, 0.5f, false), 0.0f));
    addParameter (releaseParam3  = new AudioParameterFloat ("release3", "Filter Release",NormalisableRange<float>(0.01f, 11.0f, 0.0f, 0.5f, false), 0.01f));
    
    addParameter(attackCurve3Param = new AudioParameterFloat("attackCurve3", "Attack Curve", NormalisableRange<float>(0.001f, 1.0f, 0.0f, 0.5f, false), 0.001f));
    addParameter(decayRelCurve3Param = new AudioParameterFloat("decRelCurve3", "Decay-Release Curve", NormalisableRange<float>(0.00001f, 1.0f, 0.0f, 0.5f, false), 0.00001f));
    
    
    addParameter(envelopeRetriggerParam = new AudioParameterBool("envelopeRetrigger", "Retrigger Envelopes", false));
    
    // Modulation
    addParameter(modTargetParam = new AudioParameterInt("modTarget", "Modulation Target", 0, 2, 2));
    
    
    // LFO
    addParameter(lfoRateParam = new AudioParameterFloat("lfoRate", "LFO Rate", NormalisableRange<float>(0.01f, 30.0f, 0.0f, 0.5f, false), 0.05f));
    addParameter(lfoModeParam = new AudioParameterInt ("lfoMode", "LFO Mode", 0, 2, 0));
    addParameter(lfoIntensityParam = new AudioParameterFloat("lfoIntensity", "LFO Strength", NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f, false), 0.0f));
    addParameter(lfoSyncParam = new AudioParameterInt("lfoSync", "LFO Tempo Sync", 0, 1, 0));
    addParameter(lfoDivisionParam = new AudioParameterInt("lfoDivision", "LFO Synced Rate", 1, 6, 2));
    
    
    addParameter(overSampleParam = new AudioParameterInt("overSampleParam", "Oversampling Switch", 0, 1, 1));
    addParameter(filterOrderParam = new AudioParameterInt("filterOrderParam", "Filter Order", 0, 1, 0)); // 0 = VCA->filter; 1 = filter->VCA
    
   

	//Saturation/Overdrive
	addParameter(saturationParam = new AudioParameterFloat("saturationParam", "Saturation", 1.0f, 5.0f, 1.0f));
    addParameter(waveshapeSwitchParam = new AudioParameterInt("waveshapeSwitchParam", "Waveshaping ON/OFF", 0, 1, 0));
    addParameter(waveshapeModeParam = new AudioParameterInt("waveshapeModeParam", "Waveshape", 0, 1, 0));
    
    // Oversampling HQ switch
    addParameter(oversampleSwitchParam = new AudioParameterInt("oversampleSwitchParam", "HQ ON/OFF", 0, 1, 0));
    addParameter(useHQOversamplingParam = new AudioParameterInt("useHQOversamplingParam", "Use HQ Oversampling", 0, 1, 0 ));
    
    // Softclip switch
    addParameter(softClipSwitchParam = new AudioParameterInt("softClipSwitchParam", "Softclip ON/OFF", 0, 1, 0));
    
    // SEQUENCER PARAMS
    addParameter(useSequencerParam = new AudioParameterInt( "useSequencerParam", "Use Sequencer", 0, 1 ,0 ));
    
    for (int i = 0; i < 8; i++)
    {
        addParameter(stepPitchParam[i] = new AudioParameterInt("stepPitchParam" + std::to_string(i), "Seq. Pitch " + std::to_string(i), -12, 12, 0 ));
        addParameter(stepPlayParam[i] = new AudioParameterBool("stepPlayParam" + std::to_string(i), "Seq. On " + std::to_string(i), true));
    }
    
    auto Log2_int = [](unsigned int N)
    {
        unsigned int bits = sizeof(N) * 4;
        unsigned int n = 0;
        while (N > 1)
        {
            if (N >> bits)
            {
                N >>= bits;
                n += bits;
            }
            bits >>= 1;
        }
        return n;
    };
    
    auto linToPow = [](float start, float end, float linVal) {
        double remapped = (6.0 - 1.0) * (linVal - 0.0) / (1.0 - 0.0) + 1.0 ; // remap
        
        int newValue = std::round(remapped);
        return std::round( 1 << newValue );
        
    };
    auto powToLin = [&](float start, float end, float powVal) {
        uint16 newValue = (int) powVal;
        double invLog = Log2_int( newValue );
        return (1.0 - 0.0) * (invLog - 1.0) / (6.0 - 1.0) + 0.0; //remap
        
        
    };
    
	addParameter(stepNoteLengthParam = new AudioParameterFloat("stepNoteLengthParam", "Seq. Note Length" , 0.05f, 0.95f, 0.5f));
    addParameter(stepDivisionFloatParam   = new AudioParameterFloat("stepDivisionFloatParam", "Seq. Rate", NormalisableRange<float>(2.0, 64.0, linToPow, powToLin) , 16.0));
    
    addParameter(maxStepsParam = new AudioParameterInt ("maxStepsParam", "Max. Steps", 0, 7, 7 ));
	addParameter(swingParam = new AudioParameterInt("swingParam", "Swing Amount", 50, 85, 50));

	//ARPEGGIATOR
	addParameter(arpeggioNoteLengthParam = new AudioParameterFloat("apreggioNoteLengthParam", "Arpeggio Rate", NormalisableRange<float>(2.0, 64.0, linToPow, powToLin), 16.0));
	addParameter(arpeggioUseParam = new AudioParameterBool("arpeggioUseParam", "Arpeggio ON/OFF", false));
    
    
   
    //GLIDE
    addParameter(glideTimeParam = new AudioParameterFloat("glideTimeParam", "Glide Time", NormalisableRange<float>(0.0, 5000.0, 0.0 , 0.25 ), 0.0));
    


	//CHORUS
	addParameter(skipChorusParam = new AudioParameterInt("skipChorusParam", "Chorus On/Off", 0, 1, 1));

    initialiseSynth();
    
    keyboardState.addListener(this);
    synth.addListener(this);
    
    
    
    filterA.reset( new ImprovedMoog );
    filterB.reset( new ThreeFiveModel );
    filterC.reset( new DiodeLadderModel );
    

    for( int i = 0; i < 3; i++)
        envelopeGenerator[i].reset(new ADSR);
    
    // Oversampling 2 times with FIR filtering
    oversamplingFloat.reset  ( new dsp::Oversampling<float>    ( 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR , false ) );
    oversamplingDouble.reset ( new dsp::Oversampling<double>   ( 2, 1, dsp::Oversampling<double>::filterHalfBandPolyphaseIIR , false ) );
    
    // HQ Oversampling 8 times with FIR filtering
    oversamplingFloatHQ.reset  ( new dsp::Oversampling<float>  ( 2, 3, dsp::Oversampling<float>::filterHalfBandFIREquiripple , true ) );
    oversamplingDoubleHQ.reset ( new dsp::Oversampling<double> ( 2, 3, dsp::Oversampling<double>::filterHalfBandFIREquiripple , true ) );
    

    for (int i = 0; i < numSmoothers; i++)
        smoothing[i].reset(new ParamSmoother);
    
   // sequencerProcessor = std::unique_ptr<SequencerProcessor> ( new SequencerProcessor( keyboardState ) );
	seqState.reset(new SequencerState);
    
    for(int i = 0; i < 3; i++)
    {
        LinearSmoothedValue<double> smval;
        gainSmoothed.push_back(smval);
    }
    

	loadDefaultState();
    
}

MonosynthPluginAudioProcessor::~MonosynthPluginAudioProcessor()
{
    keyboardState.removeListener(this);
    synth.removeListener(this);
    synth.clearSounds();
    synth.clearVoices();
    
    currentPlayedNotes.clear();
    gainSmoothed.clear();
    
    
}


void MonosynthPluginAudioProcessor::initialiseSynth()
{
    synth.addVoice (new MonosynthVoice());
    synth.addSound (new MonosynthSound());
}


void MonosynthPluginAudioProcessor::isSynthesiserPlaying(Monosynthesiser* source, bool isPlaying)
{
    
}

void MonosynthPluginAudioProcessor::handleSynthNoteOn   (Monosynthesiser* source, int midiChannel, int midiNoteNumber)
{
    for (int i = 0; i < 3; i++)
    {
        envelopeGenerator[i].get()->gate(true);
        
        if (*envelopeRetriggerParam)
            envelopeGenerator[i].get()->reTrigger();
    }
    
    
    lfo.setPhase(0.0);
    
    lastNotePlayed = midiNoteNumber;
    
   
    
    if(*useSequencerParam == false)
    {
        currentPlayedNotes.addIfNotAlreadyThere(midiNoteNumber);
    }
    
}

void MonosynthPluginAudioProcessor::handleSynthNoteOff   (Monosynthesiser* source, int midiChannel, int midiNoteNumber)
{
    if(*useSequencerParam == false)
    {
        if (currentPlayedNotes.size() <= 1)
            for (int i = 0; i < 3; i++)
                envelopeGenerator[i].get()->gate(false);
        
        currentPlayedNotes.removeFirstMatchingValue(midiNoteNumber);
    }
    else
    {
        for (int i = 0; i < 3; i++)
            envelopeGenerator[i].get()->gate(false);
    }
}

//==============================================================================
bool MonosynthPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    
    
    // do not allow disabling the main buses
	if ( layouts.getMainInputChannelSet() == AudioChannelSet::disabled()
		 || layouts.getMainOutputChannelSet() == AudioChannelSet::disabled() )
		return false;

	// only allow stereo and mono
	if ( layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo() )
		return false;
    

    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

AudioProcessor::BusesProperties MonosynthPluginAudioProcessor::getBusesProperties()
{
    return BusesProperties()
		.withInput  ("Input", AudioChannelSet::stereo(), true)
		.withOutput ("Output", AudioChannelSet::stereo(), true);
}

//==============================================================================
void MonosynthPluginAudioProcessor::prepareToPlay (double newSampleRate, int samplesPerBlock)
{
	oversamplingFloat->initProcessing(samplesPerBlock);
	oversamplingDouble->initProcessing(samplesPerBlock);
    oversamplingFloatHQ->initProcessing(samplesPerBlock);
    oversamplingDoubleHQ->initProcessing(samplesPerBlock);
    
    int newBlockSize = (int)oversamplingFloat->getOversamplingFactor() * samplesPerBlock;
    filterA->Prepare(newSampleRate, newBlockSize);
    filterB->Prepare(newSampleRate, newBlockSize);
    filterC->Prepare(newSampleRate, newBlockSize);

	chorusEffect.prepareToPlay(newSampleRate, newBlockSize);
    
    resetSamplerates(newSampleRate, samplesPerBlock);
    
    keyboardState.reset();
    currentPlayedNotes.clear();

    lastNotePlayed = 60;
}

void MonosynthPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();

    
    oversamplingFloat->reset();
    oversamplingDouble->reset();
    oversamplingFloatHQ->reset();
    oversamplingDoubleHQ->reset();
    
    
   
    filterA->Reset();
    filterB->Reset();
    filterC->Reset();
    

    for (int i = 0; i < numSmoothers; i++)
        smoothing[i]->reset();
}

void MonosynthPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.

    cutoff.reset             (sampleRate, cutoffRampTimeDefault);
    cutoffFromEnvelope.reset (sampleRate, cutoffRampTimeDefault);
    resonance.reset          (sampleRate, 0.001);
    drive.reset              (sampleRate, 0.001);
    pulsewidthSmooth1.reset  (sampleRate, cutoffRampTimeDefault);
    pulsewidthSmooth2.reset  (sampleRate, cutoffRampTimeDefault);
    pulsewidthSmooth3.reset  (sampleRate, cutoffRampTimeDefault);
	saturationAmount.reset   (sampleRate, cutoffRampTimeDefault);
    
    oversamplingFloat->reset();
    oversamplingDouble->reset();
    oversamplingFloatHQ->reset();
    oversamplingDoubleHQ->reset();
    
    
    filterA->Reset();
    filterB->Reset();
    filterC->Reset();
	
    for (int i = 0; i < numSmoothers; i++)
        smoothing[i]->reset();


    currentPlayedNotes.clear();
}




void MonosynthPluginAudioProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
   
    
    
    
    contourVelocity = velocity;
    
}

void MonosynthPluginAudioProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    
    
}





void MonosynthPluginAudioProcessor::resetSamplerates(const double sr, int bufferSize)
{
    double newsr = sr;
    int newBufferSize = bufferSize;
    
    if(*useHQOversamplingParam)
    {
        newsr *= oversamplingDoubleHQ->getOversamplingFactor();
        newBufferSize *= oversamplingDoubleHQ->getOversamplingFactor();
        if ( isUsingDoublePrecision() ) { setLatencySamples(roundToInt(oversamplingDoubleHQ->getLatencyInSamples())); }
        else                            { setLatencySamples(roundToInt(oversamplingFloatHQ->getLatencyInSamples())); }
    }
    else
    {
        newsr *= oversamplingDouble->getOversamplingFactor();
        newBufferSize *= oversamplingDoubleHQ->getOversamplingFactor();
        if ( isUsingDoublePrecision() ) { setLatencySamples(roundToInt(oversamplingDouble->getLatencyInSamples())); }
        else                            { setLatencySamples(roundToInt(oversamplingFloat->getLatencyInSamples())); }
    }
    
    
    
    synth.setCurrentPlaybackSampleRate (newsr);
    MonosynthVoice* synthVoice = dynamic_cast<MonosynthVoice*>(synth.getVoice(0));
	synthVoice->setEnvelopeSampleRate(newsr);
    
	seqState.get()->prepareToPlay(newsr);
	arp.prepareToPlay(newsr, 0);
    
    filterA->SetSampleRate(newsr);
    filterB->SetSampleRate(newsr);
    filterC->SetSampleRate(newsr);
   
    filterA->UpdateBufferSize(newBufferSize);
    filterB->UpdateBufferSize(newBufferSize);
    filterC->UpdateBufferSize(newBufferSize);

    
    
    for (int i = 0; i < 3; i++)
        envelopeGenerator[i].get()->setSampleRate(newsr);
    
    lfo.setSampleRate(newsr);
    
    float rampTime = (float)newBufferSize / newsr; // set time to buffersize time
    
    cutoff.reset(newsr, rampTime);
    contour.reset(newsr, rampTime);
    cutoffFromEnvelope.reset(newsr, rampTime);
    resonance.reset(newsr, rampTime);
    drive.reset(newsr, rampTime);
    
    pulsewidthSmooth1.reset(newsr, rampTime);
    pulsewidthSmooth2.reset(newsr, rampTime);
    pulsewidthSmooth3.reset(newsr, rampTime);
    
    for(int i = 0; i < 3; i++)
    {
        gainSmoothed[i].reset(newsr, rampTime);
    }

   
    
    for (int i = 0; i < numSmoothers; i++)
        smoothing[i]->init(newsr, 2.0);

	smoothing[CUTOFF_SMOOTHER]->init(newsr, 4.0);
    smoothing[CONTOUR_SMOOTHER]->init(newsr, 2.0);
    smoothing[KEY_CUTOFF_SMOOTHER]->init(newsr, 10.0);

    sampleRate = newsr;
}

void MonosynthPluginAudioProcessor::setOversampleQuality(int q = 0)
{
    hqOversampling = bool(q);
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, dsp::Oversampling<FloatType>* oversampling)
{
    
    bool hqOn = bool(*useHQOversamplingParam);
   
    if (prevHqOversampling != hqOn)
    {
        resetSamplerates( getSampleRate(), buffer.getNumSamples() );
        prevHqOversampling = hqOn;
    }


    
    // OVERSAMPLING
    dsp::AudioBlock<FloatType> block (buffer);
    dsp::AudioBlock<FloatType> osBlock;
    
    osBlock = oversampling->processSamplesUp(block);
    
    FloatType* const arr[] = { osBlock.getChannelPointer(0), osBlock.getChannelPointer(1) };
    
    AudioBuffer<FloatType> osBuffer(
                                    arr,
                                    2,
                                    static_cast<int> ( osBlock.getNumSamples() )
                                    );
    
    
    const int numSamples = osBuffer.getNumSamples();

    // Clear the buffer of any samples
    //osBuffer.clear();
    
    int chunkSize = jmin(256, numSamples);
    int position = 0;
    int samplesRemaining = numSamples;
    int numSamplesChunk = chunkSize;
    
    

    for ( ; position < numSamples; position += chunkSize )
    {
        
        AudioBuffer<FloatType> chunkBuffer (1, numSamplesChunk);
        MidiBuffer chunkMidi;
        
        chunkBuffer.copyFrom(0, 0, osBuffer, 0, position, numSamplesChunk);
        chunkMidi.addEvents(midiMessages, position, numSamplesChunk, 0);
        
        
        // PARAMETER UPDATE
        updateParameters(chunkBuffer);

    
        // KEYBOARD
        keyboardState.processNextMidiBuffer(chunkMidi, 0, numSamplesChunk, true);
    
    
        // SEQUENCER
        seqState.get()->processBuffer(chunkBuffer, chunkMidi, bool(*useSequencerParam));
    
        //ARPEGGIATOR
        arp.process(chunkBuffer, chunkMidi, *arpeggioUseParam, false);

    
        // GET SYNTHDATA
        synth.renderNextBlock (chunkBuffer, chunkMidi, 0, numSamplesChunk);
    
    
		// TEST TEST TEST FILTER
		processFilterBlending(chunkBuffer);
       
    
    
		// APPLY AMP ENVELOPE
		applyAmplitudeEnvelope(chunkBuffer);
    

        // APPLYING WAVESHAPER
        if(*waveshapeSwitchParam == 1)
            applyWaveshaper(chunkBuffer);
    
    
        // In case we have more outputs than inputs, we'll clear any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
            chunkBuffer.clear (i, 0, numSamplesChunk);
    

        // Update oscilloscope data
        updateOscilloscope(chunkBuffer);
    



        // APPLY VOLUME
        applyGain(chunkBuffer); // apply our gain-change to the outgoing data..


        // APPLY SOFTCLIP
        if (*softClipSwitchParam == 1)
            softClipBuffer(chunkBuffer);
        
        
        osBuffer.copyFrom(0, position, chunkBuffer, 0, 0, numSamplesChunk);
        
        samplesRemaining -= chunkSize;
        
        if (samplesRemaining < chunkSize)
            numSamplesChunk = samplesRemaining > 0 ? samplesRemaining : chunkSize;
        
        
    }

	//osBuffer.copyFrom(1, 0, osBuffer, 0, 0, osBuffer.getNumSamples());

 
    // DOWNSAMPLING
    oversampling->processSamplesDown(block);
    
    
    // COPY LEFT DATA to RIGHT CHANNEL
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
   
	


	// CHORUS EFFECT
	bool skip = (bool)*skipChorusParam;
	chorusEffect.processBlock(buffer, skip);
    
    
    meter.setLevel( buffer.getMagnitude( 0, buffer.getNumSamples() ) );
    

    // Now ask the host for the current time so we can store it to be displayed later...
    updateCurrentTimeInfoFromHost();
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyGain(AudioBuffer<FloatType>& buffer)
{

  masterGain = dbToGain(*gainParam, MIN_INFINITY_DB);

	if (masterGain == masterGainPrev)
	{
      buffer.applyGain(0, 0, buffer.getNumSamples(), masterGain);
	}
	else
	{
      buffer.applyGainRamp(0, 0, buffer.getNumSamples(), masterGainPrev, masterGain);
		  masterGainPrev = masterGain;
	}

}

template<typename FloatType>
void MonosynthPluginAudioProcessor::applyAmplitudeEnvelope(AudioBuffer<FloatType>& buffer)
{

	int numSamples = buffer.getNumSamples();

	const FloatType* input = buffer.getReadPointer(0);
	FloatType* dataLeft    = buffer.getWritePointer(0);

	MonosynthVoice* voice = dynamic_cast<MonosynthVoice*>(synth.getVoice(0));

	for (int pos = 0; pos < numSamples; pos++)
	{
		FloatType gain = envelopeGenerator[0].get()->process();

		envelopeLED1.setBrightness((float)gain);
		envelopeLED3.setBrightness((float)voice->getPitchEnvelopeValue());

		dataLeft[pos] = input[pos] * gain;
	}
}




template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilterEnvelope (AudioBuffer<FloatType>& buffer, LadderFilterBase* filter)
{
	
    const int numSamples = buffer.getNumSamples();
    
    for (int i = 0; i < numSamples; i++)
    {
        //
        // LFO
        //
        lfo.setMode(*lfoModeParam);
        
        const double lfo_division = pow(2.0, *lfoDivisionParam);
        
        if (*lfoSyncParam == 1)
            lfo.setFrequency(getLFOSyncedFreq(lastPosInfo, lfo_division)); //*lfoRateParam);
        else
            lfo.setFrequency(*lfoRateParam);
        
        // Retrigger LFO at new bar
        if ( lastPosInfo.isPlaying )
        {
            if (lastPosInfo.ppqPositionOfLastBarStart == lastPosInfo.ppqPosition)
                lfo.setPhase(0.0);
        }
        
        
		// Initial cutoff value mapped to linear space
		double cutoffSmooth = smoothing[CUTOFF_SMOOTHER]->processSmooth(cutoff.getNextValue());
		double linearCutoffVal = logToLin(CUTOFF_MIN, CUTOFF_MAX, cutoffSmooth);


		// LFO modulation
		const double lfoValue = lfo.nextSample();
		modAmount = *lfoIntensityParam;                            // Make parameter
		const double lfoCutoffVal = (lfoValue * modAmount);
		linearCutoffVal += lfoCutoffVal;

		applyModToTarget(*modTargetParam, lfoValue * modAmount); //external
        
		// Envelope modulation
        const double filterEnvelopeAmt = contour.getNextValue(); // LOG from 40 - 18000
        const double filterEnvelopeVal = envelopeGenerator[1].get()->process();
		const double envCutoff = filterEnvelopeVal * filterEnvelopeAmt;
		linearCutoffVal += envCutoff; 

		// SHOULDN'T BE HERE BUT HEY..
		envelopeLED2.setBrightness((float)filterEnvelopeVal);
		

		if (*useFilterKeyFollowParam)
        {
            const double keyFollowCutoff =  MidiMessage::getMidiNoteInHertz ( lastNotePlayed );
			double keyFollowCutOffHz = smoothing[KEY_CUTOFF_SMOOTHER].get()->processSmooth(keyFollowCutoff - CUTOFF_MIN);
			keyFollowCutOffHz = jmax((double)CUTOFF_MIN, jmin((double)CUTOFF_MAX, keyFollowCutOffHz));

			linearCutoffVal += logToLin(
								CUTOFF_MIN, 
								CUTOFF_MAX, 
								keyFollowCutOffHz
								);
        }
		
		currentCutoff = linToLog(CUTOFF_MIN, CUTOFF_MAX, linearCutoffVal);
       
		FloatType combinedCutoff = currentCutoff; // +smoothing[CUTOFF_SMOOTHER]->processSmooth(cutoff.getNextValue());
        
        
        
        if (combinedCutoff > CUTOFF_MAX) combinedCutoff = CUTOFF_MAX;
        if (combinedCutoff < CUTOFF_MIN) combinedCutoff = CUTOFF_MIN;
        
        
        
        auto snapToLocalVal= [](double val) -> double { if (val < 0.0) val = 0.0; else if (val > 1.0) val = 1.0; return val;  };
        
        FloatType newReso =  snapToLocalVal(resonance.getNextValue());
        
        filter->AddModulationValueForParameter(combinedCutoff, LadderFilterBase::CUTOFF, i);
        filter->AddModulationValueForParameter(newReso, LadderFilterBase::RESONANCE, i);
        filter->AddModulationValueForParameter(drive.getNextValue(), LadderFilterBase::DRIVE, i);
		
    }
    
    
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilter(AudioBuffer<FloatType>& buffer, LadderFilterBase* filter)
{

	FloatType* channelDataLeft = buffer.getWritePointer(0);
	const int numSamples = buffer.getNumSamples();


	filter->Process(channelDataLeft, numSamples);
}


bool isBlending = false;
int lastFilterChoice = 0;
float blendOldToNewAmount = 0.0;
int blendTimeRemaining = 0;

template <typename FloatType>
void MonosynthPluginAudioProcessor::processFilterBlending(AudioBuffer<FloatType>& buffer)
{

	int numSamples = buffer.getNumSamples();
	
	AudioBuffer<FloatType> tempFilterBuffer[2];


	int blendTimeSamples = std::round(sampleRate * 0.1); // blend 1/10 of a second

	// APPLY FILTER
	LadderFilterBase* curFilter;

	if (*filterSelectParam == 0)	  curFilter = filterA.get();
	else if (*filterSelectParam == 1) curFilter = filterB.get();
	else                              curFilter = filterC.get();

	for (int filter = 0; filter < 2; filter++) {
		tempFilterBuffer[filter].setSize(1, numSamples);
		tempFilterBuffer[filter].copyFrom(0, 0, buffer, 0,0, numSamples);
		applyFilterEnvelope(tempFilterBuffer[filter], curFilter);
		applyFilter(tempFilterBuffer[filter], curFilter);
	}
	
	// BLEND 2 filters
	buffer.copyFrom(0,0, tempFilterBuffer[0], 0, 0, numSamples);

}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyWaveshaper(AudioBuffer<FloatType>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const FloatType* readLeft = buffer.getReadPointer(0);
	  FloatType* dataL = buffer.getWritePointer(0);


	  auto saturation = saturationAmount.getNextValue();

	  for (int i = 0; i < numSamples; i++)
	  {
		    dataL[i] = getWaveshaped(readLeft[i], saturation, *waveshapeModeParam);
	  }
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::updateOscilloscope(AudioBuffer<FloatType>& buffer)
{
 
    float satuNorm = (*saturationParam - 1.0f) / 4.0f;
    float zoomFactor = 1.0f - ( satuNorm * satuNorm ) ;
    scope.setVerticalZoomFactor( jlimit(0.5f, 1.0f, zoomFactor) );
    scope.addSamples((float*)buffer.getReadPointer(0), buffer.getNumSamples());
}


void MonosynthPluginAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (AudioPlayHead* ph = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo newTime;
        
        if (ph->getCurrentPosition (newTime))
        {
            lastPosInfo = newTime;  // Successfully got the current time from the host..
            return;
        }
    }
    
    // If the host fails to provide the current time, we'll just reset our copy to a default..
    lastPosInfo.resetToDefault();
}

//==============================================================================
AudioProcessorEditor* MonosynthPluginAudioProcessor::createEditor()
{
    return new MonosynthPluginAudioProcessorEditor (*this) ;
}


void MonosynthPluginAudioProcessor::loadDefaultState()
{
//#if JUCE_MODAL_LOOPS_PERMITTED

	//File path ("C:/DEVELOPMENT/VermeerMonosynth-1/presets/default");
	//path.withFileExtension("");

	File path;
    
    if (isWindows())
        path = (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory). getChildFile("MRVR"). getChildFile("Monosynth-1"). getChildFile("Presets"). getChildFile("default")  );
    else
        path = (File::getSpecialLocation(File::SpecialLocationType::commonDocumentsDirectory). getChildFile("MRVR"). getChildFile("Monosynth-1"). getChildFile("Presets"). getChildFile("default")  );

    
    path.withFileExtension("");
    
	MemoryBlock data;

	//path.loadFileAsData(data);

	if (path.exists())
	{
		FileInputStream inputStream(path);


		if (inputStream.openedOk())
		{
			inputStream.readIntoMemoryBlock(data, -1);
			setStateInformation(data.getData(), (int)data.getSize());
		}
		else
		{
			Result r = inputStream.getStatus();
			String error = r.getErrorMessage();
			AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
				TRANS("Error whilst loading:"),
				TRANS(error));
		}
	}
	else
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			TRANS("Error whilst loading:"),
			TRANS("Default preset not found"));
	}

	
//#else
//	ignoreUnused(fileSuffix);
//#endif
}
//==============================================================================
void MonosynthPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:
    
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    
    
    
    // Store the values of all our parameters, using their param ID as the XML attribute
    //for (int i = 0; i < getNumParameters(); ++i)
	for (int i = 0; i < getParameters().size(); ++i)
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute (p->paramID, p->getValue());
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void MonosynthPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
	
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our last window size..
            lastUIWidth  = jmax (xmlState->getIntAttribute ("uiWidth", lastUIWidth), 400);
            lastUIHeight = jmax (xmlState->getIntAttribute ("uiHeight", lastUIHeight), 200);
            
            
            // Now reload our parameters..
            for (int i = 0; i < getParameters().size(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValue ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MonosynthPluginAudioProcessor();
}

void MonosynthPluginAudioProcessor::timerCallback()
{
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::updateParameters(AudioBuffer<FloatType>& buffer)
{
    int numSamples = buffer.getNumSamples();
    int stepSize = jmin(1, numSamples);
    
    MonosynthVoice* synthVoice = dynamic_cast<MonosynthVoice*>(synth.getVoice(0));
    
    //synthVoice->setFilterEnvelopeSampleRate(sampleRate);
   // synthVoice->filterAmpEnvelope(*attackParam3, *decayParam3, *sustainParam3, *releaseParam3, *attackCurve3Param, *decayRelCurve3Param);
   // synthVoice->setAmpEnvelope   (*attackParam1, *decayParam1, dbToGain(*sustainParam1, MIN_INFINITY_DB), *releaseParam1, *attackCurve1Param, *decayRelCurve1Param);
    synthVoice->setPitchEnvelopeParameters (*attackParam2, *decayParam2, *sustainParam2, *releaseParam2, *attackCurve2Param, *decayRelCurve2Param);
    
    
    // AMPLITUDE
    envelopeGenerator[0].get()->setAttackRate( *attackParam1 );
    envelopeGenerator[0].get()->setDecayRate( *decayParam1 );
    envelopeGenerator[0].get()->setSustainLevel( dbToGain(*sustainParam1, MIN_INFINITY_DB) );
    envelopeGenerator[0].get()->setReleaseRate( *releaseParam1 );
    envelopeGenerator[0].get()->setTargetRatioA( *attackCurve1Param );
    envelopeGenerator[0].get()->setTargetRatioDR( *decayRelCurve1Param );
    
    // FILTER ENVELOPE
    envelopeGenerator[1].get()->setAttackRate( *attackParam3 );
    envelopeGenerator[1].get()->setDecayRate( *decayParam3 );
    envelopeGenerator[1].get()->setSustainLevel( *sustainParam3 );
    envelopeGenerator[1].get()->setReleaseRate( *releaseParam3 );
    envelopeGenerator[1].get()->setTargetRatioA( *attackCurve3Param );
    envelopeGenerator[1].get()->setTargetRatioDR( *decayRelCurve3Param );
    
    
    // PITCH ENVELOPE
    /*
    envelopeGenerator[2].get()->setAttackRate( *attackParam2 );
    envelopeGenerator[2].get()->setDecayRate( *decayParam2 );
    envelopeGenerator[2].get()->setSustainLevel( *sustainParam2 );
    envelopeGenerator[2].get()->setReleaseRate( *releaseParam2 );
    envelopeGenerator[2].get()->setTargetRatioA( *attackCurve2Param );
    envelopeGenerator[2].get()->setTargetRatioDR( *decayRelCurve2Param );
    */
    double osFactor = 1.0;
    
    if(*useHQOversamplingParam)
        osFactor = oversamplingDoubleHQ->getOversamplingFactor();
    else
        osFactor = oversamplingDouble->getOversamplingFactor();
    
    
    cutoff.setValue(*filterCutoffParam);
    contour.setValue(*filterContourParam);
    drive.setValue(*filterDriveParam);
    resonance.setValue(*filterQParam);
    
    
    pulsewidthSmooth1.setValue(*pulsewidthParam[0]);
    pulsewidthSmooth2.setValue(*pulsewidthParam[1]);
    pulsewidthSmooth3.setValue(*pulsewidthParam[2]);
    
    for (int i = 0; i < 3; i++)
        gainSmoothed[i].setValue(*oscGainParam[i]);
    
    for (int i = 0; i < numSamples; i++)
    {
        
        
        if (i % stepSize == 0)
        {
            // SCOPE
            double lowestFreq = synthVoice->getLowestPitchedOscFreq();
            int note = log( lowestFreq / 440.0 ) / log(2) * 12 + 69;
            
            if (note < 0)
                note = 0;
            
            if (note > 127)
                note = 127;
            
            int newPos = synthVoice->getLowestOscillatorRephaseIndex();
            
            scope.setNewTriggerPoint(newPos);
            scope.setNumSamplesPerPixel( ( 128 -  note ) * ( osFactor / 16 ) );
            
            
            // SEQUENCER
            
            if (lastSequencerOnOffState != *useSequencerParam)
            {
                currentPlayedNotes.clear();
                lastSequencerOnOffState = *useSequencerParam;
            }
            
            seqState.get()->setMaxSteps(*maxStepsParam);
            
            double speed = getLFOSyncedFreq(lastPosInfo, *stepDivisionFloatParam);
            seqState.get()->setSpeedInHz(speed);
            
            for (int i = 0; i < 8; i++)
            {
                seqState.get()->setPitchAmountForStep(i, *stepPitchParam[i]);
                seqState.get()->setShouldPlayForStep(i, *stepPlayParam[i]);
            }
            
            seqState.get()->setNoteDuration(*stepNoteLengthParam);
            seqState.get()->setSwingAmount(*swingParam);
            
            for(int osc = 0; osc < 3; osc++)
            {
                double dbGain = gainSmoothed[osc].getNextValue();
                double gain = dbGain;
                synthVoice->setGainForOscillator( gain, osc );
                synthVoice->setModeForOscillator(*oscModeParam[osc], osc);
                synthVoice->setDetuneAmountForOscillator(*oscDetuneAmountParam[osc], *oscOffsetParam[osc], osc);
                synthVoice->setPulsewidthModAmountForOscillator(*pulsewidthAmountParam[osc], osc);
            }
            
            
            
            
            //if (i % stepSize == 0)
            {
                synthVoice->setPulsewidthForOscillator(smoothing[PW_1_SMOOTHER]->processSmooth(pulsewidthSmooth1.getNextValue()), 0);
                synthVoice->setPulsewidthForOscillator(smoothing[PW_2_SMOOTHER]->processSmooth(pulsewidthSmooth2.getNextValue()), 1);
                synthVoice->setPulsewidthForOscillator(smoothing[PW_3_SMOOTHER]->processSmooth(pulsewidthSmooth3.getNextValue()), 2);
            }
            
            
            synthVoice->setPitchEnvelopeAmount(*pitchModParam);
            synthVoice->setHardSync(*oscSyncParam);
            synthVoice->sendLFO(lfo);
            overrideGlideTime = 0;
            synthVoice->setGlideTime(int(*glideTimeParam) + overrideGlideTime);
            
            saturationAmount.setValue(*saturationParam);
            
            //ARPEGGIATOR
            double hertz = getLFOSyncedFreq(lastPosInfo, *arpeggioNoteLengthParam);
            arp.setSpeedInHz(hertz);
        }
     
        
    }
}






void MonosynthPluginAudioProcessor::applyModToTarget(int target, double amount)
{
    MonosynthVoice* voice =  dynamic_cast<MonosynthVoice*>(synth.getVoice(0));
    modTarget t = (modTarget) target;
    
    voice->setPitchModulation(0.0);
    cutoffModulationAmt = 0.0;
    
    switch (t) {
        case modCutoff:
            cutoffModulationAmt = amount;
            break;
        case modPitch:
            voice->setPitchModulation(amount);
            break;
        case off:
            break;
        default:
            break;
    }
}

template <typename FloatType>
FloatType MonosynthPluginAudioProcessor::softClip(FloatType s)
{
    FloatType localSample = s;
    if (localSample > 1.0)
    {
        localSample = FloatType(1.0);
    }
    else if (localSample < -1.0)
    {
        localSample = FloatType(-1.0);
        
    }
    else
    {
        localSample = localSample - (  localSample * localSample * localSample );
    }
    return localSample;
}


#ifndef M_PI_4
#define M_PI_4 (3.1415926535897932384626433832795/4.0)
#endif



#define A 0.0776509570923569
#define B -0.287434475393028
#define C (M_PI_4 - A - B)
#define FMT "% 16.8f"

double FastArcTan(double x)
{
    double xx = x * x;
    return ((A*xx + B)*xx + C)*x;
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::softClipBuffer(AudioBuffer<FloatType>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    
    const FloatType* readLeft = buffer.getReadPointer(0);
    
    FloatType* dataLeft = buffer.getWritePointer(0);
    

    
    auto softClipLambda = [](auto sample)
    {
        return FastArcTan(sample) * ( 2 / FloatType(double_Pi) );
    };
    
    for (int i = 0; i < numSamples; i++)
    {
        
        dataLeft[i]  = softClipLambda(readLeft[i]);
    }
    
    
    
}



