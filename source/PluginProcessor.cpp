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

AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================
MonosynthPluginAudioProcessor::MonosynthPluginAudioProcessor()
        : AudioProcessor (getBusesProperties()),
        lastUIWidth (720),
        lastUIHeight (450),

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

        filterParam(nullptr),
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

        filterSelectParam(nullptr),
        lfoDivisionParam(nullptr),
        delayPosition (0),
        filterEnvelope(nullptr),
        ampEnvelope(nullptr)
{
    lastPosInfo.resetToDefault();
    
    

    // This creates our parameters. We'll keep some raw pointers to them in this class,
    // so that we can easily access them later, but the base class will take care of
    // deleting them for us.

    //addParameter (gainParam  = new AudioParameterFloat ("volume",  "Volume",           0.0f, 1.0f, 0.4f));

    addParameter (gainParam = new AudioParameterFloat("volume", "Volume" , NormalisableRange<float> (0.0, 2.0, 0.0, 0.5, false), 0.9));
    
    addParameter (osc1GainParam  = new AudioParameterFloat ("osc1Gain",  "OSC1 Gain",  0.0f, 1.0f, 0.9f));
    addParameter (osc2GainParam  = new AudioParameterFloat ("osc2Gain",  "OSC2 Gain",  0.0f, 1.0f, 0.9f));
    addParameter (osc3GainParam  = new AudioParameterFloat ("osc3Gain",  "OSC3 Gain",  0.0f, 1.0f, 0.9f));

    addParameter (osc1DetuneAmountParam = new AudioParameterFloat("osc1DetuneAmount", "OSC1 Tune", NormalisableRange<float>(-1.0, 1.0, 0.0), 0.0));
    addParameter (osc2DetuneAmountParam = new AudioParameterFloat("osc2DetuneAmount", "OSC2 Tune", NormalisableRange<float>(-1.0, 1.0, 0.0), 0.0));
    addParameter (osc3DetuneAmountParam = new AudioParameterFloat("osc3DetuneAmount", "OSC3 Tune", NormalisableRange<float>(-1.0, 1.0, 0.0), 0.0));

    addParameter(osc1ModeParam = new AudioParameterInt("osc1ModeChoice", "OSC1 Waveform", 0, 2, 2));
    addParameter(osc2ModeParam = new AudioParameterInt("osc2ModeChoice", "OSC2 Waveform", 0, 2, 2));
    addParameter(osc3ModeParam = new AudioParameterInt("osc3ModeChoice", "OSC3 Waveform", 0, 2, 2));

    addParameter (filterParam = new AudioParameterFloat("filter", "Filter Cutoff",                  NormalisableRange<float> (0.0, 14000.0, 0.0, 0.5, false), 12000.0));
    addParameter (filterQParam = new AudioParameterFloat("filterQ", "Filter Reso.",                 NormalisableRange<float> (0.0, 1.0, 0.0, 1.0, false), 0.0));
    addParameter (filterContourParam = new AudioParameterFloat("filterContour", "Filter Contour",   NormalisableRange<float> (0, 14000.0, 0.0, 0.5, false), 0.0));
    addParameter (filterDriveParam = new AudioParameterFloat("filterDrive", "Filter Drive",         NormalisableRange<float> (1.0, 5.0, 0.0, 1.0, false), 1.0));
    // Filter Select Parameter
    addParameter (filterSelectParam = new AudioParameterInt("filterSelect", "Switch Filter", 0, 2, 0));

    addParameter (pitchModParam = new AudioParameterFloat("pitchMod", "Pitch Modulation", NormalisableRange<float> (0, 2000.0, 0.0, 0.5, false), 0.0));
    addParameter (oscOffsetParam = new AudioParameterInt("osc1Offset", "OSC1 Offset", -24, 24.0, 0.0));
    addParameter (osc2OffsetParam = new AudioParameterInt("osc2Offset", "OSC2 Offset", -24, 24.0, 0.0));
    addParameter (osc3OffsetParam = new AudioParameterInt("osc3Offset", "OSC3 Offset", -24, 24.0, 0.0));
    
    addParameter (oscSyncParam = new AudioParameterInt("oscSync", "osc2>osc1 sync", 0, 1, 0));


    //ENV 1
    addParameter (attackParam1 = new AudioParameterFloat ("attack1", "Amp Attack",       NormalisableRange<float>(0.001, 11.0, 0.0, 0.5, false), 0.001));
    addParameter (decayParam1  = new AudioParameterFloat ("decay1", "Amp Decay",         NormalisableRange<float>(0.001, 11.0, 0.0, 0.5, false), 0.001));
    addParameter (sustainParam1  = new AudioParameterFloat ("sustain1", "Amp Sustain",   NormalisableRange<float>(0.0, 1.0,  0.0, 0.5, false), 1.0));
    addParameter (releaseParam1  = new AudioParameterFloat ("release1", "Amp Release",   NormalisableRange<float>(0.001, 11.0, 0.0, 0.5, false), 0.01));

    addParameter (attackCurve1Param = new AudioParameterFloat("attackCurve1", "Attack Curve",           NormalisableRange<float>(0.001, 1.0, 0.0, 0.5, false), 0.001));
    addParameter (decayRelCurve1Param = new AudioParameterFloat("decRelCurve1", "Decay-Release Curve",  NormalisableRange<float>(0.00001, 1.0, 0.0, 0.5, false), 0.00001));  
    //ENV 2
    addParameter (attackParam2 = new AudioParameterFloat ("attack2", "Pitch Attack",     NormalisableRange<float>(0.0, 11.0, 0.0, 0.5, false), 0.0));
    addParameter (decayParam2  = new AudioParameterFloat ("decay2", "Pitch Decay",       NormalisableRange<float>(0.0, 11.0, 0.0, 0.5, false), 0.0));
    addParameter (sustainParam2  = new AudioParameterFloat ("sustain2", "Pitch Sustain", NormalisableRange<float>(0.0, 1.0,  0.0, 0.5, false), 0.0));
    addParameter (releaseParam2  = new AudioParameterFloat ("release2", "Pitch Release", NormalisableRange<float>(0.0, 11.0, 0.0, 0.5, false), 0.0));

    addParameter(attackCurve2Param = new AudioParameterFloat("attackCurve2", "Attack Curve",			NormalisableRange<float>(0.001, 1.0, 0.0, 0.5, false), 0.001));
    addParameter(decayRelCurve2Param = new AudioParameterFloat("decRelCurve2", "Decay-Release Curve",	NormalisableRange<float>(0.00001, 1.0, 0.0, 0.5, false), 0.00001));

    //ENV 3
    addParameter (attackParam3 = new AudioParameterFloat ("attack3", "Filter Attack",    NormalisableRange<float>(0.0, 11.0, 0.0, 0.5, false), 0.0));
    addParameter (decayParam3  = new AudioParameterFloat ("decay3", "Filter Decay",      NormalisableRange<float>(0.0, 11.0, 0.0, 0.5, false), 0.0));
    addParameter (sustainParam3  = new AudioParameterFloat ("sustain3", "Filter Sustain",NormalisableRange<float>(0.0, 1.0,  0.0, 0.5, false), 0.0));
    addParameter (releaseParam3  = new AudioParameterFloat ("release3", "Filter Release",NormalisableRange<float>(0.01, 11.0, 0.0, 0.5, false), 0.01));

    addParameter(attackCurve3Param = new AudioParameterFloat("attackCurve3", "Attack Curve", NormalisableRange<float>(0.001, 1.0, 0.0, 0.5, false), 0.001));
    addParameter(decayRelCurve3Param = new AudioParameterFloat("decRelCurve3", "Decay-Release Curve", NormalisableRange<float>(0.00001, 1.0, 0.0, 0.5, false), 0.00001));


    // Modulation
    addParameter(modTargetParam = new AudioParameterInt("modTarget", "Modulation Target", 0, 2, 2));


    // LFO
    addParameter(lfoRateParam = new AudioParameterFloat("lfoRate", "LFO Rate", NormalisableRange<float>(0.01, 30.0, 0.0, 0.5, false), 0.05));
    addParameter(lfoModeParam = new AudioParameterInt ("lfoMode", "LFO Mode", 0, 2, 0));
    addParameter(lfoIntensityParam = new AudioParameterFloat("lfoIntensity", "LFO Strength", NormalisableRange<float>(0.0, 1.0, 0.0, 1.0, false), 0.0));
	addParameter(lfoSyncParam = new AudioParameterInt("lfoSync", "LFO Tempo Sync", 0, 1, 0));
    addParameter(lfoDivisionParam = new AudioParameterInt("lfoDivision", "LFO Synced Rate", 1, 6, 2));
    
    initialiseSynth();

    keyboardState.addListener(this);

    filterEnvelope = new ADSR();
    ampEnvelope = new ADSR();
}

MonosynthPluginAudioProcessor::~MonosynthPluginAudioProcessor()
{
    keyboardState.removeListener(this);

	synth.clearSounds();
	synth.clearVoices();
}


void MonosynthPluginAudioProcessor::initialiseSynth()
{
    synth.addVoice (new SineWaveVoice());
    synth.addSound (new SineWaveSound());
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
    sampleRate = newSampleRate;

    for(int channel = 0; channel < 2; channel++)
    {
        filterA[channel] = new ImprovedMoog();
		filterA[channel]->SetSampleRate(sampleRate);
		filterA[channel]->SetResonance(0.1);
		filterA[channel]->SetCutoff(12000.0);
		filterA[channel]->SetDrive(1.0);

		
        filterB[channel] = new ThreeFiveModel();
		filterB[channel]->SetSampleRate(sampleRate);
		filterB[channel]->SetResonance(0.1);
		filterB[channel]->SetCutoff(12000.0);
		filterB[channel]->SetDrive(1.0);
		

        filterC[channel] = new DiodeLadderModel();
		filterC[channel]->SetSampleRate(sampleRate);
		filterC[channel]->SetResonance(0.1);
		filterC[channel]->SetCutoff(12000.0);
		filterC[channel]->SetDrive(1.0);
		
    }
	
	lfo.setSampleRate(sampleRate);
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (sampleRate);
    keyboardState.reset();
   
    cutoff.reset(sampleRate, cutoffRampTimeDefault);
	cutoffFromEnvelope.reset(sampleRate, 0.0003);
    resonance.reset(sampleRate, 0.001);
    drive.reset(sampleRate, 0.001);
	masterGain.reset(sampleRate, 0.001);

   
    filterEnvelope->setSampleRate(sampleRate);
    ampEnvelope->setSampleRate(sampleRate);
}

void MonosynthPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
	cutoff.reset(sampleRate, cutoffRampTimeDefault);
	cutoffFromEnvelope.reset(sampleRate, 0.0003);
	resonance.reset(sampleRate, 0.001);
	drive.reset(sampleRate, 0.001);
	masterGain.reset(sampleRate, 0.001);
    
    for (int channel = 0; channel < 2; channel ++)
    {
        filterA[channel]->Reset();
        filterB[channel]->Reset();
        filterC[channel]->Reset();
    }
}

void MonosynthPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
	cutoff.reset(sampleRate, cutoffRampTimeDefault);
	cutoffFromEnvelope.reset(sampleRate, 0.0003);
	resonance.reset(sampleRate, 0.001);
	drive.reset(sampleRate, 0.001);
	masterGain.reset(sampleRate, 0.001);
    
    for (int channel = 0; channel < 2; channel ++)
    {
        filterA[channel]->Reset();
        filterB[channel]->Reset();
        filterC[channel]->Reset();
    }
}




void MonosynthPluginAudioProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{

    contourVelocity = velocity;
    
	lfo.setPhase(0.0);
    
    filterEnvelope->gate(true);
    ampEnvelope->gate(true);
    
    lastNotePlayed = midiNoteNumber;

}

void MonosynthPluginAudioProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    
    if (lastNotePlayed == midiNoteNumber)
    {
        ampEnvelope->gate(false);
        filterEnvelope->gate(false);
    }

}


template <typename FloatType>
void MonosynthPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
   

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

    updateParameters();
    
    
    // and now get our synth to process these midi events and generate its output.
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    
    // getting our filter envelope values
    applyFilterEnvelope(buffer);
    
    
    // applying our filter
	if (*filterSelectParam == 0)		{ applyFilter(buffer, filterA); }
	else if (*filterSelectParam == 1)	{ applyFilter(buffer, filterB); }
	else								{ applyFilter(buffer, filterC); }


    applyAmpEnvelope(buffer);

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);

    applyGain (buffer); // apply our gain-change to the outgoing data..

    // Now ask the host for the current time so we can store it to be displayed later...
    updateCurrentTimeInfoFromHost();
}


template <typename FloatType>
void MonosynthPluginAudioProcessor::applyGain (AudioBuffer<FloatType>& buffer)
{
	masterGain.setValue(*gainParam);

    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), masterGain.getNextValue());
}





template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilterEnvelope (AudioBuffer<FloatType>& buffer)
{
    filterEnvelope->setAttackRate(*attackParam3);
    filterEnvelope->setDecayRate(*decayParam3);
    filterEnvelope->setReleaseRate(*releaseParam3);
    filterEnvelope->setSustainLevel(*sustainParam3);
    filterEnvelope->setTargetRatioA(*attackCurve2Param);
    filterEnvelope->setTargetRatioDR(*decayRelCurve2Param);

    const int numSamples = buffer.getNumSamples();
    
    for (int i = 0; i < numSamples; i++)
    {
        //
        // LFO
        //
        lfo.setMode(*lfoModeParam);
        
        double lfo_division = pow(2.0, *lfoDivisionParam);
       
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
        
        
        // Modulation by envelope and LFO (if set)
        const double lfoFilterRange = 6000.0;
        const double contourRange = *filterContourParam * contourVelocity;
        currentCutoff = (filterEnvelope->process() * contourRange) + (lfoFilterRange * cutoffModulationAmt);
        
        if (currentCutoff > 14000.0) currentCutoff = 14000.0;
        if (currentCutoff < 20.0) currentCutoff = 20.0;
        
		
        cutoff.setValue				(*filterParam);
		cutoffFromEnvelope.setValue	(currentCutoff);
		resonance.setValue			(*filterQParam);
        drive.setValue				(*filterDriveParam);
        
        
    }
    
    
}



template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilter (AudioBuffer<FloatType>& buffer, LadderFilterBase* filter[])
{
    
    const int numSamples = buffer.getNumSamples();
    
    FloatType* channelDataLeft  = buffer.getWritePointer(0);
	FloatType* channelDataRight = buffer.getWritePointer(1);
    
    //
    //  break buffer into chunks
    //
    int stepSize = jmin(16, numSamples);
    
    int samplesLeftOver = numSamples;
    
  
	for (int step = 0; step < numSamples; step += stepSize)
	{
		//filter[0]->update();
		//filter[1]->update();

		double combinedCutoff   = currentCutoff + cutoff.getNextValue();
		double Q                = resonance.getNextValue();// * 4.0;

		for (int channel = 0; channel < 2; channel++)
		{
			filter[channel]->SetResonance(Q);
			filter[channel]->SetCutoff(combinedCutoff);
			filter[channel]->SetDrive(drive.getNextValue());
		}

		if (samplesLeftOver < stepSize)
			stepSize = samplesLeftOver;

		filter[0]->Process(channelDataLeft, stepSize);
		filter[1]->Process(channelDataRight, stepSize);

		samplesLeftOver -= stepSize;

		channelDataLeft += stepSize;
		channelDataRight += stepSize;
	}
}


template <typename FloatType>
void MonosynthPluginAudioProcessor::applyFilterAlt(AudioBuffer<FloatType>& buffer)
{


	const int numSamples = buffer.getNumSamples();

	FloatType* channelDataLeft = buffer.getWritePointer(0);
	FloatType* channelDataRight = buffer.getWritePointer(1);

	//
	//  break buffer into chunks
	//
	int stepSize = jmin(16, numSamples);

	int samplesLeftOver = numSamples;


	if (*filterSelectParam == 0)
	{
		for (int step = 0; step < numSamples; step += stepSize)
		{
			//filter[0]->update();
			//filter[1]->update();

			double combinedCutoff = currentCutoff + cutoff.getNextValue();
			double Q = resonance.getNextValue();// * 4.0;

			for (int channel = 0; channel < 2; channel++)
			{
				filterA[channel]->SetResonance(Q);
				filterA[channel]->SetCutoff(combinedCutoff);
				filterA[channel]->SetDrive(drive.getNextValue());
			}

			if (samplesLeftOver < stepSize)
				stepSize = samplesLeftOver;

			filterA[0]->Process(channelDataLeft, stepSize);
			filterA[1]->Process(channelDataRight, stepSize);

			samplesLeftOver -= stepSize;

			channelDataLeft += stepSize;
			channelDataRight += stepSize;
		}
	}
	else if (*filterSelectParam == 1)
	{
		for (int step = 0; step < numSamples; step += stepSize)
		{
			//filter[0]->update();
			//filter[1]->update();

			double combinedCutoff = currentCutoff + cutoff.getNextValue();
			double Q = resonance.getNextValue();// * 4.0;

			for (int channel = 0; channel < 2; channel++)
			{
				filterB[channel]->SetResonance(Q);
				filterB[channel]->SetCutoff(combinedCutoff);
				filterB[channel]->SetDrive(drive.getNextValue());
			}

			if (samplesLeftOver < stepSize)
				stepSize = samplesLeftOver;

			filterB[0]->Process(channelDataLeft, stepSize);
			filterB[1]->Process(channelDataRight, stepSize);

			samplesLeftOver -= stepSize;

			channelDataLeft += stepSize;
			channelDataRight += stepSize;
		}
	}
	else
	{
		for (int step = 0; step < numSamples; step += stepSize)
		{
			//filter[0]->update();
			//filter[1]->update();

			double combinedCutoff = currentCutoff + cutoff.getNextValue();
			double Q = resonance.getNextValue();// * 4.0;

			for (int channel = 0; channel < 2; channel++)
			{
				filterC[channel]->SetResonance(Q);
				filterC[channel]->SetCutoff(combinedCutoff);
				filterC[channel]->SetDrive(drive.getNextValue());
			}

			if (samplesLeftOver < stepSize)
				stepSize = samplesLeftOver;

			filterC[0]->Process(channelDataLeft, stepSize);
			filterC[1]->Process(channelDataRight, stepSize);

			samplesLeftOver -= stepSize;

			channelDataLeft += stepSize;
			channelDataRight += stepSize;
		}
	}

	
}


template <typename FloatType>
void MonosynthPluginAudioProcessor::applyAmpEnvelope(AudioBuffer<FloatType>& buffer)
{
   
    int numSamples = buffer.getNumSamples();
    
    FloatType* channelDataLeft  = buffer.getWritePointer(0);
    FloatType* channelDataRight = buffer.getWritePointer(1);
    
    ampEnvelope->setSampleRate(getSampleRate());
    ampEnvelope->setAttackRate(*attackParam1);
    ampEnvelope->setAttackRate(*attackParam1);
    ampEnvelope->setDecayRate(*decayParam1);
    ampEnvelope->setReleaseRate(*releaseParam1);
    ampEnvelope->setSustainLevel(*sustainParam1);
    ampEnvelope->setTargetRatioA(*attackCurve1Param);
    ampEnvelope->setTargetRatioDR(*decayRelCurve1Param);
    
   // std::cout << filterEnvelope->getState() << std::endl;
     
	int i = 0;
    
    while ( --numSamples >= 0)
    {
		float val = ampEnvelope->process();
        envGain.setValue( val );
		channelDataLeft[i] *= envGain.getNextValue();
		channelDataRight[i] *= envGain.getNextValue();
		i++;
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
    return new MonosynthPluginAudioProcessorEditor (*this);
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



void inline MonosynthPluginAudioProcessor::updateParameters()
{
    // set various parameters
    setOscGains(*osc1GainParam, *osc2GainParam, *osc3GainParam);
    setOscModes(*osc1ModeParam, *osc2ModeParam, *osc3ModeParam);
    
   
    setPitchEnvelope(*attackParam2, *decayParam2, *sustainParam2, *releaseParam2, *attackCurve3Param, *decayRelCurve3Param);
    
	setPitchEnvelopeAmount(*pitchModParam);
    
    setOsc1DetuneAmount(*osc1DetuneAmountParam, *oscOffsetParam );
    setOsc2DetuneAmount(*osc2DetuneAmountParam, *osc2OffsetParam);
    setOsc3DetuneAmount(*osc3DetuneAmountParam, *osc3OffsetParam);

	setEnvelopeState( *ampEnvelope );
    
    setHardSync(*oscSyncParam);
    
}



void MonosynthPluginAudioProcessor::setPitchEnvelope(float attack, float decay, float sustain, float release, float attackCurve, float decRelCurve)
{
    
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setPitchEnvelope(attack, decay, sustain, release, attackCurve, decRelCurve);
    
}


void MonosynthPluginAudioProcessor::setPitchEnvelopeAmount(float pitchMod)
{
    
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setPitchEnvelopeAmount(pitchMod);
    
}


void MonosynthPluginAudioProcessor::setOsc1DetuneAmount(float fine, int coarse)
{
	
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setOsc1DetuneAmount(fine, coarse);
	
}

void MonosynthPluginAudioProcessor::setOsc2DetuneAmount(float fine, int coarse)
{
	
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setOsc2DetuneAmount(fine, coarse);
	
}


void MonosynthPluginAudioProcessor::setOsc3DetuneAmount(float fine, int coarse)
{
    
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setOsc3DetuneAmount(fine, coarse);
    
}

void MonosynthPluginAudioProcessor::setOscGains(float osc1Gain, float osc2Gain, float osc3Gain)
{
    
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setOscGains(osc1Gain, osc2Gain, osc3Gain);
    
}

void MonosynthPluginAudioProcessor::setOscModes(int osc1Mode, int osc2Mode, int osc3Mode)
{
    
    return dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setOscModes(osc1Mode, osc2Mode, osc3Mode);
    
}


void MonosynthPluginAudioProcessor::setEnvelopeState(ADSR envelope)
{

	return static_cast<SineWaveVoice*>(synth.getVoice(0))->setEnvelopeState(envelope);

}

void MonosynthPluginAudioProcessor::setHardSync(int sync)
{
    
    return static_cast<SineWaveVoice*>(synth.getVoice(0))->setHardSync(sync);
    
}


void MonosynthPluginAudioProcessor::applyModToTarget(int target, double amount)
{
    modTarget t = (modTarget) target;
    
	switch (t) {
	case modCutoff:
        
        cutoffModulationAmt = amount;
        
        dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setPitchModulation(0.0);
        
        break;
	case modPitch:
        dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setPitchModulation(amount);
            
        cutoffModulationAmt = 0.0;
        
        break;

	case off:
        dynamic_cast<SineWaveVoice*>(synth.getVoice(0))->setPitchModulation(0.0);
        cutoffModulationAmt = 0.0;
        
		break;
	default:
		break;
	}
}

float MonosynthPluginAudioProcessor::softClip(float s)
{
    float localSample = s;
    if (localSample > 0.3f)
    {
        localSample = 0.25f;
    }
    else if (localSample < -0.3f)
    {
        localSample = -0.25f;
        
    }
    else
    {
        localSample = localSample - ( ( localSample * localSample * localSample) * 0.75f );
    }
    return localSample;
}


double MonosynthPluginAudioProcessor::getLFOSyncedFreq(AudioPlayHead::CurrentPositionInfo posInfo, double division )
{
	const double beats_per_minute = posInfo.bpm;
	const double seconds_per_beat = 60.0 / beats_per_minute;
	const double seconds_per_note = seconds_per_beat * (lastPosInfo.timeSigDenominator / division);

	// double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;

	return 1.0 / seconds_per_note;
}



bool MonosynthPluginAudioProcessor::noteIsBeingPlayed()
{
	return ampEnvelope->getState() != ADSR::env_idle;
}

bool MonosynthPluginAudioProcessor::lfoSynced()
{
    return (*lfoSyncParam == 1);
	
}



