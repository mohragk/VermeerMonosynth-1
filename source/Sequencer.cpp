/*
  ==============================================================================


  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/


#include "Sequencer.h"

#define SEQUENCER_HEIGHT 84


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
    
    setSize (890, SEQUENCER_HEIGHT);
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
    
    Rectangle<int> strip ( area.removeFromTop(SEQUENCER_HEIGHT).reduced(24, 0) );
    
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



