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
#include "ParameterSlider.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Sequencer::Sequencer (MonosynthPluginAudioProcessor& p) : processor(p)
{
    typedef ParameterSlider::style knobStyle;
    
    Pitch1Slider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerPitch1Param, knobStyle(2)));
    addAndMakeVisible (Pitch1Slider.get());  //

   

    Pitch1Slider->setBounds (64, 0, 24, 112);

    addAndMakeVisible (Pitch2Slider = new Slider ("Pitch 2"));
    Pitch2Slider->setRange (0, 10, 0);
    Pitch2Slider->setSliderStyle (Slider::LinearVertical);
    Pitch2Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch2Slider->addListener (this);

    Pitch2Slider->setBounds (96, 0, 24, 112);

    addAndMakeVisible (Pitch3Slider = new Slider ("Pitch 3"));
    Pitch3Slider->setRange (0, 10, 0);
    Pitch3Slider->setSliderStyle (Slider::LinearVertical);
    Pitch3Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch3Slider->addListener (this);

    Pitch3Slider->setBounds (128, 0, 24, 112);

    addAndMakeVisible (Pitch4Slider = new Slider ("Pitch 4"));
    Pitch4Slider->setRange (0, 10, 0);
    Pitch4Slider->setSliderStyle (Slider::LinearVertical);
    Pitch4Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch4Slider->addListener (this);

    Pitch4Slider->setBounds (160, 0, 24, 112);

    addAndMakeVisible (Pitch5Slider = new Slider ("Pitch 5"));
    Pitch5Slider->setRange (0, 10, 0);
    Pitch5Slider->setSliderStyle (Slider::LinearVertical);
    Pitch5Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch5Slider->addListener (this);

    Pitch5Slider->setBounds (208, 0, 24, 112);

    addAndMakeVisible (Pitch6Slider = new Slider ("Pitch 6"));
    Pitch6Slider->setRange (0, 10, 0);
    Pitch6Slider->setSliderStyle (Slider::LinearVertical);
    Pitch6Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch6Slider->addListener (this);

    Pitch6Slider->setBounds (240, 0, 24, 112);

    addAndMakeVisible (Pitch7Slider = new Slider ("Pitch 7"));
    Pitch7Slider->setRange (0, 10, 0);
    Pitch7Slider->setSliderStyle (Slider::LinearVertical);
    Pitch7Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch7Slider->addListener (this);

    Pitch7Slider->setBounds (272, 0, 24, 112);

    addAndMakeVisible (Pitch8Slider = new Slider ("Pitch 8"));
    Pitch8Slider->setRange (0, 10, 0);
    Pitch8Slider->setSliderStyle (Slider::LinearVertical);
    Pitch8Slider->setTextBoxStyle (Slider::TextBoxBelow, true, 24, 20);
    Pitch8Slider->addListener (this);

    Pitch8Slider->setBounds (304, 0, 24, 112);

    addAndMakeVisible (syncToTempoSwitchSlider = new Slider ("Sync to Tempo Slider"));
    syncToTempoSwitchSlider->setRange (0, 10, 0);
    syncToTempoSwitchSlider->setSliderStyle (Slider::LinearHorizontal);
    syncToTempoSwitchSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    syncToTempoSwitchSlider->addListener (this);

    syncToTempoSwitchSlider->setBounds (8, 8, 48, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (890, 160);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Sequencer::~Sequencer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    Pitch1Slider = nullptr;
    Pitch2Slider = nullptr;
    Pitch3Slider = nullptr;
    Pitch4Slider = nullptr;
    Pitch5Slider = nullptr;
    Pitch6Slider = nullptr;
    Pitch7Slider = nullptr;
    Pitch8Slider = nullptr;
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
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Sequencer::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == Pitch2Slider)
    {
        //[UserSliderCode_Pitch2Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch2Slider]
    }
    else if (sliderThatWasMoved == Pitch3Slider)
    {
        //[UserSliderCode_Pitch3Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch3Slider]
    }
    else if (sliderThatWasMoved == Pitch4Slider)
    {
        //[UserSliderCode_Pitch4Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch4Slider]
    }
    else if (sliderThatWasMoved == Pitch5Slider)
    {
        //[UserSliderCode_Pitch5Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch5Slider]
    }
    else if (sliderThatWasMoved == Pitch6Slider)
    {
        //[UserSliderCode_Pitch6Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch6Slider]
    }
    else if (sliderThatWasMoved == Pitch7Slider)
    {
        //[UserSliderCode_Pitch7Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch7Slider]
    }
    else if (sliderThatWasMoved == Pitch8Slider)
    {
        //[UserSliderCode_Pitch8Slider] -- add your slider handling code here..
        //[/UserSliderCode_Pitch8Slider]
    }
    else if (sliderThatWasMoved == syncToTempoSwitchSlider)
    {
        //[UserSliderCode_syncToTempoSwitchSlider] -- add your slider handling code here..
        //[/UserSliderCode_syncToTempoSwitchSlider]
    }

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
