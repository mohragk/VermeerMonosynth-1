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

//==============================================================================
// This is a handy slider subclass that controls an AudioProcessorParameter
// (may move this class into the library itself at some point in the future..)

class JuceDemoPluginAudioProcessorEditor::ParameterSlider   : public Slider,
                                                              private Timer
{
public:
    ParameterSlider (AudioProcessorParameter& p)
        : Slider (p.getName (256)), param (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (120);
        updateSliderPos();
    }

    void valueChanged() override
    {
        if (isMouseButtonDown())
            param.setValueNotifyingHost ((float) Slider::getValue());
        else
            param.setValue ((float) Slider::getValue());
    }

    void timerCallback() override       { updateSliderPos(); }

    void startedDragging() override     { param.beginChangeGesture();  }
    void stoppedDragging() override     { param.endChangeGesture();   }

    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }

    void updateSliderPos()
    {
        const float newValue = param.getValue();

        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
        {           
            Slider::setValue (newValue);
        }
        
    }

    AudioProcessorParameter& param;
    
    LinearSmoothedValue<float> newValueSmooth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

//==============================================================================
JuceDemoPluginAudioProcessorEditor::JuceDemoPluginAudioProcessorEditor (JuceDemoPluginAudioProcessor& owner)
    : AudioProcessorEditor (owner),
        midiKeyboard (owner.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        timecodeDisplayLabel (String())
       /*
	   ,
	   gainLabel (String(), "Volume"),

        oscSectionLabel(String(), "Oscillators"),
        filterSectionLabel(String(), "Filter"),
        envelopesSectionLabel(String(), "Envelopes"),

        osc1GainLabel(String(), "Gain"),
		osc1FineTuneLabel(String(), "Tune"),
        osc1ModeLabel(String(), "Waveform"),
        osc2GainLabel(String(), "Gain"),
		osc2FineTuneLabel(String(), "Tune"),
        osc2ModeLabel(String(), "Waveform"),
        osc3GainLabel(String(), "Gain"),
        osc3FineTuneLabel(String(), "Tune"),
        osc3ModeLabel(String(), "Waveform"),

        filterLabel (String(), "Cutoff"),
        filterQLabel(String(), "Resonance"),
        filterContourLabel(String(), "Contour Amt"),
        filterDriveLabel(String(), "Drive"),

       
        envelope1Label(String(), "Amplitude"),
        envelope2Label(String(), "Pitch"),
        envelope3Label(String(), "Filter Cutoff"),

        oscOffsetLabel (String(), "Offset"),
        osc2OffsetLabel (String(), "Offset"),
        osc3OffsetLabel (String(), "Offset"),
        pitchModLabel(String(), "Amount")
		*/
{
    // add all the sliders..
    
    //
    // TITLE
    //
    
    addAndMakeVisible (titleLabel = new Label ("Title",
                                               TRANS("Vermeer Monosynth-1")));
    titleLabel->setFont (Font ("Futura", 24.00f, Font::plain).withExtraKerningFactor (0.150f));
    titleLabel->setJustificationType (Justification::centredBottom);
    titleLabel->setEditable (false, false, false);
    titleLabel->setColour (Label::textColourId, Colours::white);
    titleLabel->setColour (TextEditor::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    //
    // MAIN
    //
    addAndMakeVisible (mainLabel = new Label ("Main Label",
                                              TRANS("Main")));
    mainLabel->setFont (Font ("Futura", 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    mainLabel->setJustificationType (Justification::centredTop);
    mainLabel->setEditable (false, false, false);
    mainLabel->setColour (TextEditor::textColourId, Colours::black);
    mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    //
    // main volume
    //
    addAndMakeVisible (volumeSlider = new ParameterSlider (*owner.gainParam));  //
    volumeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    volumeSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (volumeLabel = new Label ("Main Volume Label",
                                                TRANS("Volume")));              //
    volumeLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    volumeLabel->setJustificationType (Justification::centredBottom);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    //
    // OSCILLATORS
    //
    
    addAndMakeVisible (oscillatorsLabel = new Label ("Oscillators",
                                                     TRANS("Oscillators")));        //
    oscillatorsLabel->setFont (Font ("Futura", 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    oscillatorsLabel->setJustificationType (Justification::centredTop);
    oscillatorsLabel->setEditable (false, false, false);
    oscillatorsLabel->setColour (TextEditor::textColourId, Colours::black);
    oscillatorsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    // OSC 1
    addAndMakeVisible (osc1GainSlider = new ParameterSlider (*owner.osc1GainParam)); //
    osc1GainSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1GainSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
	addAndMakeVisible(osc1GainLabel = new Label("OSC1 Gain Label",
		TRANS("Gain")));
    osc1GainLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f)); //
    osc1GainLabel->setJustificationType (Justification::centredBottom);
    osc1GainLabel->setEditable (false, false, false);
    osc1GainLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1GainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (osc1WaveformSlider = new ParameterSlider (*owner.osc1ModeParam)); //no label but graphics
    osc1WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc1WaveformSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (osc1WaveformLabel = new Label ("OSC1 Waveform Label",
                                                      TRANS("Shape")));                 //
    osc1WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc1WaveformLabel->setJustificationType (Justification::centredBottom);
    osc1WaveformLabel->setEditable (false, false, false);
    osc1WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	addAndMakeVisible(osc1TuneSlider = new ParameterSlider(*owner.osc1DetuneAmountParam)); //
	osc1TuneSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	osc1TuneSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	osc1TuneSlider->setDoubleClickReturnValue(true, 0.5);
    
    addAndMakeVisible (osc1TuneLabel = new Label ("OSC1 Tune Label",
                                                  TRANS("Tune")));                      //
    osc1TuneLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.150f));
    osc1TuneLabel->setJustificationType (Justification::centredBottom);
    osc1TuneLabel->setEditable (false, false, false);
    osc1TuneLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1TuneLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (osc1OffsetSlider = new ParameterSlider (*owner.oscOffsetParam)); //
    osc1OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 30, 10);
    osc1OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    
    addAndMakeVisible (osc1OffsetLabel = new Label ("osc1OffsetLabel",
                                                    TRANS("Offset")));          //
    osc1OffsetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc1OffsetLabel->setJustificationType (Justification::centredBottom);
    osc1OffsetLabel->setEditable (false, false, false);
    osc1OffsetLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1OffsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    
    // OSC 2
    addAndMakeVisible (osc2GainSlider = new ParameterSlider (*owner.osc2GainParam)); //
    osc2GainSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2GainSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (osc2GainLabel = new Label ("OSC2 Gain Label",
                                                  TRANS("Gain")));                  //
    osc2GainLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc2GainLabel->setJustificationType (Justification::centredBottom);
    osc2GainLabel->setEditable (false, false, false);
    osc2GainLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2GainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (osc2WaveformSlider = new ParameterSlider (*owner.osc2ModeParam)); //no label
    osc2WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc2WaveformSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (osc2WaveformLabel = new Label ("OSC2 Waveform Label",
                                                      TRANS("Shape")));             //
    osc2WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc2WaveformLabel->setJustificationType (Justification::centredBottom);
    osc2WaveformLabel->setEditable (false, false, false);
    osc2WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	addAndMakeVisible(osc2TuneSlider = new ParameterSlider(*owner.osc2DetuneAmountParam)); //
	osc2TuneSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	osc2TuneSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	osc2TuneSlider->setDoubleClickReturnValue(true, 0.5);
    
	addAndMakeVisible(osc2TuneLabel = new Label("OSC2 Tune Label",
		TRANS("Tune")));
    osc2TuneLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f)); //
    osc2TuneLabel->setJustificationType (Justification::centredBottom);
    osc2TuneLabel->setEditable (false, false, false);
    osc2TuneLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2TuneLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (osc2OffsetSlider = new ParameterSlider (*owner.osc2OffsetParam)); //
    osc2OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 30, 10);
    osc2OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    
    addAndMakeVisible (osc2OffsetLabel = new Label ("OSC2 Offset Label",
                                                     TRANS("Offset")));           //
    osc2OffsetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc2OffsetLabel->setJustificationType (Justification::centredBottom);
    osc2OffsetLabel->setEditable (false, false, false);
    osc2OffsetLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2OffsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    
    // OSC 3
    addAndMakeVisible (osc3GainSlider = new ParameterSlider (*owner.osc3GainParam)); //
    osc3GainSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc3GainSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (osc3GainLabel = new Label ("OSC2 Gain Label",
                                                  TRANS("Gain")));                  //
    osc3GainLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc3GainLabel->setJustificationType (Justification::centredBottom);
    osc3GainLabel->setEditable (false, false, false);
    osc3GainLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3GainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (osc3WaveformSlider = new ParameterSlider (*owner.osc3ModeParam));    //
    osc3WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc3WaveformSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (osc3WaveformLabel = new Label ("OSC1 Gain Label",
                                                      TRANS("Shape")));                 //
    osc3WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc3WaveformLabel->setJustificationType (Justification::centredBottom);
    osc3WaveformLabel->setEditable (false, false, false);
    osc3WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible(osc3TuneSlider = new ParameterSlider(*owner.osc3DetuneAmountParam)); //
    osc3TuneSlider->setSliderStyle(Slider::RotaryVerticalDrag);
    osc3TuneSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    osc3TuneSlider->setDoubleClickReturnValue(true, 0.5);
    
    addAndMakeVisible (osc3TuneLabel = new Label ("OSC3 Tune Label",
                                                  TRANS("Tune")));              //
    osc3TuneLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc3TuneLabel->setJustificationType (Justification::centredBottom);
    osc3TuneLabel->setEditable (false, false, false);
    osc3TuneLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3TuneLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc3OffsetSlider = new ParameterSlider (*owner.osc3OffsetParam)); //
    osc3OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc3OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 30, 10);
    osc3OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    
    addAndMakeVisible (osc3OffsetLabel = new Label ("OSC2 Offset Label",
                                                    TRANS("Offset\n")));   //
    osc3OffsetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    osc3OffsetLabel->setJustificationType (Justification::centredBottom);
    osc3OffsetLabel->setEditable (false, false, false);
    osc3OffsetLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3OffsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    
    //Pitch MODULATION SLIDER
    addAndMakeVisible (pitchModSlider = new ParameterSlider (*owner.pitchModParam));        //
    pitchModSlider->setSliderStyle(Slider::RotaryVerticalDrag);
    pitchModSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    pitchModSlider->setDoubleClickReturnValue(true, 0.0);
    
    
    addAndMakeVisible (pitchModLabel = new Label ("OSC2 Gain Label",
                                                  TRANS("Pitch Mod")));                     //
    pitchModLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    pitchModLabel->setJustificationType (Justification::centredBottom);
    pitchModLabel->setEditable (false, false, false);
    pitchModLabel->setColour (TextEditor::textColourId, Colours::black);
    pitchModLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    //
    // FILTER
    //
    
    addAndMakeVisible (filterLabel = new Label ("Filter",
                                                TRANS("Filter")));                          //
    filterLabel->setFont (Font ("Futura", 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    filterLabel->setJustificationType (Justification::centredTop);
    filterLabel->setEditable (false, false, false);
    filterLabel->setColour (TextEditor::textColourId, Colours::black);
    filterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (filterCutoffSlider = new ParameterSlider (*owner.filterParam));   //
	filterCutoffSlider->setSliderStyle (Slider::RotaryVerticalDrag);
	filterCutoffSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (filterCutoffLabel = new Label ("Filter Cutoff Label",
                                                      TRANS("Cutoff")));            //
    filterCutoffLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterCutoffLabel->setJustificationType (Justification::centredBottom);
    filterCutoffLabel->setEditable (false, false, false);
    filterCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    filterCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (filterResonanceSlider = new ParameterSlider (*owner.filterQParam));   //
    filterResonanceSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterResonanceSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (filterResonanceLabel = new Label ("Filter Resonance Label",
                                                         TRANS("Resonance")));      //
    filterResonanceLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterResonanceLabel->setJustificationType (Justification::centredBottom);
    filterResonanceLabel->setEditable (false, false, false);
    filterResonanceLabel->setColour (TextEditor::textColourId, Colours::black);
    filterResonanceLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (filterContourSlider = new ParameterSlider (*owner.filterContourParam));  //
    filterContourSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterContourSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    
    addAndMakeVisible (filterContourLabel = new Label ("Filter Contour Label",
                                                       TRANS("Contour")));              //
    filterContourLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterContourLabel->setJustificationType (Justification::centredBottom);
    filterContourLabel->setEditable (false, false, false);
    filterContourLabel->setColour (TextEditor::textColourId, Colours::black);
    filterContourLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (filterDriveSlider = new ParameterSlider (*owner.filterDriveParam));  //
    filterDriveSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterDriveSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (filterDriveLabel = new Label ("Filter Drive Label",
                                                     TRANS("Drive")));                  //
    filterDriveLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterDriveLabel->setJustificationType (Justification::centredBottom);
    filterDriveLabel->setEditable (false, false, false);
    filterDriveLabel->setColour (TextEditor::textColourId, Colours::black);
    filterDriveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
   
    
    //
    // ENVELOPES
    //
    
    
    addAndMakeVisible (envelopesLabel = new Label ("Envelopes",
                                                   TRANS("Envelopes")));        //
    envelopesLabel->setFont (Font ("Futura", 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    envelopesLabel->setJustificationType (Justification::centredTop);
    envelopesLabel->setEditable (false, false, false);
    envelopesLabel->setColour (TextEditor::textColourId, Colours::black);
    envelopesLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (envAmpLabel = new Label ("Envelope Amp Label",
                                                TRANS("Amplitude")));           //
    envAmpLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envAmpLabel->setJustificationType (Justification::centredBottom);
    envAmpLabel->setEditable (false, false, false);
    envAmpLabel->setColour (TextEditor::textColourId, Colours::black);
    envAmpLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    // Amplitude Envelope
    addAndMakeVisible (attackSlider1 = new ParameterSlider (*owner.attackParam1));      //
    attackSlider1->setSliderStyle(Slider::LinearVertical);
    attackSlider1->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (decaySlider1 = new ParameterSlider (*owner.decayParam1));        //
    decaySlider1->setSliderStyle(Slider::LinearVertical);
    decaySlider1->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(sustainSlider1 = new ParameterSlider (*owner.sustainParam1));     //
    sustainSlider1->setSliderStyle(Slider::LinearVertical);
    sustainSlider1->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(releaseSlider1 = new ParameterSlider (*owner.releaseParam1));     //
    releaseSlider1->setSliderStyle(Slider::LinearVertical);
    releaseSlider1->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
   
    
    
    addAndMakeVisible (envFilterLabel = new Label ("Envelope Filter Label",
                                                   TRANS("Filter Cutoff")));            //
    envFilterLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envFilterLabel->setJustificationType (Justification::centredBottom);
    envFilterLabel->setEditable (false, false, false);
    envFilterLabel->setColour (TextEditor::textColourId, Colours::black);
    envFilterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    // Filter Cutoff Envelope
    addAndMakeVisible (attackSlider2 = new ParameterSlider (*owner.attackParam3));      //
    attackSlider2->setSliderStyle(Slider::LinearVertical);
    attackSlider2->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (decaySlider2 = new ParameterSlider (*owner.decayParam3));        //
    decaySlider2->setSliderStyle(Slider::LinearVertical);
    decaySlider2->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(sustainSlider2 = new ParameterSlider (*owner.sustainParam3));     //
    sustainSlider2->setSliderStyle(Slider::LinearVertical);
    sustainSlider2->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(releaseSlider2 = new ParameterSlider (*owner.releaseParam3));     //
    releaseSlider2->setSliderStyle(Slider::LinearVertical);
    releaseSlider2->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    
    addAndMakeVisible (envPitchLabel = new Label ("Envelope Pitch Label",
                                                  TRANS("Pitch Mod.")));          //
    envPitchLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envPitchLabel->setJustificationType (Justification::centredBottom);
    envPitchLabel->setEditable (false, false, false);
    envPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    envPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    // Pitch Modulation Envelope
    addAndMakeVisible (attackSlider3 = new ParameterSlider (*owner.attackParam2));      //
    attackSlider3->setSliderStyle(Slider::LinearVertical);
    attackSlider3->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (decaySlider3 = new ParameterSlider (*owner.decayParam2));        //
    decaySlider3->setSliderStyle(Slider::LinearVertical);
    decaySlider3->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(sustainSlider3 = new ParameterSlider (*owner.sustainParam2));     //
    sustainSlider3->setSliderStyle(Slider::LinearVertical);
    sustainSlider3->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(releaseSlider3 = new ParameterSlider (*owner.releaseParam2));     //
    releaseSlider3->setSliderStyle(Slider::LinearVertical);
    releaseSlider3->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    
    addAndMakeVisible (attackCurveSlider1 = new ParameterSlider (*owner.attackCurve1Param));     //
    attackCurveSlider1->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider1->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider1->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
  
    
    addAndMakeVisible (decRelCurveSlider1 = new ParameterSlider (*owner.decayRelCurve1Param));      //
    decRelCurveSlider1->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    decRelCurveSlider1->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider1->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    
    
    addAndMakeVisible (attackCurveSlider2 = new ParameterSlider (*owner.attackCurve2Param));    //@TODO: add parameter in Processor
    attackCurveSlider2->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider2->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    
    addAndMakeVisible (decRelCurveSlider2 = new ParameterSlider (*owner.decayRelCurve2Param));  //
    decRelCurveSlider2->setTooltip (TRANS("set decay and release curves from exponential to linear"));//@TODO: add parameter in Processor
    decRelCurveSlider2->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    
    addAndMakeVisible (attackCurveSlider3 = new ParameterSlider (*owner.attackCurve3Param));      //@TODO: add parameter in Processor
    attackCurveSlider3->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider3->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider3->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    
    addAndMakeVisible (decRelCurveSlider3 = new ParameterSlider (*owner.decayRelCurve3Param)); //@TODO: add parameter in Processor
    decRelCurveSlider3->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    decRelCurveSlider3->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider3->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);


	


	//
	// LFO
	//
    addAndMakeVisible (lfoLabel = new Label ("lfoLabel",
                                                TRANS("LFO")));                          //
    lfoLabel->setFont (Font ("Futura", 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    lfoLabel->setJustificationType (Justification::centredTop);
    lfoLabel->setEditable (false, false, false);
    lfoLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (lfoRateLabel = new Label ("lfoRateLabel",
                                                    TRANS("Rate")));          //
    lfoRateLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoRateLabel->setJustificationType (Justification::centredBottom);
    lfoRateLabel->setEditable (false, false, false);
    lfoRateLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoRateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
	addAndMakeVisible(lfoRateSlider = new ParameterSlider(*owner.lfoRateParam));  //
	lfoRateSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	lfoRateSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (lfoModeLabel = new Label ("lfoModeLabel",
                                                 TRANS("Shape")));          //
    lfoModeLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoModeLabel->setJustificationType (Justification::centredBottom);
    lfoModeLabel->setEditable (false, false, false);
    lfoModeLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoModeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	addAndMakeVisible(lfoModeSlider = new ParameterSlider(*owner.lfoModeParam));  //
	lfoModeSlider->setSliderStyle(Slider::LinearHorizontal);
	lfoModeSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

    addAndMakeVisible (lfoIntensityLabel = new Label ("lfoIntensityLabel",
                                                 TRANS("Mod Amt")));          //
    lfoIntensityLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoIntensityLabel->setJustificationType (Justification::centredBottom);
    lfoIntensityLabel->setEditable (false, false, false);
    lfoIntensityLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoIntensityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible(lfoIntensitySlider = new ParameterSlider(*owner.lfoIntensityParam));  //
	lfoIntensitySlider->setSliderStyle(Slider::RotaryVerticalDrag);
	lfoIntensitySlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	
    
    
    //
    // Modulation Target
    //
    addAndMakeVisible(modTargetSlider = new ParameterSlider(*owner.modTargetParam));  //
    modTargetSlider->setSliderStyle(Slider::LinearVertical);
    modTargetSlider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    addAndMakeVisible (modTargetLabel = new Label ("modTargetLabel",
                                                      TRANS("Target")));          //
    modTargetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetLabel->setJustificationType (Justification::centredBottom);
    modTargetLabel->setEditable (false, false, false);
    modTargetLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (modTargetOffLabel = new Label ("modTargetOffLabel",
                                                        TRANS("-off")));          //
    modTargetOffLabel->setFont (Font ("Futura", 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetOffLabel->setJustificationType (Justification::centredLeft);
    modTargetOffLabel->setEditable (false, false, false);
    modTargetOffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetOffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (modTargetPitchLabel = new Label ("modTargetPitchLabel",
                                                      TRANS("-pitch")));          //
    modTargetPitchLabel->setFont (Font ("Futura", 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetPitchLabel->setJustificationType (Justification::centredLeft);
    modTargetPitchLabel->setEditable (false, false, false);
    modTargetPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (modTargetCutoffLabel = new Label ("modTargetCutoffLabel",
                                                        TRANS("-cutoff")));          //
    modTargetCutoffLabel->setFont (Font ("Futura", 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetCutoffLabel->setJustificationType (Justification::centredLeft);
    modTargetCutoffLabel->setEditable (false, false, false);
    modTargetCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
	
	
	
	if (JUCEApplicationBase::isStandaloneApp())
		addAndMakeVisible(midiKeyboard);
                       
                       
    //
    // Drawables for symbols/icons
    //
                       
    drawable1 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable2 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable3 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    drawable4 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable5 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable6 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    drawable7 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable8 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable9 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);

	// SIGH.. swapped the graphics out
    drawable11 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable10 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
    
	drawable13 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable12 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);
    
	drawable15 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable14 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
    
	drawable17 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable16 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);
 
	drawable19 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable18 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
 
	drawable21 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable20 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);
    
    drawable22 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable23 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable24 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    

    // set resize limits for this plug-in
	if (JUCEApplicationBase::isStandaloneApp())
		setResizeLimits (820, 550, 820, 550);
	else
		setResizeLimits(820, 480, 820, 480);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (owner.lastUIWidth,
             owner.lastUIHeight);

    // start a timer which will keep our timecode display updated
    startTimerHz (120);
    
    
}

JuceDemoPluginAudioProcessorEditor::~JuceDemoPluginAudioProcessorEditor()
{
    osc1GainSlider = nullptr;
    osc1GainLabel = nullptr;
    osc1OffsetSlider = nullptr;
    osc1OffsetLabel = nullptr;
    oscillatorsLabel = nullptr;
    osc1TuneSlider = nullptr;
    osc1TuneLabel = nullptr;
    osc1WaveformSlider = nullptr;
    osc2GainSlider = nullptr;
    osc2GainLabel = nullptr;
    osc2OffsetSlider = nullptr;
    osc2OffsetLabel = nullptr;
    osc2TuneSlider = nullptr;
    osc2TuneLabel = nullptr;
    osc2WaveformSlider = nullptr;
    osc3GainSlider = nullptr;
    osc3GainLabel = nullptr;
    osc3OffsetSlider = nullptr;
    osc3OffsetLabel = nullptr;
    osc3TuneSlider = nullptr;
    osc3TuneLabel = nullptr;
    osc3WaveformSlider = nullptr;
    filterLabel = nullptr;
	filterCutoffSlider = nullptr;
    filterCutoffLabel = nullptr;
    filterResonanceSlider = nullptr;
    filterResonanceLabel = nullptr;
    filterContourSlider = nullptr;
    filterContourLabel = nullptr;
    filterDriveSlider = nullptr;
    filterDriveLabel = nullptr;
    envelopesLabel = nullptr;
    attackSlider1 = nullptr;
    attackSlider2 = nullptr;
    envAmpLabel = nullptr;
    decaySlider1 = nullptr;
    sustainSlider1 = nullptr;
    releaseSlider1 = nullptr;
    envFilterLabel = nullptr;
    decaySlider2 = nullptr;
    sustainSlider2 = nullptr;
    releaseSlider2 = nullptr;
    attackSlider3 = nullptr;
    envPitchLabel = nullptr;
    decaySlider3 = nullptr;
    sustainSlider3 = nullptr;
    releaseSlider3 = nullptr;
    volumeSlider = nullptr;
    volumeLabel = nullptr;
    mainLabel = nullptr;
    titleLabel = nullptr;
    attackCurveSlider1 = nullptr;
    decRelCurveSlider1 = nullptr;
    pitchModSlider = nullptr;
    pitchModLabel = nullptr;
    osc1WaveformLabel = nullptr;
    osc2WaveformLabel = nullptr;
    osc3WaveformLabel = nullptr;
    attackCurveSlider2 = nullptr;
    decRelCurveSlider2 = nullptr;
    attackCurveSlider3 = nullptr;
    decRelCurveSlider3 = nullptr;

    lfoLabel = nullptr;
    lfoRateLabel = nullptr;
    lfoModeLabel = nullptr;
    lfoIntensityLabel = nullptr;
    modTargetLabel = nullptr;
    modTargetCutoffLabel = nullptr;
    modTargetPitchLabel = nullptr;
	
    modTargetSlider = nullptr;
	lfoRateSlider = nullptr;
	lfoModeSlider = nullptr;
    lfoIntensitySlider = nullptr;

    drawable1 = nullptr;
    drawable2 = nullptr;
    drawable3 = nullptr;
    drawable4 = nullptr;
    drawable5 = nullptr;
    drawable6 = nullptr;
    drawable7 = nullptr;
    drawable8 = nullptr;
    drawable9 = nullptr;
    drawable10 = nullptr;
    drawable11 = nullptr;
    drawable12 = nullptr;
    drawable13 = nullptr;
    drawable14 = nullptr;
    drawable15 = nullptr;
    drawable16 = nullptr;
    drawable17 = nullptr;
    drawable18 = nullptr;
    drawable19 = nullptr;
    drawable20 = nullptr;
    drawable21 = nullptr;
	
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour (0xff0e0e0e));
    
    {
        int x = 305, y = 64, width = 1, height = 358;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = 401, y = 64, width = 1, height = 358;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = 617, y = 64, width = 1, height = 358;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    
    {
        int x = 713, y = 64, width = 1, height = 358;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = proportionOfWidth (0.5000f) - (proportionOfWidth (0.9500f) / 2), y = 49, width = proportionOfWidth (0.9500f), height = 1;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = 66, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != 0);
        if (drawable1 != 0)
            drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 44, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable2 != 0);
        if (drawable2 != 0)
            drawable2->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 20, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable3 != 0);
        if (drawable3 != 0)
            drawable3->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 138, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable4 != 0);
        if (drawable4 != 0)
            drawable4->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 116, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable5 != 0);
        if (drawable5 != 0)
            drawable5->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 92, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable6 != 0);
        if (drawable6 != 0)
            drawable6->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 210, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable7 != 0);
        if (drawable7 != 0)
            drawable7->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 188, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable8 != 0);
        if (drawable8 != 0)
            drawable8->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 164, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable9 != 0);
        if (drawable9 != 0)
            drawable9->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 168, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable10 != 0);
        if (drawable10 != 0)
            drawable10->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 128, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable11 != 0);
        if (drawable11 != 0)
            drawable11->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 168, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable12 != 0);
        if (drawable12 != 0)
            drawable12->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 128, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable13 != 0);
        if (drawable13 != 0)
            drawable13->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 278, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable14 != 0);
        if (drawable14 != 0)
            drawable14->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 238, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable15 != 0);
        if (drawable15 != 0)
            drawable15->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 278, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable16 != 0);
        if (drawable16 != 0)
            drawable16->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 238, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable17 != 0);
        if (drawable17 != 0)
            drawable17->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 390, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable18 != 0);
        if (drawable18 != 0)
            drawable18->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 538, y = 350, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable19 != 0);
        if (drawable19 != 0)
            drawable19->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 390, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable20 != 0);
        if (drawable20 != 0)
            drawable20->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    {
        int x = 582, y = 350, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable21 != 0);
        if (drawable21 != 0)
            drawable21->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    
    
    
    
    {
        int x = 682, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable22 != 0);
        if (drawable22 != 0)
            drawable22->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    {
        int x = 660, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable23 != 0);
        if (drawable23 != 0)
            drawable23->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
    {
        int x = 638, y = 224, width = 10, height = 6;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable24 != 0);
        if (drawable24 != 0)
            drawable24->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }
	
}

void JuceDemoPluginAudioProcessorEditor::resized()
{
    
    
    osc1GainSlider->setBounds (16, 104, 64, 64);
    osc1GainLabel->setBounds (16, 88, 65, 24);
    osc1OffsetSlider->setBounds (16, 344, 64, 74);
    osc1OffsetLabel->setBounds (16, 328, 65, 24);
    oscillatorsLabel->setBounds (88, 64, 150, 24);
    osc1TuneSlider->setBounds (16, 263, 64, 64);
    osc1TuneLabel->setBounds (16, 247, 65, 24);
    osc1WaveformSlider->setBounds (16, 178, 64, 64);
    osc2GainSlider->setBounds (88, 104, 64, 64);
    osc2GainLabel->setBounds (88, 88, 65, 24);
    osc2OffsetSlider->setBounds (88, 344, 64, 74);
    osc2OffsetLabel->setBounds (88, 328, 65, 24);
    osc2TuneSlider->setBounds (88, 263, 64, 64);
    osc2TuneLabel->setBounds (88, 247, 65, 24);
    osc2WaveformSlider->setBounds (88, 178, 64, 64);
    osc3GainSlider->setBounds (160, 104, 64, 64);
    osc3GainLabel->setBounds (160, 88, 65, 24);
    osc3OffsetSlider->setBounds (160, 344, 64, 74);
    osc3OffsetLabel->setBounds (160, 328, 65, 24);
    osc3TuneSlider->setBounds (160, 263, 64, 64);
    osc3TuneLabel->setBounds (160, 247, 65, 24);
    osc3WaveformSlider->setBounds (160, 178, 64, 64);
    filterLabel->setBounds (320, 64, 65, 24);
    filterCutoffSlider->setBounds (320, 104, 64, 64);
    filterCutoffLabel->setBounds (320, 88, 65, 24);
    filterResonanceSlider->setBounds (320, 184, 64, 64);
    filterResonanceLabel->setBounds (304, 168, 100, 24);
    filterContourSlider->setBounds (320, 264, 64, 64);
    filterContourLabel->setBounds (320, 248, 65, 24);
    filterDriveSlider->setBounds (320, 344, 64, 64);
    filterDriveLabel->setBounds (320, 328, 65, 24);
    envelopesLabel->setBounds (464, 64, 100, 24);
    attackSlider1->setBounds (424, 112, 14, 80);
    attackSlider2->setBounds (424, 224, 14, 80);
    envAmpLabel->setBounds (416, 88, 100, 24);
    decaySlider1->setBounds (448, 112, 14, 80);
    sustainSlider1->setBounds (472, 112, 14, 80);
    releaseSlider1->setBounds (496, 112, 14, 80);
    envFilterLabel->setBounds (416, 200, 100, 24);
    decaySlider2->setBounds (448, 224, 14, 80);
    sustainSlider2->setBounds (472, 224, 14, 80);
    releaseSlider2->setBounds (496, 224, 14, 80);
    attackSlider3->setBounds (424, 336, 14, 80);
    envPitchLabel->setBounds (416, 312, 100, 24);
    decaySlider3->setBounds (448, 336, 14, 80);
    sustainSlider3->setBounds (472, 336, 14, 80);
    releaseSlider3->setBounds (496, 336, 14, 80);
    volumeSlider->setBounds (getWidth() - 24 - 64, 104, 64, 65);
    volumeLabel->setBounds (getWidth() - 23 - 65, 88, 65, 24);

    lfoLabel->setBounds             (628, 64, 80, 24);
    
    lfoRateLabel->setBounds         (634, 88, 64, 24);
	lfoRateSlider->setBounds        (634, 104, 64, 64);
    
    lfoModeLabel->setBounds         (634, 168, 64, 24);
	lfoModeSlider->setBounds        (634, 178, 64, 64);
    
    lfoIntensityLabel->setBounds    (634, 247, 64, 24);
	lfoIntensitySlider->setBounds   (634, 263, 64, 64);
    
    modTargetLabel->setBounds       (634, 328, 64, 24);
    modTargetSlider->setBounds      (612, 352, 64, 54);
    
    modTargetOffLabel->setBounds  (650, 352, 64, 24);
    modTargetCutoffLabel->setBounds (650, 368, 64, 24);
    modTargetPitchLabel->setBounds  (650, 384, 64, 24);
    

    mainLabel->setBounds (getWidth() - 24 - 64, 64, 64, 24);
    titleLabel->setBounds ((getWidth() / 2) - (400 / 2), 12, 400, 24);
    attackCurveSlider1->setBounds (554, 122, 12, 60);
    decRelCurveSlider1->setBounds (568, 122, 12, 60);
    pitchModSlider->setBounds (232, 104, 64, 64);
    pitchModLabel->setBounds (224, 88, 80, 24);
    osc1WaveformLabel->setBounds (16, 168, 65, 24);
    osc2WaveformLabel->setBounds (88, 168, 64, 24);
    osc3WaveformLabel->setBounds (160, 168, 64, 24);
    attackCurveSlider2->setBounds (554, 232, 12, 60);
    decRelCurveSlider2->setBounds (568, 232, 12, 60);
    attackCurveSlider3->setBounds (554, 346, 12, 60);
    decRelCurveSlider3->setBounds (568, 346, 12, 60);
   
	if (JUCEApplicationBase::isStandaloneApp())
	{
		Rectangle<int> r(getLocalBounds().reduced(8));
		midiKeyboard.setBounds(r.removeFromBottom(70));
	}

    getProcessor().lastUIWidth = getWidth();
    getProcessor().lastUIHeight = getHeight();
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::timerCallback()
{
    updateTimecodeDisplay (getProcessor().lastPosInfo);
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
void JuceDemoPluginAudioProcessorEditor::updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo pos)
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

// JUCER_RESOURCE: oscSquareWaveSymbol_svg, 599, "../../../../../../Pictures/OscSquareWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSquareWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,
102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,
116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,
49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,
120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,
120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,
108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,50,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
68,95,49,56,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,46,52,44,49,55,46,50,32,49,52,46,55,44,49,55,46,50,32,49,52,46,55,44,57,46,49,32,56,46,55,44,57,46,49,
32,56,46,55,44,49,55,46,50,32,51,44,49,55,46,50,32,51,44,49,53,46,50,32,54,46,55,44,49,53,46,50,32,54,46,55,44,55,46,49,32,13,10,9,9,49,54,46,55,44,55,46,49,32,49,54,46,55,44,49,53,46,50,32,50,48,46,52,
44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* JuceDemoPluginAudioProcessorEditor::oscSquareWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg;
const int JuceDemoPluginAudioProcessorEditor::oscSquareWaveSymbol_svgSize = 599;

// JUCER_RESOURCE: oscSquareWaveSymbol_svg2, 599, "../Resources/OscSquareWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSquareWaveSymbol_svg2[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,
102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,
116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,
49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,
120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,
101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,
120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,
108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,50,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
68,95,49,56,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,46,52,44,49,55,46,50,32,49,52,46,55,44,49,55,46,50,32,49,52,46,55,44,57,46,49,32,56,46,55,44,57,46,49,
32,56,46,55,44,49,55,46,50,32,51,44,49,55,46,50,32,51,44,49,53,46,50,32,54,46,55,44,49,53,46,50,32,54,46,55,44,55,46,49,32,13,10,9,9,49,54,46,55,44,55,46,49,32,49,54,46,55,44,49,53,46,50,32,50,48,46,52,
44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* JuceDemoPluginAudioProcessorEditor::oscSquareWaveSymbol_svg2 = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg2;
const int JuceDemoPluginAudioProcessorEditor::oscSquareWaveSymbol_svg2Size = 599;

// JUCER_RESOURCE: oscSawWaveSymbol_svg, 559, "../Resources/OscSawWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSawWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,102,
45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,116,
32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,49,
46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,
108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,120,
109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,108,
108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,49,54,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
68,95,49,57,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,44,49,56,32,49,53,46,50,44,49,56,32,57,46,56,44,49,49,46,51,32,57,46,56,44,49,56,32,52,44,49,56,32,52,
44,49,54,32,55,46,56,44,49,54,32,55,46,56,44,53,46,55,32,49,54,46,49,44,49,54,32,50,48,44,49,54,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* JuceDemoPluginAudioProcessorEditor::oscSawWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSawWaveSymbol_svg;
const int JuceDemoPluginAudioProcessorEditor::oscSawWaveSymbol_svgSize = 559;

// JUCER_RESOURCE: oscSineWaveSymbol_svg, 711, "../Resources/OscSineWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSineWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,102,
45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,116,
32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,49,
46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,
108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,101,
119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,120,
109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,108,
108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,49,53,95,34,62,13,10,9,60,112,97,116,104,32,105,100,61,34,88,77,76,73,68,95,50,48,
95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,100,61,34,77,49,53,44,49,55,99,45,49,46,50,44,48,45,51,45,49,45,52,46,52,45,52,46,55,67,49,48,44,49,48,46,54,44,57,44,57,46,53,44,56,46,49,44,57,46,53,
99,45,49,44,48,45,50,46,52,44,49,46,50,45,51,46,51,44,51,46,56,108,45,49,46,57,45,48,46,54,13,10,9,9,99,49,45,51,46,50,44,51,45,53,46,50,44,53,46,49,45,53,46,51,99,49,46,49,44,48,44,51,46,49,44,48,46,
54,44,52,46,52,44,52,46,49,67,49,51,46,52,44,49,52,44,49,52,46,52,44,49,53,44,49,53,44,49,53,99,48,44,48,44,48,44,48,44,48,44,48,99,48,46,55,44,48,44,50,45,49,46,51,44,51,45,52,46,52,108,49,46,57,44,48,
46,55,67,49,56,46,56,44,49,52,46,57,44,49,55,44,49,55,44,49,53,44,49,55,13,10,9,9,67,49,53,46,49,44,49,55,44,49,53,44,49,55,44,49,53,44,49,55,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,
13,10,0,0 };

const char* JuceDemoPluginAudioProcessorEditor::oscSineWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSineWaveSymbol_svg;
const int JuceDemoPluginAudioProcessorEditor::oscSineWaveSymbol_svgSize = 711;

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

const char* JuceDemoPluginAudioProcessorEditor::attackCurveLinear_symbol_svg = (const char*)resource_CustomPluginEditor_attackCurveLinear_symbol_svg;
const int JuceDemoPluginAudioProcessorEditor::attackCurveLinear_symbol_svgSize = 531;

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

const char* JuceDemoPluginAudioProcessorEditor::attackCurveExponential_symbol_svg = (const char*)resource_CustomPluginEditor_attackCurveExponential_symbol_svg;
const int JuceDemoPluginAudioProcessorEditor::attackCurveExponential_symbol_svgSize = 547;

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

const char* JuceDemoPluginAudioProcessorEditor::decayCurveLinear_symbol_svg = (const char*)resource_CustomPluginEditor_decayCurveLinear_symbol_svg;
const int JuceDemoPluginAudioProcessorEditor::decayCurveLinear_symbol_svgSize = 527;

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

const char* JuceDemoPluginAudioProcessorEditor::decayCurveExponential_symbol_svg = (const char*)resource_CustomPluginEditor_decayCurveExponential_symbol_svg;
const int JuceDemoPluginAudioProcessorEditor::decayCurveExponential_symbol_svgSize = 537;


//[EndFile] You can add extra defines here...
//[/EndFile]











