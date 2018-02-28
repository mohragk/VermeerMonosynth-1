/*
  ==============================================================================

    SequencerProcessor.cpp
    Created: 25 Feb 2018 12:41:20pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "SequencerProcessor.h"

SequencerProcessor::SequencerProcessor ( SequencerState& st ) : seqState(st), startTime(Time::getMillisecondCounter())
{
    seqState.addListener(this);
}

SequencerProcessor::~SequencerProcessor()
{
    seqState.removeListener(this);
}

void SequencerProcessor::handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if (isActive)
    {
        stepCount = 0;
        lastNotePlayed = midiNoteNumber;
        currentMidiChannel = midiChannel;
        
        startPulseClock();
        
        isPlaying = true;
    }
}


void SequencerProcessor::handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if(isActive)
    {
        isPlaying = false;
    }
}


void SequencerProcessor::processSequencer(int bufferSize)
{
    int numSamples = bufferSize;
    
    while (--numSamples >= 0)
    {
        if(isPlaying)
        {
            pulseClock.update();
            
            if(pulseClock.isPulseHigh())
            {
                playStep(stepCount);
            }
        }
        
        //CHECK IF NOTES SHOULD BE RELEASED
        for (int i = 0; i <= maxSteps; i++)
        {
            
            if (!step[i].isReleased)
            {
                int currentTime = static_cast<int>( std::round(Time::getMillisecondCounterHiRes() ) - startTime);
                int range = 16;
                
                if (step[i].timeStamp + step[i].noteLengthMillis > currentTime - range && step[i].timeStamp + step[i].noteLengthMillis < currentTime + range )
                {
                    int note = step[i].noteNumber;
                    seqState.noteOff(currentMidiChannel, note, 1.0f);
                    step[i].isReleased = true;
                    step[i].isActive = false;
                }
            }
        }
    }
}

void SequencerProcessor::playStep(int currentStep)
{
    int newNote = lastNotePlayed + stepPitchValue[currentStep];
    int pulseInterval = (1 / pulseClock.getFrequency()) * 1000;
    int releaseTime = std::round(  noteLength * pulseInterval );
    
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthMillis = releaseTime;
    step[currentStep].timeStamp = static_cast<int> ( std::round( Time::getMillisecondCounterHiRes() ) - startTime);
    step[currentStep].isReleased = false;
    step[currentStep].isActive = true;
    
    
    //send noteOn message
    seqState.noteOn(currentMidiChannel, newNote, 1.0f);
    
    
    stepCount++;
    
    if (stepCount > maxSteps)
        stepCount = 0;
}


void SequencerProcessor::startPulseClock()
{
    
    int division = (int)timeDivision;
    double pulseTimeHz = getPulseInHz(currentBPM, division);
    
    pulseClock.setFrequency(pulseTimeHz);
    pulseClock.resetModulo();
}


double SequencerProcessor::getPulseInHz(int bpm, int division)
{
    double beats_per_minute = 120;
    beats_per_minute = bpm;
    
    const double seconds_per_beat = 60.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * 4 / division;
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return 1.0 / seconds_per_note;
}
