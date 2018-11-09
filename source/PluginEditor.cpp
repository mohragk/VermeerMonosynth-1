/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TITLE_HEIGHT 48
#define STRIP_WIDTH 72
#define MODULE_MARGIN 24
#define MODULE_HEIGHT 460
#define KEYBOARD_HEIGHT 140


/*

class MonosynthPluginAudioProcessorEditor::ParamToggleButton : public ToggleButton, private Timer
{
public:
    ParamToggleButton(AudioProcessorParameter& p ) : ToggleButton(p.getName(256)), param(p)
    {
        
        updateToggleState();
        startTimerHz(30);
    };
    
    ~ParamToggleButton() {};
    
    void timerCallback() override { }; //updateToggleState(); };
    
    void clicked() override
    {
        int newState = ToggleButton::getToggleState();
            
        if (newState != param.getValue())
            param.setValue(newState);
    };
    
private:
    AudioProcessorParameter& param;
    
    void updateToggleState()
    {
        const int newState = param.getValue();
        
        if( newState != ToggleButton::getToggleState() )
        {
            ToggleButton::setToggleState ( newState, dontSendNotification );
        }
    }
};

*/

//==============================================================================
MonosynthPluginAudioProcessorEditor::MonosynthPluginAudioProcessorEditor (MonosynthPluginAudioProcessor& owner)
        :   AudioProcessorEditor (owner),
            midiKeyboard (owner.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
            timecodeDisplayLabel (String()),
	
            titleLabel(nullptr),
            oversampleSwitchSlider(nullptr),
            hqOversamplingButton(nullptr),
            expandSequencerButton(nullptr)
      
{
    // add all the sliders..
    
    font = "Futura";
    

	typedef ParameterSlider::style knobStyle;
    
    // set resize limits for this plug-in
    int width = (STRIP_WIDTH * 8) + ( MODULE_MARGIN * 10);
    int height= TITLE_HEIGHT + MODULE_HEIGHT + KEYBOARD_HEIGHT;
    
    
    // OSCILLATOR SECTION
    oscillatorSection.reset(new OscillatorSection(owner));
    addAndMakeVisible(oscillatorSection.get());
    
    
    // FILTER SECTION
    filterSection.reset(new FilterSection(owner));
    addAndMakeVisible(filterSection.get());
    
    
    //Envelope Section
    envelopeSection.reset (new EnvelopeSection(owner));
    addAndMakeVisible(envelopeSection.get());
    
    
    //LFO Section
    lfoSection.reset (new LFOSection(owner));
    addAndMakeVisible(lfoSection.get());
    
    
    // MASTER SECTION
    masterSection.reset (new MasterSection(owner));
    addAndMakeVisible(masterSection.get());
    
    
    //SEQUENCER SECTION
    sequencerSection.reset (new Sequencer(owner, *owner.seqState.get()));
    addAndMakeVisible(sequencerSection.get());
    sequencerSection->setVisible(false);
    Rectangle<int> seqBounds (0,0 , width, height);
    seqBounds.translate(0, SEQUENCER_HEIGHT + height);
    sequencerSection->setBounds(seqBounds);
    
    
    //
    // TITLE
    //
    titleLabel.reset ( new Label ("Title", TRANS("Monosynth-1")));
    addAndMakeVisible (titleLabel.get());
    titleLabel->setFont (MonosynthLookAndFeel::getCustomFont().withExtraKerningFactor (0.150f).withHeight(22.0f));
    titleLabel->setJustificationType (Justification::centredBottom);
    titleLabel->setEditable (false, false, false);
    titleLabel->setColour (Label::textColourId, Colours::white);
    titleLabel->setColour (TextEditor::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


	

    
    //Oversample button
    hqOversamplingButton.reset( new ParamToggleButton (*owner.useHQOversamplingParam));
    addAndMakeVisible (hqOversamplingButton.get());
    hqOversamplingButton->addListener (this);
	hqOversamplingButton.get()->setTooltip("High Quality ON/OFF");
    

    
    //Sequencer expansion Button
    expandSequencerButton.reset(new ParamToggleButton(*owner.useSequencerParam));
    addAndMakeVisible (expandSequencerButton.get());
    expandSequencerButton->addListener(this);
	expandSequencerButton.get()->setTooltip("Expand and activate Sequencer");
    
    // Keyboard
    addAndMakeVisible(midiKeyboard);
    midiKeyboard.setAvailableRange(12, 115);
    midiKeyboard.setLowestVisibleKey(36);
    midiKeyboard.setKeyWidth(midiKeyboard.getKeyWidth() * 1.5);
    
    
    
    
    
	// Oscilloscope
	oscilloscope_ptr = &owner.scope;
	addAndMakeVisible(oscilloscope_ptr);
    

	
    
    
    //
    // Drawables for symbols/icons
    //
    
	// SIGH.. swapped the graphics out
    drawable11.reset(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable10.reset(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
    
	drawable13.reset(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable12.reset(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
    
	drawable15.reset(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable14.reset(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
    
	drawable17.reset(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable16.reset(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
 
	drawable19.reset(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable18.reset(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
 
	drawable21.reset(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable20.reset(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
    
   
    
    // ANIMATOR
    animator.reset(new ComponentAnimator);
    

    
    //TOOLTIP WINDOW
    tooltipWindow.reset(new TooltipWindow(this, 700));

    // start a timer which will keep our timecode display updated
    startTimerHz (60);
   
    setResizeLimits (width,
                     height,
                     width,
                     height);

    setSize (owner.lastUIWidth, owner.lastUIHeight);
    


    setLookAndFeel(&customLook);


   
}

MonosynthPluginAudioProcessorEditor::~MonosynthPluginAudioProcessorEditor()
{
    
	setLookAndFeel(nullptr);

}


//==============================================================================
void MonosynthPluginAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colour (0xff0e0e0e));
    
    {
        int x = 8 , y = 49, width = getWidth() - 16, height = 1;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
}

void MonosynthPluginAudioProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    // TITLE
    titleLabel->setBounds (area.removeFromTop(TITLE_HEIGHT).reduced(12,0));
    titleLabel->setJustificationType(Justification::centredLeft);
    //oversampleSwitchSlider->setBounds(getWidth() - 24 - 24, 8, 36, 36); // TODO
    
    Rectangle<int> buttonArea (titleLabel->getBounds());
    hqOversamplingButton->setBounds(buttonArea.removeFromRight(48));
    hqOversamplingButton->setButtonText("");

	// OSCILLOSCOPE
	Rectangle<int> oscilloArea(getLocalBounds());
	oscilloscope_ptr->setBounds(oscilloArea.removeFromTop(TITLE_HEIGHT).reduced( (getWidth() / 2) - 128, 6));
    oscilloscope_ptr->setBackgroundColour(Colour (0xff0e0e0e));
    

    
    //
    // MODULES
    //
    Rectangle<int> parameterArea (area.removeFromTop(MODULE_HEIGHT).reduced(MODULE_MARGIN,0));
    
    // OSCILLATOR SECTION
    oscillatorSection->setBounds(parameterArea.removeFromLeft((STRIP_WIDTH * 4) + MODULE_MARGIN));
    
	
    // FILTER SECTION
    filterSection->setBounds (parameterArea.removeFromLeft(STRIP_WIDTH + MODULE_MARGIN));
	
	
    // ENVELOPES
    envelopeSection->setBounds(parameterArea.removeFromLeft((STRIP_WIDTH * 2) + MODULE_MARGIN));
    
    // LFO SECTION
    lfoSection->setBounds(parameterArea.removeFromLeft(STRIP_WIDTH + MODULE_MARGIN));
    
    // MASTER SECTION
    masterSection->setBounds(parameterArea.removeFromLeft(STRIP_WIDTH + MODULE_MARGIN * 2));
    
    
    Rectangle<int> seqButtonArea (masterSection->getBounds());
    seqButtonArea.translate(masterSection->getWidth() - 24, 0);
    expandSequencerButton->setBounds(seqButtonArea.removeFromBottom(48));
    expandSequencerButton->setButtonText("");
    
    
    
    
    
    // KEYBOARD SECTION
    midiKeyboard.setBounds(area.removeFromBottom(KEYBOARD_HEIGHT).reduced(8));
    midiKeyboard.setAlwaysOnTop(true);
    midiKeyboard.setColour(MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colour (0xffc8e6ff));
    midiKeyboard.setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colour (0xff84a7c4));
    
	
    
    
    getProcessor().lastUIWidth = getWidth();
    getProcessor().lastUIHeight = getHeight();
    
    
}

//==============================================================================
void MonosynthPluginAudioProcessorEditor::timerCallback()
{
    updateTimecodeDisplay (getProcessor().lastPosInfo);

    updateStates();
    
}




void MonosynthPluginAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]
    
    
    
    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}


void MonosynthPluginAudioProcessorEditor::updateStates()
{
    //sequencer
    bool seqState =  *getProcessor().useSequencerParam;
    showSequencer(seqState);
    
    
    
}

void MonosynthPluginAudioProcessorEditor::showSequencer(bool shouldShow)
{
    
   
    if (shouldShow)
    {
        if(sequencerSection.get()->isActivated() == true)
            return;
        
		int sequencerMargin = 10;
        int width = (STRIP_WIDTH * 8) + ( MODULE_MARGIN * 10);
        int height= TITLE_HEIGHT + MODULE_HEIGHT + KEYBOARD_HEIGHT + SEQUENCER_HEIGHT + sequencerMargin;
        setSize (width, height);
        
        
        sequencerSection.get()->setVisible(true);
        Rectangle<int> finalBounds (getLocalBounds().removeFromBottom(SEQUENCER_HEIGHT + KEYBOARD_HEIGHT));
        
        if(sequencerSection.get()->isActivated() == false)
            animator.get()->animateComponent(sequencerSection.get(), finalBounds, 1.0f, 600, false, 0.0f, 0.0f);
    }
    else
    {
        if(sequencerSection.get()->isActivated() == false)
            return;
        
        int width = (STRIP_WIDTH * 8) + ( MODULE_MARGIN * 10);
        int height= TITLE_HEIGHT + MODULE_HEIGHT + KEYBOARD_HEIGHT;
        setSize (width, height);
        
        sequencerSection.get()->setVisible(false);
        Rectangle<int> finalBounds (sequencerSection.get()->getBounds());
        finalBounds.translate(0, SEQUENCER_HEIGHT + KEYBOARD_HEIGHT);
        
        if(sequencerSection.get()->isActivated() == true)
            animator.get()->animateComponent(sequencerSection.get(), finalBounds, 0.0f, 50, false, 0.0f, 0.0f);
        
    }
    
     sequencerSection.get()->makeActive(shouldShow);
}


//==============================================================================
// quick-and-dirty function to format a timecode string
static String timeToTimecodeString (double seconds)
{
    const int millisecs = roundToInt (seconds * 1000.0);
    const int absMillisecs = std::abs (millisecs);

    return String::formatted ("%02d:%02d:%02d.%03d",
                              millisecs / 360000,
                              (absMillisecs / 60000) % 60,
                              (absMillisecs / 1000) % 60,
                              absMillisecs % 1000);
}

// quick-and-dirty function to format a bars/beats string
static String quarterNotePositionToBarsBeatsString (double quarterNotes, int numerator, int denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|000";

    const int quarterNotesPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (quarterNotes, quarterNotesPerBar) / quarterNotesPerBar) * numerator;

    const int bar    = ((int) quarterNotes) / quarterNotesPerBar + 1;
    const int beat   = ((int) beats) + 1;
    const int ticks  = ((int) (fmod (beats, 1.0) * 960.0 + 0.5));

    return String::formatted ("%d|%d|%03d", bar, beat, ticks);
}

// Updates the text in our position label.
void MonosynthPluginAudioProcessorEditor::updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo pos)
{
    MemoryOutputStream displayText;

    displayText //<< "[" << SystemStats::getJUCEVersion() << "]   "
                << String (pos.bpm, 2) << " bpm, "
                << pos.timeSigNumerator << '/' << pos.timeSigDenominator
                << "  -  " << timeToTimecodeString (pos.timeInSeconds)
                << "  -  " << quarterNotePositionToBarsBeatsString (pos.ppqPosition,
                                                                    pos.timeSigNumerator,
                                                                    pos.timeSigDenominator);

    if (pos.isRecording)
        displayText << "  (recording)";
    else if (pos.isPlaying)
        displayText << "  (playing)";

    timecodeDisplayLabel.setText (displayText.toString(), dontSendNotification);
}


//==============================================================================
// Binary resources - be careful not to edit any of these sections!



// JUCER_RESOURCE: attackCurveLinear_symbol_svg, 531, "../Resources/attackCurveLinear_Symbol.svg"
static const unsigned char resource_CustomPluginEditor_attackCurveLinear_symbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,
116,102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,
114,116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,
61,34,49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,
115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,
118,105,101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,
34,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,
105,108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,50,53,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,
77,76,73,68,95,51,50,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,56,46,52,44,49,57,32,53,46,53,44,49,57,32,49,56,44,54,46,54,32,49,56,44,49,57,32,49,54,44,49,57,32,
49,54,44,49,49,46,52,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* MonosynthPluginAudioProcessorEditor::attackCurveLinear_symbol_svg = (const char*)resource_CustomPluginEditor_attackCurveLinear_symbol_svg;
const int MonosynthPluginAudioProcessorEditor::attackCurveLinear_symbol_svgSize = 531;

// JUCER_RESOURCE: attackCurveExponential_symbol_svg, 547, "../Resources/attackCurveExponential_Symbol.svg"
static const unsigned char resource_CustomPluginEditor_attackCurveExponential_symbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,
117,116,102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,
111,114,116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,
110,61,34,49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,
110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,
9,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,
52,59,34,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,
123,102,105,108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,51,48,95,34,62,13,10,9,60,112,97,116,104,32,105,100,61,34,88,77,76,
73,68,95,51,51,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,100,61,34,77,49,56,44,49,56,104,45,50,86,57,46,49,99,45,52,46,54,44,48,46,53,45,56,46,49,44,52,46,51,45,56,46,49,44,56,46,57,104,45,50,
99,48,45,54,46,49,44,53,45,49,49,44,49,49,46,49,45,49,49,104,49,86,49,56,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* MonosynthPluginAudioProcessorEditor::attackCurveExponential_symbol_svg = (const char*)resource_CustomPluginEditor_attackCurveExponential_symbol_svg;
const int MonosynthPluginAudioProcessorEditor::attackCurveExponential_symbol_svgSize = 547;

// JUCER_RESOURCE: decayCurveLinear_symbol_svg, 527, "../Resources/decayCurveLinear_Symbol.svg"
static const unsigned char resource_CustomPluginEditor_decayCurveLinear_symbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,
116,102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,
114,116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,
61,34,49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,
115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,
118,105,101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,
34,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,
105,108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,51,49,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,
77,76,73,68,95,51,52,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,49,53,46,49,44,49,57,32,49,56,44,49,57,32,53,44,54,46,54,32,53,44,49,57,32,55,44,49,57,32,55,44,49,
49,46,52,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* MonosynthPluginAudioProcessorEditor::decayCurveLinear_symbol_svg = (const char*)resource_CustomPluginEditor_decayCurveLinear_symbol_svg;
const int MonosynthPluginAudioProcessorEditor::decayCurveLinear_symbol_svgSize = 527;

// JUCER_RESOURCE: decayCurveExponential_symbol_svg, 537, "../Resources/decayCurveExponential_Symbol.svg"
static const unsigned char resource_CustomPluginEditor_decayCurveExponential_symbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,
117,116,102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,
111,114,116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,
110,61,34,49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,
110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,
9,32,118,105,101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,
52,59,34,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,
123,102,105,108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,51,53,95,34,62,13,10,9,60,112,97,116,104,32,105,100,61,34,88,77,76,
73,68,95,51,55,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,100,61,34,77,49,56,44,49,57,99,45,51,46,57,44,48,45,55,46,56,45,50,46,53,45,49,48,45,53,46,56,86,49,57,72,54,86,55,104,50,99,48,44,53,44,
53,44,49,48,44,49,48,44,49,48,86,49,57,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* MonosynthPluginAudioProcessorEditor::decayCurveExponential_symbol_svg = (const char*)resource_CustomPluginEditor_decayCurveExponential_symbol_svg;
const int MonosynthPluginAudioProcessorEditor::decayCurveExponential_symbol_svgSize = 537;


//[EndFile] You can add extra defines here...
//[/EndFile]










