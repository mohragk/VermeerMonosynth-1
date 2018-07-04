/*
  ==============================================================================


  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/


#include "Sequencer.h"

class Sequencer::StepSlider : public Slider, private Timer
{
public:
    StepSlider(int type, AudioProcessorParameter& p) : Slider(p.getName(256)), paramInt(p)
    {
        if (type == ROTARY) { setSliderStyle(Slider::RotaryVerticalDrag); }
        else if (type == LINEARHORIZONTAL) {setSliderStyle(Slider::LinearHorizontal);}
        else { setSliderStyle(Slider::LinearVertical); }
        
        setTextBoxStyle(Slider::TextBoxBelow, true, 36, 18);

        setColour(thumbColourId, darkThumb);
        
        startTimerHz(60);
        updateSliderPos();
        setRange(0,1,0);
    };
    ~StepSlider() {};
    
    void startedDragging() override     { paramInt.beginChangeGesture(); setColour(thumbColourId, lightThumb); }
    void stoppedDragging() override     { paramInt.endChangeGesture();   setColour(thumbColourId, darkThumb);  }
    
    void timerCallback () override { updateSliderPos(); };
    
    void valueChanged() override
    {
        if (isMouseButtonDown())
        {
            paramInt.setValueNotifyingHost(Slider::getValue());
        }
        else
        {
            paramInt.setValue(Slider::getValue());
        }
        
    }
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
private:
   // String sliderName;
    
    AudioProcessorParameter&   paramInt;
   // float paramFloat;
    
    void updateSliderPos()
    {
        const int newValue = paramInt.getValue();
        
        if (newValue != Slider::getValue() && ! isMouseButtonDown())
        {
            Slider::setValue (newValue);
        }
    }
    
};

Sequencer::Sequencer (MonosynthPluginAudioProcessor& p, SequencerState& st) : processor(p), sequencerState(st)

{
    typedef StepSlider::style knobStyle;

    globalNoteLengthSlider = std::unique_ptr<ParameterSlider> (new ParameterSlider( *processor.stepNoteLengthParam, knobStyle(ROTARY) ));
    addAndMakeVisible (globalNoteLengthSlider.get());
   
    for (int i = 0; i < numSteps; i++)
    {
        pitchSlider[i] = std::unique_ptr<ParameterSlider> ( new ParameterSlider (  *processor.stepPitchParam[i], knobStyle(LINEARVERTICAL) ) );
        addAndMakeVisible (pitchSlider[i].get());
    }
    
	stepDivision = std::unique_ptr<ParameterSlider>(new ParameterSlider(  *processor.stepDivisionFloatParam, knobStyle(ROTARY)));
	addAndMakeVisible(stepDivision.get());
    
    maxStepsSlider = std::unique_ptr<ParameterSlider> ( new ParameterSlider (*processor.maxStepsParam, knobStyle(LINEARHORIZONTAL)));
    addAndMakeVisible(maxStepsSlider.get());
    
    
    setSize (890, SEQUENCER_HEIGHT);
    
    
	startTimerHz(120);
    
    
}

Sequencer::~Sequencer()
{
    
}


void Sequencer::makeActive(bool on)
{
    isActive = on;
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
    int vertSliderSize = 36;
    int horSliderSize = 18;
    int marginX = 12;
    
    Rectangle<int> strip ( area.removeFromTop(SEQUENCER_HEIGHT - horSliderSize).reduced(24, 8) );
    
    {
        Rectangle<int> block (strip.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        for (int i =0; i < numSteps; i++)
        {
            pitchSlider[i]->setBounds(block.removeFromLeft(vertSliderSize));
			pitchSlider[i]->setColour(Slider::textBoxOutlineColourId, Colour(0xff323e44));
            pitchSlider[i]->setTextBoxStyle (Slider::TextBoxBelow, true, 60, 14);
            pitchSlider[i]->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));

        }
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        globalNoteLengthSlider->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        globalNoteLengthSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 60, 14);
        globalNoteLengthSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        stepDivision->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        stepDivision->setTextBoxStyle (Slider::TextBoxBelow, true, 60, 14);
        stepDivision->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    }
    
    Rectangle<int> stripBottom ( area.removeFromTop(horSliderSize).reduced(24, 0) );
    
    {
        Rectangle<int> block (stripBottom.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        maxStepsSlider->setBounds(block.removeFromLeft(vertSliderSize * numSteps).reduced(8, 0));
    }
}

void Sequencer::parentSizeChanged()
{
    
}





void Sequencer::timerCallback()
{
    updateStepKnobColours();
    updateStepSliderAlpha();
}





void Sequencer::updateGlobalNoteLengthLabel()
{
    
}

void Sequencer::updateStepDivisionLabel()
{
    
}


void Sequencer::updateStepKnobColours()
{
    for (int i = 0; i < numSteps; i++)
    {
        if (sequencerState.getStepData(i).isActive == true)
            pitchSlider[i].get()->setColour(Slider::thumbColourId, lightThumb);
        else
            pitchSlider[i].get()->setColour(Slider::thumbColourId, darkThumb);
    }
}

void Sequencer::updateStepSliderAlpha()
{
    for (int i = 0; i < 8; i++)
    if (i <= *processor.maxStepsParam)
        pitchSlider[i].get()->setAlpha(1.0);
    else
    {
        pitchSlider[i].get()->setAlpha(0.3);
    }
    
}




//==============================================================================



