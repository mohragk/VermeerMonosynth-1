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
#define SEQUENCER_HEIGHT 180


#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ParameterSlider.h"
#include "ParamToggleButton.h"
#include "SequencerState.h"
#include "MonosynthLookAndFeel.h"




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
    Sequencer (MonosynthPluginAudioProcessor& p, SequencerState& st);
    ~Sequencer();
    
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void parentSizeChanged() override;
    void timerCallback() override;
    
    
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
    
   


    
    
    
    
    

private:
    
    class StepSlider;
	//class ParamToggleButton;
    // members
    MonosynthPluginAudioProcessor& processor;
    SequencerState& sequencerState;

    CriticalSection lock;

    
    std::unique_ptr<ParameterSlider> pitchSlider[numSteps];
	std::unique_ptr<ParamToggleButton>	 skipStepButton[numSteps];
    std::unique_ptr<ParameterSlider> globalNoteLengthSlider;
    std::unique_ptr<ParameterSlider> stepDivision;
    std::unique_ptr<ParameterSlider> maxStepsSlider;
	std::unique_ptr<ParameterSlider> swingAmountSlider;

    
    std::unique_ptr<Label> stepDivisionLabel, globalNoteLengthLabel, swingAmountLabel;
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
    int stepCount = 0;
    
   
    bool isActive = false;
    
    // methods
    void updateStepDivisionLabel();
    void updateGlobalNoteLengthLabel();
	void updateStepKnobColours();
    void updateStepSliderAlpha();
   
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

#endif // SEQUENCER_H
