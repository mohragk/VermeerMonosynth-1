/*
  ==============================================================================

    SequencerProcessor.h
    Created: 25 Feb 2018 12:21:07pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef SEQUENCER_PROCESSOR_H
#define SEQUENCER_PROCESSOR_H

#include "../JuceLibraryCode/JuceHeader.h"


#include "SequencerState.h"
#include "PulseClock.h"




class SequencerProcessor :  public SequencerStateListener
{
public:
    SequencerProcessor( SequencerState& st );
    
    
    ~SequencerProcessor();
    
    enum steps
    {
        STEP1 = 0,
        STEP2,
        STEP3,
        STEP4,
        STEP5,
        STEP6,
        STEP7,
        STEP8,
        numSteps
        
    };
    
    struct Step
    {
        int    stepNumber = 0;
        int    noteNumber = 60;
        int    timeStamp = 0;
        int    noteLengthMillis = 10;
        bool   isReleased = false;
        bool   isActive = false;
    } step[numSteps];
    
   
    
    void handleSequencerNoteOn (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleSequencerNoteOff (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    int getStepCount() { return stepCount; };
    
    void makeActive(bool on)
    {
        isActive = on;
    }
    
    bool isActivated(){ return isActive; };
    
    void processSequencer(int bufferSize);
    
    void setStepData(int curStep, int note, int noteLen, int time, bool isRel, bool isAct)
    {
        step[curStep].stepNumber = curStep;
        step[curStep].noteNumber = note;
        step[curStep].noteLengthMillis = noteLen;
        step[curStep].timeStamp = time;
        step[curStep].isReleased = isRel;
        step[curStep].isActive = isAct;
    }
    
    Step getStepData(int s) { return step[s]; };
    
    void setGlobalNoteLength (double noteLen)
    {
        noteLength = noteLen;
    }
    
    void setStepPitch(int st, int pitch)
    {
        stepPitchValue[st] = pitch;
    }
    
    void setTimeDivision (double division)
    {
        timeDivision = division;
    }
    
    void setPulseClockSampleRate( double sr)
    {
        pulseClock.setSampleRate(sr);
    }
    
    
    void setBPM (int bpm)
    {
        currentBPM = bpm;
    }
    
    void setMaxSteps (int max)
    {
        maxSteps = max;
    }
    
private:
    
    SequencerState& seqState;
    
    PulseClock pulseClock;
    
    int lastNotePlayed = 60;
    int currentMidiChannel = 1;
    int stepCount = 0;
    int maxSteps = 8;
    
    bool isPlaying = false;
    bool isActive = false;
    
    void playStep(int currentStep);
    
    
    void startPulseClock();
    void stopPulseClock();
    
    double getPulseInHz( int bpm, int division );
    int startTime;
    
    int stepPitchValue[numSteps];
    double noteLength;
    double timeDivision;
    int currentBPM;
    
    JUCE_LEAK_DETECTOR (SequencerProcessor)
    
};




#endif // SEQUENCER_PROCESSOR_H
