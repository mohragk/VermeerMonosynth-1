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
    
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
private:
    String sliderName;
    
    
    
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
    startTimer(hiFreqTimer, 1);
}

Sequencer::~Sequencer()
{
    state.removeListener(this);
}

void Sequencer::handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    lastNotePlayed = midiNoteNumber;
    currentMidiChannel = midiChannel;
    
    isPlaying = true;
    
    int bpm = 120;
    
    bpm = processor.lastPosInfo.bpm;
    int division = pow(2, stepDivision.get()->getValue());
    int pulseTime = (60000 / bpm) / division; // 4 = every beat
    startPulseClock(pulseTime);
    
    
}


void Sequencer::handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    stopPulseClock();
    state.allNotesOff(currentMidiChannel);
    
    isPlaying = false;
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
        
        //updateGlobalNoteLengthLabel();
       // updateStepDivisionLabel();
        
    
    }
    
    else if (timerID == hiFreqTimer)
    {
        int currentTime = Time::getMillisecondCounter();
        
        for (int i = 0; i < numSteps; i++)
        {
            if (step[i].timeStamp + step[i].noteLengthMillis == currentTime)
            {
                int note = step[i].noteNumber;
                state.noteOff(currentMidiChannel, note, 1.0f);
                step[i].isReleased = true;
                
                
                //trigger Listener
                processor.handleSequencerNoteOff(&state, currentMidiChannel, note, 1.0f);
            }
        }
    }
    
    else if (timerID == PULSECLOCK_TIMER)
    {
        playStep(stepCount);
        
        stepCount++;
        
        if (stepCount >= numSteps)
            stepCount = 0;
        
    }
    else if (timerID == releaseTimerStep1)
    {
        int curStep = releaseTimerStep1 - 1;
        stepNoteOff(curStep);
    }
    
	
}

void Sequencer::stepNoteOff(int currentStep)
{
    /*
    if(releaseTimerCounter[currentStep] == 1)
    {
        state.allNotesOff(currentMidiChannel);
        stopTimer(currentStep + 1);
        releaseTimerCounter[currentStep] = 0;
        processor.handleSequencerNoteOff(&state, currentMidiChannel, 60, 1.0f);
    }
    else
        releaseTimerCounter[currentStep]++;
     
     */
}

void Sequencer::playStep (int currentStep)
{
    int newNote = lastNotePlayed + pitchSlider[currentStep].get()->getValue();
    int releaseTime = globalNoteLengthSlider.get()->getValue();
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthMillis = releaseTime;
    step[currentStep].timeStamp = Time::getMillisecondCounter();
    step[currentStep].isReleased = false;
    
    //send noteOn message
    state.noteOn(currentMidiChannel, newNote, 1.0f);
    
    //trigger Listener
    processor.handleSequencerNoteOn(&state, currentMidiChannel, newNote, 1.0f);
    
    //update thumb colour
    updateStepKnobColour(currentStep);
}

void Sequencer::startPulseClock(int timeMillis)
{
    startTimer(PULSECLOCK_TIMER, timeMillis);
}


void Sequencer::stopPulseClock()
{
    stopTimer(PULSECLOCK_TIMER);
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


void Sequencer::updateStepKnobColour(int curStep)
{
    for (int step = 0; step < numSteps; step++)
    {
        if (step == stepCount)
            pitchSlider[step].get()->setColour(Slider::thumbColourId, lightThumb);
        else
            pitchSlider[step].get()->setColour(Slider::thumbColourId, darkThumb);
    }
    
    

}
//==============================================================================



