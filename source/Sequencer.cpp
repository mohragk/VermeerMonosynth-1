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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "Sequencer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Sequencer::Sequencer (MonosynthPluginAudioProcessor& p) 
: processor(p),
                                Pitch1Slider(nullptr),
                                Pitch2Slider(nullptr),
                                Pitch3Slider(nullptr),
                                Pitch4Slider(nullptr),
                                Pitch5Slider(nullptr),
                                Pitch6Slider(nullptr),
                                Pitch7Slider(nullptr),
                                Pitch8Slider(nullptr)


{
    typedef ParameterSlider::style knobStyle;
    
    Pitch1Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch1Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch1Slider.get());  //
    
    Pitch2Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch2Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch2Slider.get());  //

    Pitch3Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch3Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch3Slider.get());  //

    Pitch4Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch4Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch4Slider.get());  //

    Pitch5Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch5Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch5Slider.get());  //

    Pitch6Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch6Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch6Slider.get());  //

    Pitch7Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch7Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch7Slider.get());  //

    Pitch8Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch8Param, knobStyle(ROTARY)));
    addAndMakeVisible (Pitch8Slider.get());  //

    addAndMakeVisible (syncToTempoSwitchSlider = new Slider ("Sync to Tempo Slider"));
    syncToTempoSwitchSlider->setRange (0, 10, 0);
    syncToTempoSwitchSlider->setSliderStyle (Slider::LinearHorizontal);
    syncToTempoSwitchSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    syncToTempoSwitchSlider->addListener (this);

    syncToTempoSwitchSlider->setBounds (8, 8, 48, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (890, 80);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Sequencer::~Sequencer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    syncToTempoSwitchSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Sequencer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Sequencer::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int rotarySize = 60;
    int marginX = 24;
    int marginY = 8;
    
    Rectangle<int> strip (area.removeFromTop(rotarySize + marginY * 2));
    
    syncToTempoSwitchSlider->setBounds(strip.removeFromLeft(rotarySize));
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize * 4) + marginX) );
        Pitch1Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch2Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch3Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch4Slider->setBounds(block.removeFromLeft(rotarySize));
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize * 4) + marginX) );
        Pitch5Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch6Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch7Slider->setBounds(block.removeFromLeft(rotarySize));
        Pitch8Slider->setBounds(block.removeFromLeft(rotarySize));
    }
    
}

void Sequencer::sliderValueChanged (Slider* sliderThatWasMoved)
{
    
    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Sequencer::parentSizeChanged()
{
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Sequencer" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="890" initialHeight="160">
  <METHODS>
    <METHOD name="parentSizeChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <SLIDER name="Pitch 1" id="ee9067c6c91788c7" memberName="Pitch1Slider"
          virtualName="" explicitFocusOrder="0" pos="64 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 2" id="eccb3fa4050bcfe" memberName="Pitch2Slider"
          virtualName="" explicitFocusOrder="0" pos="96 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 3" id="b5c4e7df968d9032" memberName="Pitch3Slider"
          virtualName="" explicitFocusOrder="0" pos="128 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 4" id="f28ef3132e6d3c75" memberName="Pitch4Slider"
          virtualName="" explicitFocusOrder="0" pos="160 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 5" id="6ce1113c52e7da19" memberName="Pitch5Slider"
          virtualName="" explicitFocusOrder="0" pos="208 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 6" id="3af366c3f76fd393" memberName="Pitch6Slider"
          virtualName="" explicitFocusOrder="0" pos="240 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 7" id="4249bb79ecc5e269" memberName="Pitch7Slider"
          virtualName="" explicitFocusOrder="0" pos="272 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Pitch 8" id="fbfe95442c72732a" memberName="Pitch8Slider"
          virtualName="" explicitFocusOrder="0" pos="304 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="Sync to Tempo Slider" id="80d83cf5bbcf7404" memberName="syncToTempoSwitchSlider"
          virtualName="" explicitFocusOrder="0" pos="8 8 48 24" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
