/*
  ==============================================================================


  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/


#include "Sequencer.h"

class Sequencer::StepSlider : public Slider
{
public:
    StepSlider(String n, int type) : Slider(n), sliderName(n)
    {
        if (type == ROTARY) { setSliderStyle(Slider::RotaryVerticalDrag); }
        else if (type == LINEARHORIZONTAL) {setSliderStyle(Slider::LinearHorizontal);}
        else { setSliderStyle(Slider::LinearVertical); }
        
        setTextBoxStyle(Slider::TextBoxBelow, true, 36, 18);

        setColour(thumbColourId, darkThumb);
    };
    ~StepSlider() {};
    
    void startedDragging() override     { setColour(thumbColourId, lightThumb); }
    void stoppedDragging() override     { setColour(thumbColourId, darkThumb);  }
    
    enum style
    {
        ROTARY = 0,
        LINEARHORIZONTAL,
        LINEARVERTICAL
    };
    
private:
    String sliderName;
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
};

Sequencer::Sequencer (MonosynthPluginAudioProcessor& p, SequencerState& s) : processor(p), state(s)

{
    typedef StepSlider::style knobStyle;

    globalNoteLengthSlider = std::unique_ptr<StepSlider> (new StepSlider("globalNoteLength", knobStyle(ROTARY)));
    addAndMakeVisible (globalNoteLengthSlider.get());
    globalNoteLengthSlider.get()->setRange(10, 1000, 1);
 
    
    //globalNoteLengthLabel = std::unique_ptr<Label>(new Label("StepDiv Label", TRANS("Div")));
    //addAndMakeVisible(globalNoteLengthLabel.get());
   // globalNoteLengthLabel->setFont(Font("", 12.00f, Font::plain).withExtraKerningFactor(0.108f));
   // globalNoteLengthLabel->setJustificationType(Justification::centredTop);
   // globalNoteLengthLabel->setEditable(false, false, false);
   // globalNoteLengthLabel->setColour(TextEditor::textColourId, Colours::black);
   // globalNoteLengthLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    
    for (int i = 0; i < numSteps; i++)
    {
        pitchSlider[i] = std::unique_ptr<StepSlider> ( new StepSlider ("pitchSlider" + std::to_string(i), knobStyle(LINEARVERTICAL) ) );
        addAndMakeVisible (pitchSlider[i].get());
        pitchSlider[i].get()->setRange(-12, 12, 1);
    }
    
	stepDivision = std::unique_ptr<StepSlider>(new StepSlider("stepDivision", knobStyle(ROTARY)));
	addAndMakeVisible(stepDivision.get());
    stepDivision.get()->setRange(1, 6, 1);

	//stepDivisionLabel = std::unique_ptr<Label>(new Label("StepDiv Label", TRANS("Div")));
	//addAndMakeVisible(stepDivisionLabel.get());
	//stepDivisionLabel->setFont(Font("", 12.00f, Font::plain).withExtraKerningFactor(0.108f));
	//stepDivisionLabel->setJustificationType(Justification::centredTop);
	//stepDivisionLabel->setEditable(false, false, false);
	//stepDivisionLabel->setColour(TextEditor::textColourId, Colours::black);
	//stepDivisionLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    
    setSize (890, SEQUENCER_HEIGHT);
    
    state.addListener(this);

	startTimer(displayTimer, 1000 / 60);
    startTimer(hiFreqTimer, 4);
}

Sequencer::~Sequencer()
{
    state.removeListener(this);
}

void Sequencer::handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    lastNotePlayed = midiNoteNumber;
    currentMidiChannel = midiChannel;
    
    int bpm = 120;
    
    bpm = processor.lastPosInfo.bpm;
    int division = pow(2, stepDivision.get()->getValue());
    int pulseTime = (60000 / bpm) / division; // 4 = every beat
    
    //pulseTime =  100;
    startPulseClock(pulseTime);
}


void Sequencer::handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    stopPulseClock();
    //state.allNotesOff(currentMidiChannel);
}


void Sequencer::processSteps()
{
   if (isTimerRunning(PULSECLOCK_TIMER))
   {
       
   }
   
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
    
    Rectangle<int> strip ( area.removeFromTop(SEQUENCER_HEIGHT).reduced(24, 8) );
    
    {
        Rectangle<int> block (strip.removeFromLeft((vertSliderSize * numSteps) + marginX * 2) );
        for (int i =0; i < numSteps; i++)
        {
            pitchSlider[i]->setBounds(block.removeFromLeft(vertSliderSize));
			pitchSlider[i]->setColour(Slider::textBoxOutlineColourId, Colour(0xff323e44));
        }
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        globalNoteLengthSlider->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        
        //globalNoteLengthLabel->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.25 ) );
    }
    
    {
        Rectangle<int> block (strip.removeFromLeft((rotarySize) + marginX) );
        stepDivision->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.75 ).reduced(6, 0) );
        
        //stepDivisionLabel->setBounds(block.removeFromTop(SEQUENCER_HEIGHT * 0.25)  );
    }
}

void Sequencer::parentSizeChanged()
{
    
}

void Sequencer::timerCallback(int timerID)
{
    
    if (timerID == displayTimer)
    {
        updateStepKnobColour();
        //updateGlobalNoteLengthLabel();
       // updateStepDivisionLabel();
        
    
    }
    
    else if (timerID == hiFreqTimer)
    {
        processSteps();
    }
    
    else if (timerID == PULSECLOCK_TIMER)
    {
        playStep(stepCount);
        
        stepCount++;
        
        if (stepCount >= numSteps)
            stepCount = 0;
        
    }
	
}

void Sequencer::playStep (int currentStep)
{
    int newNote = lastNotePlayed + pitchSlider[currentStep].get()->getValue();
    
    std::cout << newNote << std::endl;
    
    state.noteOn(currentMidiChannel, newNote, 1.0f);
}

void Sequencer::startPulseClock(int timeMillis)
{
    startTimer(PULSECLOCK_TIMER, timeMillis);
}


void Sequencer::stopPulseClock()
{
    stopTimer(PULSECLOCK_TIMER);
}


void Sequencer::startReleaseTimer(int step, int timeMillis)
{
    startTimer(step, timeMillis);
}

void Sequencer::updateGlobalNoteLengthLabel()
{
    int val = processor.globalNoteLengthVal * 100;
    String s = std::to_string(val) + "%";
    globalNoteLengthLabel->setText(s, dontSendNotification);
}

void Sequencer::updateStepDivisionLabel()
{
    int val = processor.sequencerStepDivisionVal;
    String s = "1/" + std::to_string(val) + "th";
    stepDivisionLabel->setText(s, dontSendNotification);
}


void Sequencer::updateStepKnobColour()
{
	if (processor.isSequencerPlaying())
	{
		int currentStep = processor.getCurrentStep() - 1; //I DUNNO WHY!

		for (int step = 0; step < numSteps; step++)
		{
			if (step == currentStep)
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
//==============================================================================



