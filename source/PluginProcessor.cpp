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
#include "MonoSynth.h"
#include <math.h>
#include <iostream>

#define MIN_INFINITY_DB -96.0f
#define CUTOFF_MIN 40.0f
#define CUTOFF_MAX 20000.0f

AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================
MonosynthPluginAudioProcessor::MonosynthPluginAudioProcessor()
: AudioProcessor (getBusesProperties()),
lastUIWidth (820),
lastUIHeight (590),

gainParam (nullptr),

osc1GainParam(nullptr),
osc2GainParam(nullptr),
osc3GainParam(nullptr),
osc1DetuneAmountParam(nullptr),
osc2DetuneAmountParam(nullptr),
osc3DetuneAmountParam(nullptr),

osc1ModeParam(nullptr),
osc2ModeParam(nullptr),
osc3ModeParam(nullptr),
oscSyncParam(nullptr),

filterCutoffParam(nullptr),
filterQParam(nullptr),
filterContourParam(nullptr),
filterDriveParam(nullptr),

pitchModParam(nullptr),

oscOffsetParam(nullptr),
osc2OffsetParam(nullptr),
osc3OffsetParam(nullptr),

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

pulsewidthAmount1Param(nullptr),
pulsewidthAmount2Param(nullptr),
pulsewidthAmount3Param(nullptr),

overSampleParam(nullptr),
oversampleSwitchParam(nullptr),

softClipSwitchParam(nullptr),

useSequencerParam(nullptr),
useHQOversamplingParam(nullptr)



{
    lastPosInfo.resetToDefault();

    
    // This creates our parameters. We'll keep some raw pointers to them in this class,
    // so that we can easily access them later, but the base class will take care of
    // deleting them for us.

	auto decibelToGainLambda = [](auto min, auto end, auto dB)  { return dB > min ? std::pow(10.0f, dB * 0.05f) / Decibels::decibelsToGain(end) : 0.0; };
	//auto gainToDecibelLambda = [](auto min, auto end, auto gain){ return gain > 0.0f ? 20.0f * std::log10(gain * Decibels::decibelsToGain(end)) : -std::numeric_limits<float>::infinity(); };
    
    auto gainToDecibelLambda = [](auto min, auto end, auto gain) { return gain > 0.0f ? 20.0f * std::log10(gain * Decibels::decibelsToGain(end) ) : -std::numeric_limits<float>::infinity(); };
	
	auto linToLogLambda = [](float start, float end, float linVal) { return std::pow(10.0f, (std::log10(end / start) * linVal + std::log10(start))); };
	auto logToLinLambda = [](float start, float end, float logVal) { return (std::log10(logVal / start) / std::log10(end / start)); };


    
	NormalisableRange<float> cutoffRange = NormalisableRange<float>(CUTOFF_MIN, CUTOFF_MAX,	linToLogLambda, logToLinLambda) ;

    addParameter (gainParam = new AudioParameterFloat("volume", "Volume" , NormalisableRange<float>(MIN_INFINITY_DB, 6.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
    
    addParameter (osc1GainParam  = new AudioParameterFloat ("osc1Gain",  "OSC1 Gain", NormalisableRange<float>(MIN_INFINITY_DB, 0.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
    addParameter (osc2GainParam  = new AudioParameterFloat ("osc2Gain",  "OSC2 Gain", NormalisableRange<float>(MIN_INFINITY_DB, 0.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
    addParameter (osc3GainParam  = new AudioParameterFloat ("osc3Gain",  "OSC3 Gain", NormalisableRange<float>(MIN_INFINITY_DB, 0.0f, gainToDecibelLambda, decibelToGainLambda), -1.0f));
    
    addParameter (osc1DetuneAmountParam = new AudioParameterFloat("osc1DetuneAmount", "OSC1 Tune", NormalisableRange<float>(-0.5f, 0.5f, 0.0f), 0.0f));
    addParameter (osc2DetuneAmountParam = new AudioParameterFloat("osc2DetuneAmount", "OSC2 Tune", NormalisableRange<float>(-0.5f, 0.5f, 0.0f), 0.0f));
    addParameter (osc3DetuneAmountParam = new AudioParameterFloat("osc3DetuneAmount", "OSC3 Tune", NormalisableRange<float>(-0.5f, 0.5f, 0.0f), 0.0f));
    
    addParameter(osc1ModeParam = new AudioParameterInt("osc1ModeChoice", "OSC1 Waveform", 0, 3, 2)); // TEST
    addParameter(osc2ModeParam = new AudioParameterInt("osc2ModeChoice", "OSC2 Waveform", 0, 3, 2));
    addParameter(osc3ModeParam = new AudioParameterInt("osc3ModeChoice", "OSC3 Waveform", 0, 3, 2));
    
   
    
	// addParameter (filterParam = new AudioParameterFloat("filter", "Filter Cutoff",                  NormalisableRange<float> (40.0f, 20000.0f, 0.0f, 0.3f, false), 20000.0f));
	addParameter (filterCutoffParam = new AudioParameterFloat("filter", "Filter Cutoff", cutoffRange, CUTOFF_MAX));
    
	addParameter (filterQParam = new AudioParameterFloat("filterQ", "Filter Reso.",                 NormalisableRange<float> (0.0f, 1.0f, 0.0f, 1.0f, false), 0.0f));
    addParameter (filterContourParam = new AudioParameterFloat("filterContour", "Filter Contour",	cutoffRange, CUTOFF_MIN));
    addParameter (filterDriveParam = new AudioParameterFloat("filterDrive", "Filter Drive",         NormalisableRange<float> (1.0f, 5.0f, 0.0f, 1.0f, false), 1.0f));
    
    // Filter Select Parameter
    addParameter (filterSelectParam = new AudioParameterInt("filterSelect", "Switch Filter", 0, 2, 2));
    
    addParameter (pitchModParam = new AudioParameterFloat("pitchMod", "Pitch Modulation", NormalisableRange<float> (0.0f, 2000.0f, 0.0f, 0.5f, false), 0.0f));
    addParameter (oscOffsetParam = new AudioParameterInt("osc1Offset", "OSC1 Offset", -24, 24, 0));
    addParameter (osc2OffsetParam = new AudioParameterInt("osc2Offset", "OSC2 Offset", -24, 24, 0));
    addParameter (osc3OffsetParam = new AudioParameterInt("osc3Offset", "OSC3 Offset", -24, 24, 0));
    
    addParameter (oscSyncParam = new AudioParameterInt("oscSync", "osc2>osc1 sync", 0, 1, 0));
    
    
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
    
    // PWM
    addParameter(pulsewidth1Param = new AudioParameterFloat("pulsewidth1Param", "PW1", 0.0f, 1.0f, 0.0f));
	addParameter(pulsewidth2Param = new AudioParameterFloat("pulsewidth2Param", "PW2", 0.0f, 1.0f, 0.0f));
	addParameter(pulsewidth3Param = new AudioParameterFloat("pulsewidth3Param", "PW3", 0.0f, 1.0f, 0.0f));
    
    addParameter(pulsewidthAmount1Param = new AudioParameterFloat("pulsewidthAmount1Param", "PWM1 Amt", 0.0f, 1.0f, 0.0f));
    addParameter(pulsewidthAmount2Param = new AudioParameterFloat("pulsewidthAmount2Param", "PWM2 Amt", 0.0f, 1.0f, 0.0f));
    addParameter(pulsewidthAmount3Param = new AudioParameterFloat("pulsewidthAmount3Param", "PWM3 Amt", 0.0f, 1.0f, 0.0f));

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
    }
    
    auto linToPow = [](float start, float end, float linVal) {
        double remapped = (6.0 - 1.0) * (linVal - 0.0) / (1.0 - 0.0) + 1.0 ; // remap
        int newValue = std::round(remapped);
        return std::pow(2, newValue);
        
    };
    auto powToLin = [](float start, float end, float powVal) {
        int newValue = std::round( powVal );
        double invLog = std::log2( newValue );
        return (1.0 - 0.0) * (invLog - 1.0) / (6.0 - 1.0) + 0.0; //remap
        
        
    };
    
	addParameter(stepNoteLengthParam = new AudioParameterFloat("stepNoteLengthParam", "Seq. Note Length" , 0.1f, 1.0f, 0.5f));
    addParameter(stepDivisionFloatParam   = new AudioParameterFloat("stepDivisionFloatParam", "Seq. Rate", NormalisableRange<float>(2.0, 64.0, linToPow, powToLin) , 16.0));
    
    addParameter(maxStepsParam = new AudioParameterInt ("maxStepsParam", "Max. Steps", 1, 8, 8 ));
    
    initialiseSynth();
    
    keyboardState.addListener(this);
    
    
    for(int channel = 0; channel < 2; channel++)
    {
        filterA[channel] = std::unique_ptr<LadderFilterBase>( new ImprovedMoog );
        filterB[channel] = std::unique_ptr<LadderFilterBase>( new ThreeFiveModel );
        filterC[channel] = std::unique_ptr<LadderFilterBase>( new DiodeLadderModel );
    }
    
    
    // Oversampling 2 times with FIR filtering
    oversamplingFloat  = std::unique_ptr<dsp::Oversampling<float>>  ( new dsp::Oversampling<float>  ( 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR , false ) );
    oversamplingDouble = std::unique_ptr<dsp::Oversampling<double>> ( new dsp::Oversampling<double> ( 2, 1, dsp::Oversampling<double>::filterHalfBandPolyphaseIIR , false ) );
    
    // HQ Oversampling 8 times with FIR filtering
    oversamplingFloatHQ  = std::unique_ptr<dsp::Oversampling<float>>  ( new dsp::Oversampling<float>  ( 2, 3, dsp::Oversampling<float>::filterHalfBandFIREquiripple , true ) );
    oversamplingDoubleHQ = std::unique_ptr<dsp::Oversampling<double>> ( new dsp::Oversampling<double> ( 2, 3, dsp::Oversampling<double>::filterHalfBandFIREquiripple , true ) );
    

    for (int i = 0; i < 6; i++)
        smoothing[i] = std::unique_ptr<SmoothParam>(new SmoothParam);
    
    sequencerProcessor = std::unique_ptr<SequencerProcessor> ( new SequencerProcessor( sequencerState ) );
    
}

MonosynthPluginAudioProcessor::~MonosynthPluginAudioProcessor()
{
    keyboardState.removeListener(this);
    synth.clearSounds();
    synth.clearVoices();
    
}


void MonosynthPluginAudioProcessor::initialiseSynth()
{
    synth.addVoice (new MonosynthVoice());
    synth.addSound (new MonosynthSound());
}




//==============================================================================
bool MonosynthPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only mono/stereo and input/output must have same layout
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
    
    // input and output layout must be the same
    if (layouts.getMainInputChannelSet() != mainOutput)
        return false;
    
    // do not allow disabling the main buses
    if (mainOutput.isDisabled()) return false;
    
    // only allow stereo and mono
    if (mainOutput.size() > 2) return false;
    
    return true;
}

AudioProcessor::BusesProperties MonosynthPluginAudioProcessor::getBusesProperties()
{
    return BusesProperties().withInput  ("Input",  AudioChannelSet::stereo(), true)
    .withOutput ("Output", AudioChannelSet::stereo(), true);
}

//==============================================================================
void MonosynthPluginAudioProcessor::prepareToPlay (double newSampleRate, int samplesPerBlock)
{
	oversamplingFloat->initProcessing(samplesPerBlock);
	oversamplingDouble->initProcessing(samplesPerBlock);
    oversamplingFloatHQ->initProcessing(samplesPerBlock);
    oversamplingDoubleHQ->initProcessing(samplesPerBlock);
    
    resetSamplerates(newSampleRate);
    
    keyboardState.reset();
	sequencerState.reset();

    for(int channel = 0; channel < 2; channel++)
    {
        
        filterA[channel]->SetResonance(0.1);
        filterA[channel]->SetCutoff(12000.0);
        filterA[channel]->SetDrive(1.0);
        
        
        filterB[channel]->SetResonance(0.1);
        filterB[channel]->SetCutoff(12000.0);
        filterB[channel]->SetDrive(1.0);
        
        
        filterC[channel]->SetResonance(0.1);
        filterC[channel]->SetCutoff(12000.0);
        filterC[channel]->SetDrive(1.0);
        
    }
    
    
    
    
    
}

void MonosynthPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
	sequencerState.reset();
    
    oversamplingFloat->reset();
    oversamplingDouble->reset();
    oversamplingFloatHQ->reset();
    oversamplingDoubleHQ->reset();
    
    for (int channel = 0; channel < 2; channel++)
    {
        filterA[channel]->Reset();
        filterB[channel]->Reset();
        filterC[channel]->Reset();
    }

    for (int i = 0; i < 6; i++)
        smoothing[i]->reset();
}

void MonosynthPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.

    cutoff.reset(sampleRate, cutoffRampTimeDefault);
    cutoffFromEnvelope.reset(sampleRate, cutoffRampTimeDefault);
    resonance.reset(sampleRate, 0.001);
    drive.reset(sampleRate, 0.001);
    pulsewidthSmooth1.reset(sampleRate, cutoffRampTimeDefault);
    pulsewidthSmooth2.reset(sampleRate, cutoffRampTimeDefault);
    pulsewidthSmooth3.reset(sampleRate, cutoffRampTimeDefault);
	saturationAmount.reset(sampleRate, cutoffRampTimeDefault);
    
    oversamplingFloat->reset();
    oversamplingDouble->reset();
    oversamplingFloatHQ->reset();
    oversamplingDoubleHQ->reset();
    
    
	for (int channel = 0; channel < 2; channel++)
	{
		filterA[channel]->Reset();
		filterB[channel]->Reset();
		filterC[channel]->Reset();
	}
    for (int i = 0; i < 6; i++)
        smoothing[i]->reset();
}




void MonosynthPluginAudioProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
   
    
    lfo.setPhase(0.0);
    
    contourVelocity = velocity;
    
    
    lastNotePlayed = midiNoteNumber;
	curMidiChannel = midiChannel;
}

void MonosynthPluginAudioProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    
    
}





void MonosynthPluginAudioProcessor::resetSamplerates(const double sr)
{
    double newsr = sr;
    
    
    
    
    
    if(*useHQOversamplingParam)
    {
        newsr *= oversamplingDoubleHQ->getOversamplingFactor();
        if ( isUsingDoublePrecision() )    { setLatencySamples(roundToInt(oversamplingDoubleHQ->getLatencyInSamples())); }
        else                            { setLatencySamples(roundToInt(oversamplingFloatHQ->getLatencyInSamples())); }
    }
    else
    {
        newsr *= oversamplingDouble->getOversamplingFactor();
        if ( isUsingDoublePrecision() )    { setLatencySamples(roundToInt(oversamplingDouble->getLatencyInSamples())); }
        else                            { setLatencySamples(roundToInt(oversamplingFloat->getLatencyInSamples())); }
    }
    
    
    synth.setCurrentPlaybackSampleRate (newsr);
    sequencerProcessor.get()->setPulseClockSampleRate(newsr);
    
    MonosynthVoice* synthVoice = dynamic_cast<MonosynthVoice*>(synth.getVoice(0));

	synthVoice->setEnvelopeSampleRate(newsr);
    
    for(int channel = 0; channel < 2; channel++)
    {
        filterA[channel]->SetSampleRate(newsr);
        filterB[channel]->SetSampleRate(newsr);
        filterC[channel]->SetSampleRate(newsr);
    }
    
    lfo.setSampleRate(newsr);
    
    
    
    cutoff.reset(newsr, cutoffRampTimeDefault);
    cutoffFromEnvelope.reset(newsr, cutoffRampTimeDefault);
    resonance.reset(newsr, 0.001);
    drive.reset(newsr, 0.001);
    
    pulsewidthSmooth1.reset(newsr, cutoffRampTimeDefault);
    pulsewidthSmooth2.reset(newsr, cutoffRampTimeDefault);
    pulsewidthSmooth3.reset(newsr, cutoffRampTimeDefault);
    
   
    
    
    smoothing[0]->init(newsr, 4.0);
    
    for (int i = 0; i < 6; i++)
        smoothing[i]->init(newsr, 2.0);
    

    
    sampleRate = newsr;
}

void MonosynthPluginAudioProcessor::setOversampleQuality(int q = 0)
{
    hqOversampling = bool(q);
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, std::unique_ptr<dsp::Oversampling<FloatType>>& oversampling)
{
    
    bool hqOn = bool(*useHQOversamplingParam);
   
    if (prevHqOversampling != hqOn)
    {
        resetSamplerates( getSampleRate() );
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

	updateParameters(buffer);

    
    

   
    // KEYBOARD
    keyboardState.processNextMidiBuffer(midiMessages, 0, osBuffer.getNumSamples(), true);
    
    
    // SEQUENCER
    if(*useSequencerParam)
    {
        sequencerProcessor.get()->processSequencer(osBuffer.getNumSamples());
        sequencerState.processBuffer(midiMessages, 0, osBuffer.getNumSamples(), true);
        
    }
    
    
    
    
	// Now pass any incoming midi messages to our keyboard state object, and let it
	// add messages to the buffer if the user is clicking on the on-screen keys
	
	
  
    
    // GET SYNTHDATA
    synth.renderNextBlock (osBuffer, midiMessages, 0, static_cast<int> ( osBuffer.getNumSamples() ) );
    
    
	
    
    
   
    
    
    
    // getting our filter envelope values
    applyFilterEnvelope(osBuffer);
    
    


    if (*filterOrderParam == 0)
    {
        if(filterOn)
        {
            // applying filter
            if (*filterSelectParam == 0)         { applyFilter(osBuffer, filterA) ; }
            else if (*filterSelectParam == 1)    { applyFilter(osBuffer, filterB) ; }
            else                                 { applyFilter(osBuffer, filterC) ; }
			
        }
        
        
    }
    else
    {
        if(filterOn)
        {
            // applying filter
            if (*filterSelectParam == 0)         { applyFilter(osBuffer, filterA) ; }
            else if (*filterSelectParam == 1)    { applyFilter(osBuffer, filterB) ; }
            else                                 { applyFilter(osBuffer, filterC) ; }
        }
    }
    

	//APPLYING WAVESHAPER
	if(*waveshapeSwitchParam == 1)
		applyWaveshaper(osBuffer);
 
    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		osBuffer.clear (i, 0, numSamples);
    
    
    // APPLY VOLUME
    applyGain (osBuffer); // apply our gain-change to the outgoing data..
    
    
    
    // APPLY SOFTCLIP
    if (*softClipSwitchParam == 1)
        softClipBuffer(osBuffer);

    
    
    

    //DOWNSAMPLING
	oversampling->processSamplesDown(block);

	    
    // Now ask the host for the current time so we can store it to be displayed later...
    updateCurrentTimeInfoFromHost();
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyGain(AudioBuffer<FloatType>& buffer)
{
	//masterGain.setValue(*gainParam);

    masterGain = dbToGain(*gainParam, MIN_INFINITY_DB);

	if (masterGain == masterGainPrev)
	{
		for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
			buffer.applyGain(channel, 0, buffer.getNumSamples(), masterGain);

	}
	else
	{
		for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
			buffer.applyGainRamp(channel, 0, buffer.getNumSamples(), masterGainPrev, masterGain);

		masterGainPrev = masterGain;
	}

}




template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilterEnvelope (AudioBuffer<FloatType>& buffer)
{
	//filterEnvelope->setSampleRate(sampleRate);
    
    MonosynthVoice* synthVoice = dynamic_cast<MonosynthVoice*> ( synth.getVoice(0) );
    
    synthVoice->setFilterEnvelopeSampleRate(sampleRate);
    synthVoice->filterAmpEnvelope(*attackParam3, *decayParam3, *sustainParam3, *releaseParam3, *attackCurve3Param, *decayRelCurve3Param);
    
    
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
        
        
        const double lfoValue = lfo.nextSample();
        
        modAmount = *lfoIntensityParam;                            // Make parameter
        applyModToTarget(*modTargetParam, lfoValue * modAmount);
        

		//
		// @BUG: Cutoff initializes to NaN
		//
        
        // Modulation by envelope and LFO (if set)
        const double lfoFilterRange = 6000.0;
        const double contourRange = *filterContourParam;
        const double filterEnvelopeVal = synthVoice->getFilterEnvelopeValue();
        currentCutoff = (filterEnvelopeVal * contourRange) + (lfoFilterRange * cutoffModulationAmt);
        
        cutoff.setValue				(*filterCutoffParam);
        cutoffFromEnvelope.setValue	(currentCutoff);
        resonance.setValue			(*filterQParam);
        drive.setValue				(*filterDriveParam);
        
    }
    
    
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilter (AudioBuffer<FloatType>& buffer, std::unique_ptr<LadderFilterBase> filter[])
{
    
    FloatType* channelDataLeft  = buffer.getWritePointer(0);
    FloatType* channelDataRight = buffer.getWritePointer(1);
    
    int numSamples = buffer.getNumSamples();

    
    //
    //  break buffer into chunks
    //
    int stepSize = jmin(16, numSamples);
    
    int samplesLeftOver = numSamples;
    
    
    for (int step = 0; step < numSamples; step += stepSize)
    {
        
        FloatType combinedCutoff = currentCutoff + smoothing[0]->processSmooth( cutoff.getNextValue() ) ;

		if (combinedCutoff > CUTOFF_MAX) combinedCutoff = CUTOFF_MAX;
		if (combinedCutoff < CUTOFF_MIN) combinedCutoff = CUTOFF_MIN;
        
		

        for (int channel = 0; channel < 2; channel++)
        {
                //filter[channel]->SetSampleRate(sampleRate * oversamp->getOversamplingFactor());
            auto snapToLocalVal= [](double val) -> double { if (val < 0.0) val = 0.0; else if (val > 1.0) val = 1.0; return val;  };

            FloatType newReso =  snapToLocalVal(resonance.getNextValue());

            filter[channel]->SetResonance(newReso);
            filter[channel]->SetDrive(drive.getNextValue());
        }

        if (samplesLeftOver < stepSize)
            stepSize = samplesLeftOver;
        

		if (prevCutoff == combinedCutoff)
		{
			filter[0]->SetCutoff(combinedCutoff);
			filter[1]->SetCutoff(combinedCutoff);

            if (filter[0]->SetCutoff(combinedCutoff))
                filter[0]->Process(channelDataLeft, stepSize);

            if (filter[1]->SetCutoff(combinedCutoff))
                filter[1]->Process(channelDataRight, stepSize);
		}
		else
		{
			filter[0]->ProcessRamp(channelDataLeft, stepSize, prevCutoff, combinedCutoff);
			filter[1]->ProcessRamp(channelDataRight, stepSize, prevCutoff, combinedCutoff);
		}
        
        prevCutoff = combinedCutoff;

        samplesLeftOver -= stepSize;

        channelDataLeft += stepSize;
        channelDataRight += stepSize;
    }
    
}

template <typename FloatType>
void MonosynthPluginAudioProcessor::applyWaveshaper(AudioBuffer<FloatType>& buffer)
{
	const int numSamples = buffer.getNumSamples();

    const FloatType* readLeft = buffer.getReadPointer(0);
    const FloatType* readRight = buffer.getReadPointer(1);
    
	FloatType* dataL = buffer.getWritePointer(0);
	FloatType* dataR = buffer.getWritePointer(1);

	

	auto saturation = saturationAmount.getNextValue();

	for (int i = 0; i < numSamples; i++)
	{
		dataL[i] = getWaveshaped(readLeft[i], saturation, *waveshapeModeParam);
		dataR[i] = getWaveshaped(readRight[i], saturation, *waveshapeModeParam);
	}
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
    
    xml.setAttribute ("sequencerOn", lastSequencerChoice);
    xml.setAttribute ("oversampleOn", lastOversampleChoice);
    
    
    xml.setAttribute ("seqNoteLength", lastSeqNoteLength);
    xml.setAttribute ("seqDivision", lastSeqDivision);
    
    
    for (int step = 0; step < 8; step++)
    {
        String s = "seqPitchVal" + std::to_string(step);
        xml.setAttribute (s, lastSeqPitchValue[step]);
    }
    
    // Store the values of all our parameters, using their param ID as the XML attribute
    for (int i = 0; i < getNumParameters(); ++i)
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
            
            lastSequencerChoice  = xmlState->getBoolAttribute("sequencerOn");
            lastOversampleChoice = xmlState->getBoolAttribute("oversampleOn");
            
            for (int step = 0; step < 8; step++)
            {
                String s = "seqPitchVal" + std::to_string(step);
                lastSeqPitchValue[step] = xmlState->getIntAttribute(s);
            }
            
            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
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
    int stepSize = jmin(16, numSamples);
    
    MonosynthVoice* synthVoice = dynamic_cast<MonosynthVoice*>(synth.getVoice(0));
    
    
    
    for (int i = 0; i < numSamples; i++)
    {
        
       
        
        sequencerProcessor.get()->setTimeDivision(*stepDivisionFloatParam);
        
        
        for (int i = 0; i < 8; i++ )
        {
            sequencerProcessor.get()->setStepPitch(i, *stepPitchParam[i]);
        }
        
        sequencerProcessor.get()->setGlobalNoteLength(*stepNoteLengthParam);
        
        sequencerProcessor.get()->setBPM(lastPosInfo.bpm);
        
        
        // set various parameters
        synthVoice->setOscGains(
                    dbToGain(*osc1GainParam, MIN_INFINITY_DB),
                    dbToGain(*osc2GainParam, MIN_INFINITY_DB),
                    dbToGain(*osc3GainParam, MIN_INFINITY_DB)
                    );
        
        synthVoice->setOscModes(*osc1ModeParam, *osc2ModeParam, *osc3ModeParam);

        synthVoice->setAmpEnvelope   (*attackParam1, *decayParam1, dbToGain(*sustainParam1, MIN_INFINITY_DB), *releaseParam1, *attackCurve1Param, *decayRelCurve1Param);
        synthVoice->setPitchEnvelope (*attackParam2, *decayParam2, *sustainParam2, *releaseParam2, *attackCurve3Param, *decayRelCurve3Param);

        synthVoice->setPitchEnvelopeAmount(*pitchModParam);

        synthVoice->setOsc1DetuneAmount(*osc1DetuneAmountParam, *oscOffsetParam ); //TEST
        synthVoice->setOsc2DetuneAmount(*osc2DetuneAmountParam, *osc2OffsetParam);
        synthVoice->setOsc3DetuneAmount(*osc3DetuneAmountParam, *osc3OffsetParam);

        

        synthVoice->setHardSync(*oscSyncParam);

        

        synthVoice->setModAmountPW(*pulsewidthAmount1Param, 0);
        synthVoice->setModAmountPW(*pulsewidthAmount2Param, 1);
        synthVoice->setModAmountPW(*pulsewidthAmount3Param, 2);

		saturationAmount.setValue(*saturationParam);
        
        pulsewidthSmooth1.setValue(*pulsewidth1Param);
        pulsewidthSmooth2.setValue(*pulsewidth2Param);
        pulsewidthSmooth3.setValue(*pulsewidth3Param);

        synthVoice->sendLFO(lfo);


        if (i % stepSize == 0)
        {
            synthVoice->setPulsewidth(smoothing[1]->processSmooth(pulsewidthSmooth1.getNextValue()), 0);
            synthVoice->setPulsewidth(smoothing[2]->processSmooth(pulsewidthSmooth2.getNextValue()), 1);
            synthVoice->setPulsewidth(smoothing[3]->processSmooth(pulsewidthSmooth3.getNextValue()), 2);
        }
    }
}






void MonosynthPluginAudioProcessor::applyModToTarget(int target, double amount)
{
    modTarget t = (modTarget) target;
    
    switch (t) {
        case modCutoff:
            
            cutoffModulationAmt = amount;
            
            dynamic_cast<MonosynthVoice*>(synth.getVoice(0))->setPitchModulation(0.0);
            
            break;
        case modPitch: 
            dynamic_cast<MonosynthVoice*>(synth.getVoice(0))->setPitchModulation(amount);
            
            cutoffModulationAmt = 0.0;
            
            break;
            
        case off:
            dynamic_cast<MonosynthVoice*>(synth.getVoice(0))->setPitchModulation(0.0);
            cutoffModulationAmt = 0.0;
            
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


template <typename FloatType>
void MonosynthPluginAudioProcessor::softClipBuffer(AudioBuffer<FloatType>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    
    const FloatType* readLeft = buffer.getReadPointer(0);
    const FloatType* readRight = buffer.getReadPointer(1);
    
    FloatType* dataLeft = buffer.getWritePointer(0);
    FloatType* dataRight = buffer.getWritePointer(1);
    

    
    auto softClipLambda = [](auto sample)
    {
        return std::atan(sample) * ( 2 / FloatType(double_Pi) );
    };
    
    for (int i = 0; i < numSamples; i++)
    {
        
        dataLeft[i]  = softClipLambda(readLeft[i]);
        dataRight[i] = softClipLambda(readRight[i]);
    }
    
    
    
}



double MonosynthPluginAudioProcessor::getLFOSyncedFreq(AudioPlayHead::CurrentPositionInfo posInfo, double division )
{
    const double beats_per_minute = posInfo.bpm;
    const double seconds_per_beat = 60.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * (lastPosInfo.timeSigDenominator / division);
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return 1.0 / seconds_per_note;
}



bool MonosynthPluginAudioProcessor::saturationOn()
{
    return *waveshapeSwitchParam == 1;
}

bool MonosynthPluginAudioProcessor::noteIsBeingPlayed()
{
    return true;
}

bool MonosynthPluginAudioProcessor::lfoSynced()
{
    return (*lfoSyncParam == 1);
    
}


bool MonosynthPluginAudioProcessor::isSequencerPlaying()
{
	return *useSequencerParam;
}

