/*
  ==============================================================================


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

    globalNoteLengthSlider = std::unique_ptr<ParameterSlider> (new ParameterSlider(*processor.stepNoteLengthParam, knobStyle(ROTARY)));
    addAndMakeVisible (globalNoteLengthSlider.get());
 
    
    globalNoteLengthLabel = std::unique_ptr<Label>(new Label("StepDiv Label", TRANS("Div")));
    addAndMakeVisible(globalNoteLengthLabel.get());
    globalNoteLengthLabel->setFont(Font("", 12.00f, Font::plain).withExtraKerningFactor(0.108f));
    globalNoteLengthLabel->setJustificationType(Justification::centredTop);
    globalNoteLengthLabel->setEditable(false, false, false);
    globalNoteLengthLabel->setColour(TextEditor::textColourId, Colours::black);
    globalNoteLengthLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    
    for (int i = 0; i < numSteps; i++)
    {
        pitchSlider[i] = std::unique_ptr<ParameterSlider> ( new ParameterSlider (*processor.stepPitchParam[i], knobStyle(LINEARVERTICAL) ) );
        addAndMakeVisible (pitchSlider[i].get());
    }
    
	stepDivision = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.sequencerStepDivisionParam, knobStyle(ROTARY)));
	addAndMakeVisible(stepDivision.get());

	stepDivisionLabel = std::unique_ptr<Label>(new Label("StepDiv Label", TRANS("Div")));
	addAndMakeVisible(stepDivisionLabel.get());
	stepDivisionLabel->setFont(Font("", 12.00f, Font::plain).withExtraKerningFactor(0.108f));
	stepDivisionLabel->setJustificationType(Justification::centredTop);
	stepDivisionLabel->setEditable(false, false, false);
	stepDivisionLabel->setColour(TextEditor::textColourId, Colours::black);
	stepDivisionLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    setSize (890, SEQUENCER_HEIGHT);

	startTimerHz(60);
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
    int vertSliderSize = 36;
    int marginX = 12;
    
    Rectangle<int> strip ( area.removeFromTop(SEQUENCER_HEIGHT).reduced(24, 0) );
    
    {
        Rectangle<int> block (strip.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        for (int i =0; i < numSteps; i++)
        {
            pitchSlider[i]->setBounds(block.removeFromLeft(vertSliderSize));
			pitchSlider[i]->setTextBoxStyle(Slider::TextBoxBelow, true, vertSliderSize, 18);
			pitchSlider[i]->setColour(Slider::textBoxOutlineColourId, Colour(0xff323e44));
        }
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        globalNoteLengthSlider->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        
        globalNoteLengthLabel->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.25 ) );
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        stepDivision->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        
        stepDivisionLabel->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.25)  );
    }
}

void Sequencer::parentSizeChanged()
{
    
}

void Sequencer::timerCallback()
{
    
    updateGlobalNoteLengthLabel();
    updateStepDivisionLabel();

	

	if (processor.isSequencerPlaying())
	{
		int currentStep = processor.getCurrentStep() - 1; //I DUNNO WHY!

		for (int step = 0; step < numSteps; step++)
		{
			if(step == currentStep)
				pitchSlider[currentStep].get()->setColour(Slider::thumbColourId, Colour(0xffdee5fc));
			else
				pitchSlider[step].get()->setColour(Slider::thumbColourId, Colour(0xff3e7db3));
		}
		
	}
	else
	{
		for (int step = 0; step < numSteps; step++)
			pitchSlider[step].get()->setColour(Slider::thumbColourId, Colour(0xff3e7db3));
	}

}



void Sequencer::updateGlobalNoteLengthLabel()
{
    int val = processor.globalNoteLengthVal * 100;
    String s = std::to_string(val) + " %";
    globalNoteLengthLabel->setText(s, dontSendNotification);
}

void Sequencer::updateStepDivisionLabel()
{
    int val = processor.sequencerStepDivisionVal;
    String s = "1/" + std::to_string(val) + "th";
    stepDivisionLabel->setText(s, dontSendNotification);
}
//==============================================================================



