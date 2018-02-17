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



private:
    MonosynthPluginAudioProcessor& processor;

    //==============================================================================
    std::unique_ptr<Slider> Pitch1Slider,
                            Pitch2Slider,
                            Pitch3Slider,
                            Pitch4Slider,
                            Pitch5Slider,
                            Pitch6Slider,
                            Pitch7Slider,
                            Pitch8Slider;
    
    ScopedPointer<Slider> syncToTempoSwitchSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sequencer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
