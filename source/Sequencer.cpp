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
: processor(p)


{
    typedef ParameterSlider::style knobStyle;
    
    

    addAndMakeVisible (syncToTempoSwitchSlider = new Slider ("Sync to Tempo Slider"));
    syncToTempoSwitchSlider->setRange (0, 10, 0);
    syncToTempoSwitchSlider->setSliderStyle (Slider::LinearHorizontal);
    syncToTempoSwitchSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    syncToTempoSwitchSlider->addListener (this);

    syncToTempoSwitchSlider->setBounds (8, 8, 48, 24);


    //[UserPreSize]
    //[/UserPreSize]

    

    globalNoteLengthSlider = std::unique_ptr<Slider> (new Slider("Global Note Length"));
    addAndMakeVisible (globalNoteLengthSlider.get());
    globalNoteLengthSlider->setRange (0, 1, 0);
    globalNoteLengthSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    globalNoteLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    globalNoteLengthSlider->addListener (this);
    
    for (int i = 0; i < numSteps; i++)
    {
        regPitchSlider[i] = std::unique_ptr<Slider> ( new Slider ("Seq Pitch Slider" + std::to_string(i) ) );
        addAndMakeVisible (regPitchSlider[i].get());
        regPitchSlider[i]->setRange (-12, 12, 1);
        regPitchSlider[i]->setSliderStyle (Slider::LinearVertical);
        regPitchSlider[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 18, 18);
        regPitchSlider[i]->addListener (this);
    }

    setSize (890, 80);
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
    int vertSliderSize = 18;
    int marginX = 8;
    int marginY = 8;
    
    Rectangle<int> strip (area.removeFromTop(rotarySize + marginY * 2));
    
    syncToTempoSwitchSlider->setBounds(strip.removeFromLeft(rotarySize));
    
    
    
    {
        Rectangle<int> block (strip.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        for (int i =0; i < numSteps; i++)
        {
            regPitchSlider[i]->setBounds(block.removeFromLeft(vertSliderSize));
        }
        
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        
        globalNoteLengthSlider->setBounds(block.removeFromLeft(rotarySize));
        
        
    }
    
    
    
    
    
}

void Sequencer::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if(sliderThatWasMoved == syncToTempoSwitchSlider)
    {
        
    }
    
    if(sliderThatWasMoved == globalNoteLengthSlider.get())
    {
        globalNoteLength = globalNoteLengthSlider->getValue();
        updateSteps();
    }
    
    for(int i = 0 ; i < numSteps; i++)
    {
        if(sliderThatWasMoved == regPitchSlider[i].get())
        {
            updateSteps();
            
            std::cout << "Step " << i << " pitch: " << regPitchSlider[i]->getValue() << " | notelength: " << globalNoteLength << std::endl;
            
        }
    }
    
}

void Sequencer::parentSizeChanged()
{
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}

void Sequencer::updateSteps()
{
    for (int s = 0; s < numSteps; s++)
        setStepData(s, regPitchSlider[s]->getValue(), globalNoteLength, 0.0);
    
}

void Sequencer::processSteps()
{
    
}


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
  <SLIDER name="pitch 1" id="ee9067c6c91788c7" memberName="pitch1Slider"
          virtualName="" explicitFocusOrder="0" pos="64 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 2" id="eccb3fa4050bcfe" memberName="pitch2Slider"
          virtualName="" explicitFocusOrder="0" pos="96 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 3" id="b5c4e7df968d9032" memberName="pitch3Slider"
          virtualName="" explicitFocusOrder="0" pos="128 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 4" id="f28ef3132e6d3c75" memberName="pitch4Slider"
          virtualName="" explicitFocusOrder="0" pos="160 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 5" id="6ce1113c52e7da19" memberName="pitch5Slider"
          virtualName="" explicitFocusOrder="0" pos="208 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 6" id="3af366c3f76fd393" memberName="pitch6Slider"
          virtualName="" explicitFocusOrder="0" pos="240 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 7" id="4249bb79ecc5e269" memberName="pitch7Slider"
          virtualName="" explicitFocusOrder="0" pos="272 0 24 112" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="0" textBoxWidth="24"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="pitch 8" id="fbfe95442c72732a" memberName="pitch8Slider"
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
