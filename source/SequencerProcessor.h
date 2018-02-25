/*
  ==============================================================================

    SequencerProcessor.h
    Created: 25 Feb 2018 12:21:07pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef SEQUENCER_PROCESSOR_H
#define SEQUENCER_PROCESSOR_H



#include "PluginProcessor.h"

class SequencerProcessor : public MidiKeyboardStateListener , private SequencerStateListener
{
public:
    SequencerProcessor(MonosynthPluginAudioProcessor& p, SequencerState& st);
    
    
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
    
    Step getStepData(int s) { return step[s]; };
    
    void makeActive(bool on)
    {
        isActive = on;
    }
    
    bool isActivated(){ return isActive; };
    
    void processSequencer(int bufferSize);
    
    
private:
    
    MonosynthPluginAudioProcessor& processor;
    
    SequencerState& seqState;
    
    PulseClock pulseClock;
    
    int lastNotePlayed = 60;
    int currentMidiChannel = 1;
    int stepCount = 0;
    
    bool isPlaying = false;
    bool isActive = false;
    
    void playStep(int currentStep);
    
    
    void startPulseClock();
    void stopPulseClock();
    
    double getPulseInHz( AudioPlayHead::CurrentPositionInfo posInfo, int division );
    int startTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerProcessor)
    
};




#endif // SEQUENCER_PROCESSOR_H
