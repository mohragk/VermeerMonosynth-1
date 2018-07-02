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

#include "PulseClock.h"




class SequencerProcessor :  public MidiKeyboardStateListener
{
public:
    SequencerProcessor( MidiKeyboardState& ks );
    
    
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
        uint32 timeStamp = 0;
        int    noteLengthTicks = 0;
        bool   isReleased = false;
        bool   isActive = false;
    } step[numSteps];
    
    void setSampleRate(double sr)
    {
        sampleRate = sr;
    }
    
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    int getStepCount() { return stepCount; };
    
    void makeActive(bool on)
    {
        isActive = on;
    }
    
    bool isActivated(){ return isActive; };
    
    void processSequencer(MidiBuffer& midBuf, int bufferSize, bool useSequencer);
    
    void setStepData(int curStep, int note, int noteLen, int time, bool isRel, bool isAct)
    {
        step[curStep].stepNumber = curStep;
        step[curStep].noteNumber = note;
        step[curStep].noteLengthTicks = noteLen;
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
    
    CriticalSection lock;
    
    MidiKeyboardState& keyState;
    
    PulseClock pulseClock;
    
    int lastNotePlayed = 60;
    int currentMidiChannel = 1;
    int stepCount;
    int maxSteps;
    
    bool isPlaying;
    bool isActive;
    
	int stepPitchValue[numSteps];
	double noteLength;
	double timeDivision;
	int currentBPM;

	uint32 globalSampleCount;
	double sampleRate;

    void playStep(MidiBuffer& midBuf, int currentStep, int curSample);
    
    void startPulseClock();
    
    double getPulseInHz( int bpm, int division );
    int getPulseInSamples(int bpm, int division, double sr );
   
    
   
    
    JUCE_LEAK_DETECTOR (SequencerProcessor)
    
};




#endif // SEQUENCER_PROCESSOR_H
