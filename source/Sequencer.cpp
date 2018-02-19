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


#include "Sequencer.h"


Sequencer::Sequencer (MonosynthPluginAudioProcessor& p) : processor(p)

{
    typedef ParameterSlider::style knobStyle;

    //globalNoteLengthSlider = std::unique_ptr<ParameterSlider> (new ParameterSlider(processor.));
   // addAndMakeVisible (globalNoteLengthSlider.get());
   // globalNoteLengthSlider->setRange (0, 1, 0);
    //globalNoteLengthSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    //globalNoteLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
   // globalNoteLengthSlider->addListener (this);
    
    for (int i = 0; i < numSteps; i++)
    {
        pitchSlider[i] = std::unique_ptr<ParameterSlider> ( new ParameterSlider (*processor.stepPitchParam[i], knobStyle(LINEARVERTICAL) ) );
        addAndMakeVisible (pitchSlider[i].get());
    }
    
    setSize (890, 80);
}

Sequencer::~Sequencer()
{
 
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
    
    Rectangle<int> strip ( area.removeFromTop(84).reduced(24, 0) );
    
    {
        Rectangle<int> block (strip.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        for (int i =0; i < numSteps; i++)
        {
            pitchSlider[i]->setBounds(block.removeFromLeft(vertSliderSize));
        }
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        //globalNoteLengthSlider->setBounds(block.removeFromLeft(rotarySize));
    }
}

void Sequencer::parentSizeChanged()
{
    
}




void Sequencer::updateSteps()
{
    for (int s = 0; s < numSteps; s++)
    {
        double normalised = (pitchSlider[s]->getValue() + 12) / 24;
        setStepData(s, pitchSlider[s]->getValue(), globalNoteLength, normalised);
    }
    
}

/*
void Sequencer::processSteps(MidiBuffer& midiBuffer, size_t numSamples, double sampleRate)
{
   
}
*/

/*
void Sequencer::addStepToMidiSequence(Step s)
{
    int middleNote = 60;
    int newNote = middleNote + s.pitch;
    double noteLength = s.noteLength;
    
    MidiMessage messageOn = MidiMessage::noteOn (midiChannel, newNote, (uint8) 100);
    messageOn.setTimeStamp ( ( Time::getMillisecondCounterHiRes() * 0.001 - startTime ));
    
    midiSequence.addEvent(messageOn);
    
    MidiMessage messageOff (MidiMessage::noteOff (messageOn.getChannel(), messageOn.getNoteNumber()));
    messageOff.setTimeStamp (messageOn.getTimeStamp() + noteLength);
    
    midiSequence.addEvent(messageOff);
}
*/

/*
double Sequencer::getMilliSecondsPerNote(AudioPlayHead::CurrentPositionInfo pInfo, double division )
{
    const double beats_per_minute = pInfo.bpm;
    const double seconds_per_beat = 6000.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * (pInfo.timeSigDenominator / division);
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return seconds_per_note;
}
*/


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


