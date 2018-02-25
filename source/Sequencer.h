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
#include "ParameterSlider.h"
#include "SequencerState.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/




class Sequencer  : public Component, public SequencerStateListener, private Timer
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
    void timerCallback() override;
    
    
    void processSequencer(int bufferSize);
    
    
    void handleSequencerNoteOn (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleSequencerNoteOff (SequencerState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    void makeActive(bool on);
    bool isActivated(){ return isActive;};
    
    
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
        displayTimer,
        hiFreqTimer,
        numTimers
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

    
    Step getStepData(int s) { return step[s]; };
    
    
    

private:
    
    class StepSlider;
    
    // members
    MonosynthPluginAudioProcessor& processor;
    SequencerState& state;
    
    PulseClock pulseClock;
    
    CriticalSection lock;

    
    std::unique_ptr<ParameterSlider> pitchSlider[numSteps];
    std::unique_ptr<ParameterSlider> globalNoteLengthSlider;
    std::unique_ptr<ParameterSlider> stepDivision;
    
    std::unique_ptr<Label> stepDivisionLabel, globalNoteLengthLabel;
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
    int lastNotePlayed = 60;
    int currentMidiChannel = 1;
    int stepCount = 0;
    
    bool isPlaying = false;
    bool isActive = false;
    
    // methods
    void updateStepDivisionLabel();
    void updateGlobalNoteLengthLabel();
	void updateStepKnobColour(int curStep);
    
    void playStep(int currentStep);
    
    void startPulseClock();
    void stopPulseClock();
    
    double getPulseInHz( AudioPlayHead::CurrentPositionInfo posInfo, int division );
    int startTime;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

#endif // SEQUENCER_H
