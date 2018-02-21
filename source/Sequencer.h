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

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ParameterSlider.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Sequencer  : public Component, private Timer
{
public:
    //==============================================================================
    Sequencer (MonosynthPluginAudioProcessor& p);
    ~Sequencer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    
    void parentSizeChanged() override;
	void timerCallback() override;
    
    
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
        //int stepId;
        double pitch;
        double noteLength;
        double normalValue;
    } step[numSteps];

    
    Step getStepData(int s) { return step[s]; };
    
    
    
    

private:
    
    // members
    MonosynthPluginAudioProcessor& processor;
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    void updateStepDivisionLabel();
    void updateGlobalNoteLengthLabel();
    
    std::unique_ptr<ParameterSlider> pitchSlider[numSteps];
    std::unique_ptr<ParameterSlider> globalNoteLengthSlider;
	std::unique_ptr<ParameterSlider> stepDivision;
    
	std::unique_ptr<Label> stepDivisionLabel, globalNoteLengthLabel;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

#endif // SEQUENCER_H
