/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

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
class Sequencer  : public Component,
                   public Slider::Listener
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
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void parentSizeChanged() override;
    
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

    
    
    
    
    

private:
    
    // members
    MonosynthPluginAudioProcessor& processor;
    //AudioPlayHead* playHead;
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    
    std::unique_ptr<Slider> pitchSlider[numSteps];
    std::unique_ptr<Slider> globalNoteLengthSlider;
    
    
    
    int bpm;
    
    double globalNoteLength = 1.0;

    //methods
    
    void setStepData(int s, double p, double nl, double nv )
    {
        step[s].pitch = p;
        step[s].noteLength = nl;
        step[s].normalValue = nv;
        
    };
    
    void updateClock();
    
    void updateSteps();
    
    void processSteps();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
