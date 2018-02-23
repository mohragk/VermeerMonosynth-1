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
    globalNoteLengthSlider.get()->setRange(10, 100, 1);
 
    
    for (int i = 0; i < numSteps; i++)
    {
        pitchSlider[i] = std::unique_ptr<StepSlider> ( new StepSlider ("pitchSlider" + std::to_string(i), knobStyle(LINEARVERTICAL) ) );
        addAndMakeVisible (pitchSlider[i].get());
        pitchSlider[i].get()->setRange(-12, 12, 1);
    }
    
	stepDivision = std::unique_ptr<StepSlider>(new StepSlider("stepDivision", knobStyle(ROTARY)));
	addAndMakeVisible(stepDivision.get());
    stepDivision.get()->setRange(1, 6, 1);

    
    setSize (890, SEQUENCER_HEIGHT);
    
    processor.keyboardState.addListener(this);
	startTimer(displayTimer, 1000 / 60);
    startTimer(hiFreqTimer, 1);
}

Sequencer::~Sequencer()
{
    processor.keyboardState.removeListener(this);
}


void Sequencer::makeActive(bool on)
{
    isActive = on;
}


void Sequencer::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if (isActive)
    {
        lastNotePlayed = midiNoteNumber;
        currentMidiChannel = midiChannel;
        
        stepCount = 0;
        
        startPulseClock();
        
        playStep(stepCount);
        
        isPlaying = true;
    }
    
    
    
}


void Sequencer::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if(isActive)
    {
        stopPulseClock();
       
        isPlaying = false;
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
        
    }
}

void Sequencer::parentSizeChanged()
{
    
}

void Sequencer::timerCallback(int timerID)
{

    if (timerID == hiFreqTimer)
    {
        int currentTime = static_cast<int>( std::round(Time::getMillisecondCounterHiRes() ) );
        
        //std::cout << currentTime << std::endl;
        
        for (int i = 0; i < numSteps; i++)
        {
            int range = 4;
            
            if (!step[i].isReleased)
            {
                if (step[i].timeStamp + step[i].noteLengthMillis > currentTime - range && step[i].timeStamp + step[i].noteLengthMillis < currentTime + range )
                {
                    int note = step[i].noteNumber;
                    state.noteOff(currentMidiChannel, note, 1.0f);
                    step[i].isReleased = true;
                    
                    
                    //trigger Listener
                    processor.handleNoteOff(nullptr, currentMidiChannel, note, 1.0f);
                }
                
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
    else if (timerID == displayTimer)
    {
        updateStepKnobColour(stepCount);
    }
}


void Sequencer::playStep (int currentStep)
{
    //ScopedLock s1 (lock);
    
    int newNote = lastNotePlayed + pitchSlider[currentStep].get()->getValue();
    int pulseInterval = getTimerInterval(PULSECLOCK_TIMER);
    int releaseTime = std::round( ( globalNoteLengthSlider.get()->getValue() / 100 ) * pulseInterval );
    
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthMillis = releaseTime;
    step[currentStep].timeStamp = static_cast<int> ( std::round( Time::getMillisecondCounterHiRes() ) );
    step[currentStep].isReleased = false;
    
    
    //send noteOn message
    state.noteOn(currentMidiChannel, newNote, 1.0f);
    
    //trigger Listener
    processor.handleNoteOn(nullptr, currentMidiChannel, newNote, 1.0f);
}

void Sequencer::startPulseClock()
{
    int bpm = 120;
    
    bpm = processor.lastPosInfo.bpm;
    
    
    
    int division = pow(2, stepDivision.get()->getValue());
    int pulseTime = getPulseInterval(processor.lastPosInfo, division);
    
    std::cout << "div: " << division << " || ms: " << pulseTime << std::endl;
    
    startTimer(PULSECLOCK_TIMER, pulseTime);
}


void Sequencer::stopPulseClock()
{
    stopTimer(PULSECLOCK_TIMER);
}


int Sequencer::getPulseInterval(AudioPlayHead::CurrentPositionInfo posInfo, int division)
{
    int BPM = 120;
    
   
    BPM = posInfo.bpm;
    const int denominator = posInfo.timeSigDenominator;
    const int msPerBeat = 60000 / BPM;
    const int msPerNote = msPerBeat * denominator / division;
    
    return msPerNote;
    
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



