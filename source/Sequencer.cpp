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

Sequencer::Sequencer (MonosynthPluginAudioProcessor& p, SequencerState& s) : processor(p), state(s), startTime(Time::getMillisecondCounter())

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
    
    
    setSize (890, SEQUENCER_HEIGHT);
    
    processor.sequencerState.addListener(this);
	startTimerHz(60);
    
    
}

Sequencer::~Sequencer()
{
    processor.sequencerState.removeListener(this);
}


void Sequencer::makeActive(bool on)
{
    isActive = on;
}


void Sequencer::handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if (isActive)
    {
        stepCount = 0;
        lastNotePlayed = midiNoteNumber;
        currentMidiChannel = midiChannel;
        
        startPulseClock();
        
        isPlaying = true;
    }
}


void Sequencer::handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if(isActive)
    {
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
}

void Sequencer::parentSizeChanged()
{
    
}


void Sequencer::processSequencer(int bufferSize)
{

    int numSamples = bufferSize;
    
    while (--numSamples >= 0)
    {
        if(isPlaying)
        {
            pulseClock.update();
            
            if(pulseClock.isPulseHigh())
            {
                playStep(stepCount);
            }
        }
       
        //CHECK IF NOTES SHOULD BE RELEASED
        for (int i = 0; i < numSteps; i++)
        {
            
            if (!step[i].isReleased)
            {
                int currentTime = static_cast<int>( std::round(Time::getMillisecondCounterHiRes() ) - startTime);
                int range = 4;
                
                if (step[i].timeStamp + step[i].noteLengthMillis > currentTime - range && step[i].timeStamp + step[i].noteLengthMillis < currentTime + range )
                {
                    int note = step[i].noteNumber;
                    state.noteOff(currentMidiChannel, note, 1.0f);
                    step[i].isReleased = true;
                    step[i].isActive = false;
                    
                    
                    //trigger Listener
                    processor.handleNoteOff(nullptr, currentMidiChannel, note, 1.0f);
                }
            }
        }
    }
}


void Sequencer::timerCallback()
{
    updateStepKnobColour(stepCount);
}


void Sequencer::playStep (int currentStep)
{
    
    int newNote = lastNotePlayed + *processor.stepPitchParam[currentStep];
    int pulseInterval = (1 / pulseClock.getFrequency()) * 1000;
    int releaseTime = std::round(  *processor.stepNoteLengthParam * pulseInterval );
    
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthMillis = releaseTime;
    step[currentStep].timeStamp = static_cast<int> ( std::round( Time::getMillisecondCounterHiRes() ) - startTime);
    step[currentStep].isReleased = false;
    step[currentStep].isActive = true;
    
    
    //send noteOn message
    state.noteOn(currentMidiChannel, newNote, 1.0f);
    
   
    
    //trigger Listener
    processor.handleNoteOn(nullptr, currentMidiChannel, newNote, 1.0f);
    
    stepCount++;
    
    if (stepCount >= numSteps)
        stepCount = 0;
}

void Sequencer::startPulseClock()
{
    
    int division = (int) std::round( (double)*processor.stepDivisionFloatParam );
    double pulseTimeHz = getPulseInHz(processor.lastPosInfo, division);

    
    pulseClock.setSampleRate(processor.getSampleRate());
    pulseClock.setFrequency(pulseTimeHz);
    pulseClock.resetModulo();
}


double Sequencer::getPulseInHz(AudioPlayHead::CurrentPositionInfo posInfo, int division)
{
    /*
    int BPM = 120;

    BPM = posInfo.bpm;
    const int denominator = posInfo.timeSigDenominator;
    const int msPerBeat = 60000 / BPM;
    const int msPerNote = msPerBeat * denominator / division;
    
    return msPerNote;
    */
    
    double beats_per_minute = 120;
                beats_per_minute = posInfo.bpm;
    
    const double seconds_per_beat = 60.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * posInfo.timeSigDenominator / division;
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return 1.0 / seconds_per_note;
    
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
    for (int i = 0; i < numSteps; i++)
    {
        if (step[i].isActive == true)
            pitchSlider[i].get()->setColour(Slider::thumbColourId, lightThumb);
        else
            pitchSlider[i].get()->setColour(Slider::thumbColourId, darkThumb);
    }
    
    

}
//==============================================================================



