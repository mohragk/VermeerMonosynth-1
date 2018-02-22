/*
  ==============================================================================

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#ifndef SEQUENCER_H
#define SEQUENCER_H

#define SEQUENCER_HEIGHT 120

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "SequencerState.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/




class Sequencer  : public Component, public SequencerStateListener, private MultiTimer
{
public:
    //==============================================================================
    Sequencer (MonosynthPluginAudioProcessor& p, SequencerState& s);
    ~Sequencer();
    
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void parentSizeChanged() override;
    void timerCallback(int timerID) override;
    
    void handleSequencerNoteOn (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleSequencerNoteOff (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    
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
    
    enum timers
    {
        PULSECLOCK_TIMER = 0,
        releaseTimerStep1,
        releaseTimerStep2,
        releaseTimerStep3,
        releaseTimerStep4,
        releaseTimerStep5,
        releaseTimerStep6,
        releaseTimerStep7,
        releaseTimerStep8,
        displayTimer,
        hiFreqTimer,
        numTimers
    };

    struct Step
    {
        int    stepNumber;
        int    noteNumber;
        int    timeStamp;
        int    noteLengthMillis;
        bool   isReleased;
    } step[numSteps];

    
    Step getStepData(int s) { return step[s]; };
    
    
    

private:
    
    class StepSlider;
    // members
    MonosynthPluginAudioProcessor& processor;
    //MidiKeyBoardState& keyState;
    SequencerState& state;

    
    std::unique_ptr<StepSlider> pitchSlider[numSteps];
    std::unique_ptr<StepSlider> globalNoteLengthSlider;
    std::unique_ptr<StepSlider> stepDivision;
    
    std::unique_ptr<Label> stepDivisionLabel, globalNoteLengthLabel;
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
    int lastNotePlayed;
    int currentMidiChannel;
    int stepCount = 0;
    
    bool isPlaying = false;
    bool isActive;
    
    // methods
    void updateStepDivisionLabel();
    void updateGlobalNoteLengthLabel();
	void updateStepKnobColour(int step);
    
    void playStep(int currentStep);
    
    void startPulseClock(int timeMillis);
    void stopPulseClock();
    
    
    
    void stepNoteOff(int step);
    



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

#endif // SEQUENCER_H
