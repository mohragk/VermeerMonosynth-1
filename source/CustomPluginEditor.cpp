/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "CustomPluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CustomPluginEditor::CustomPluginEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("GUIEditor");
    addAndMakeVisible (osc1GainSlider = new Slider ("OSC1 Gain"));
    osc1GainSlider->setRange (0, 10, 0);
    osc1GainSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1GainSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc1GainSlider->addListener (this);

    addAndMakeVisible (osc1GainLabel = new Label ("OSC1 Gain Label",
                                                  TRANS("Gain")));
    osc1GainLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc1GainLabel->setJustificationType (Justification::centredBottom);
    osc1GainLabel->setEditable (false, false, false);
    osc1GainLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1GainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1OffsetSlider = new Slider ("OSC1 Offset Slider"));
    osc1OffsetSlider->setRange (0, 10, 0);
    osc1OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 20, 10);
    osc1OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc1OffsetSlider->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Offset\n")));
    label2->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    label2->setJustificationType (Justification::centredBottom);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (oscillatorsLabel = new Label ("Oscillators",
                                                     TRANS("Oscillators")));
    oscillatorsLabel->setFont (Font ("Futura", 20.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.108f));
    oscillatorsLabel->setJustificationType (Justification::centredTop);
    oscillatorsLabel->setEditable (false, false, false);
    oscillatorsLabel->setColour (TextEditor::textColourId, Colours::black);
    oscillatorsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1TuneSlider = new Slider ("OSC1 Tune"));
    osc1TuneSlider->setRange (0, 10, 0);
    osc1TuneSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1TuneSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc1TuneSlider->addListener (this);

    addAndMakeVisible (osc1TuneLabel = new Label ("OSC1 Tune Label",
                                                  TRANS("Tune")));
    osc1TuneLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc1TuneLabel->setJustificationType (Justification::centredBottom);
    osc1TuneLabel->setEditable (false, false, false);
    osc1TuneLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1TuneLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1WaveformSlider = new Slider ("OSC1 Waveform Slider"));
    osc1WaveformSlider->setRange (0, 10, 0);
    osc1WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc1WaveformSlider->setTextBoxStyle (Slider::NoTextBox, true, 20, 20);
    osc1WaveformSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc1WaveformSlider->addListener (this);

    addAndMakeVisible (osc2GainSlider = new Slider ("OSC2 Gain"));
    osc2GainSlider->setRange (0, 10, 0);
    osc2GainSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2GainSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc2GainSlider->addListener (this);

    addAndMakeVisible (osc2GainLabel = new Label ("OSC2 Gain Label",
                                                  TRANS("Gain")));
    osc2GainLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2GainLabel->setJustificationType (Justification::centredBottom);
    osc2GainLabel->setEditable (false, false, false);
    osc2GainLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2GainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc2OffsetSlider = new Slider ("OSC2 Offset Slider"));
    osc2OffsetSlider->setRange (0, 10, 0);
    osc2OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 20, 10);
    osc2OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc2OffsetSlider->addListener (this);

    addAndMakeVisible (osc2OffsetLabel = new Label ("OSC2 Offset Label",
                                                    TRANS("Offset\n")));
    osc2OffsetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2OffsetLabel->setJustificationType (Justification::centredBottom);
    osc2OffsetLabel->setEditable (false, false, false);
    osc2OffsetLabel->setColour (Label::outlineColourId, Colour (0x009e1919));
    osc2OffsetLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2OffsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc2TuneSlider = new Slider ("OSC2 Tune"));
    osc2TuneSlider->setRange (0, 10, 0);
    osc2TuneSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2TuneSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc2TuneSlider->addListener (this);

    addAndMakeVisible (osc2TuneLabel = new Label ("OSC2 Tune Label",
                                                  TRANS("Tune")));
    osc2TuneLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2TuneLabel->setJustificationType (Justification::centredBottom);
    osc2TuneLabel->setEditable (false, false, false);
    osc2TuneLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2TuneLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc2WaveformSlider = new Slider ("OSC2 Waveform Slider"));
    osc2WaveformSlider->setRange (0, 10, 0);
    osc2WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc2WaveformSlider->setTextBoxStyle (Slider::NoTextBox, true, 20, 20);
    osc2WaveformSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc2WaveformSlider->addListener (this);

    addAndMakeVisible (osc2GainSlider2 = new Slider ("OSC2 Gain"));
    osc2GainSlider2->setRange (0, 10, 0);
    osc2GainSlider2->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2GainSlider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc2GainSlider2->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    osc2GainSlider2->addListener (this);

    addAndMakeVisible (osc2GainLabel2 = new Label ("OSC2 Gain Label",
                                                   TRANS("Gain")));
    osc2GainLabel2->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2GainLabel2->setJustificationType (Justification::centredBottom);
    osc2GainLabel2->setEditable (false, false, false);
    osc2GainLabel2->setColour (TextEditor::textColourId, Colours::black);
    osc2GainLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc3OffsetSlider = new Slider ("OSC2 Offset Slider"));
    osc3OffsetSlider->setRange (0, 10, 0);
    osc3OffsetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc3OffsetSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 20, 10);
    osc3OffsetSlider->setColour (Slider::thumbColourId, Colour (0xff42a2c8));
    osc3OffsetSlider->setColour (Slider::textBoxTextColourId, Colours::white);
    osc3OffsetSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc3OffsetSlider->addListener (this);

    addAndMakeVisible (osc3OffsetLabel = new Label ("OSC2 Offset Label",
                                                    TRANS("Offset\n")));
    osc3OffsetLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc3OffsetLabel->setJustificationType (Justification::centredBottom);
    osc3OffsetLabel->setEditable (false, false, false);
    osc3OffsetLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3OffsetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc3TuneSlider = new Slider ("OSC2 Tune"));
    osc3TuneSlider->setRange (0, 10, 0);
    osc3TuneSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    osc3TuneSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc3TuneSlider->addListener (this);

    addAndMakeVisible (osc2TuneLabel2 = new Label ("OSC2 Tune Label",
                                                   TRANS("Tune")));
    osc2TuneLabel2->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2TuneLabel2->setJustificationType (Justification::centredBottom);
    osc2TuneLabel2->setEditable (false, false, false);
    osc2TuneLabel2->setColour (TextEditor::textColourId, Colours::black);
    osc2TuneLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc3WaveformSlider = new Slider ("OSC2 Waveform Slider"));
    osc3WaveformSlider->setRange (0, 10, 0);
    osc3WaveformSlider->setSliderStyle (Slider::LinearHorizontal);
    osc3WaveformSlider->setTextBoxStyle (Slider::NoTextBox, true, 20, 20);
    osc3WaveformSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    osc3WaveformSlider->addListener (this);

    addAndMakeVisible (filterLabel = new Label ("Filter",
                                                TRANS("Filter")));
    filterLabel->setFont (Font ("Futura", 20.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.108f));
    filterLabel->setJustificationType (Justification::centredTop);
    filterLabel->setEditable (false, false, false);
    filterLabel->setColour (TextEditor::textColourId, Colours::black);
    filterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filterCutoffSider = new Slider ("Filter Cutoff Slider"));
    filterCutoffSider->setRange (0, 10, 0);
    filterCutoffSider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterCutoffSider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filterCutoffSider->addListener (this);

    addAndMakeVisible (filterCutoffLabel = new Label ("Filter Cutoff Label",
                                                      TRANS("Cutoff")));
    filterCutoffLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    filterCutoffLabel->setJustificationType (Justification::centredBottom);
    filterCutoffLabel->setEditable (false, false, false);
    filterCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    filterCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filterResonanceSlider = new Slider ("Filter Resonance Slider"));
    filterResonanceSlider->setRange (0, 10, 0);
    filterResonanceSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterResonanceSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filterResonanceSlider->addListener (this);

    addAndMakeVisible (filterResonanceLabel = new Label ("Filter Resonance Label",
                                                         TRANS("Resonance")));
    filterResonanceLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    filterResonanceLabel->setJustificationType (Justification::centredBottom);
    filterResonanceLabel->setEditable (false, false, false);
    filterResonanceLabel->setColour (TextEditor::textColourId, Colours::black);
    filterResonanceLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filterContourSlider = new Slider ("Filter Contour Slider"));
    filterContourSlider->setRange (0, 10, 0);
    filterContourSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterContourSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filterContourSlider->addListener (this);

    addAndMakeVisible (filterContourLabel = new Label ("Filter Contour Label",
                                                       TRANS("Contour")));
    filterContourLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    filterContourLabel->setJustificationType (Justification::centredBottom);
    filterContourLabel->setEditable (false, false, false);
    filterContourLabel->setColour (TextEditor::textColourId, Colours::black);
    filterContourLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filterDriveSlider = new Slider ("Filter Drive Slider"));
    filterDriveSlider->setRange (0, 10, 0);
    filterDriveSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    filterDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filterDriveSlider->addListener (this);

    addAndMakeVisible (filterDriveLabel = new Label ("Filter Drive Label",
                                                     TRANS("Drive")));
    filterDriveLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    filterDriveLabel->setJustificationType (Justification::centredBottom);
    filterDriveLabel->setEditable (false, false, false);
    filterDriveLabel->setColour (TextEditor::textColourId, Colours::black);
    filterDriveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (envelopesLabel = new Label ("Envelopes",
                                                   TRANS("Envelopes")));
    envelopesLabel->setFont (Font ("Futura", 20.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.108f));
    envelopesLabel->setJustificationType (Justification::centredTop);
    envelopesLabel->setEditable (false, false, false);
    envelopesLabel->setColour (TextEditor::textColourId, Colours::black);
    envelopesLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (attackSlider1 = new Slider ("Attack Slider 1"));
    attackSlider1->setRange (0, 10, 0);
    attackSlider1->setSliderStyle (Slider::LinearVertical);
    attackSlider1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    attackSlider1->addListener (this);

    addAndMakeVisible (attackSlider2 = new Slider ("Attack Slider 1"));
    attackSlider2->setRange (0, 10, 0);
    attackSlider2->setSliderStyle (Slider::LinearVertical);
    attackSlider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    attackSlider2->addListener (this);

    addAndMakeVisible (envAmpLabel = new Label ("Envelope Amp Label",
                                                TRANS("Amplitude")));
    envAmpLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    envAmpLabel->setJustificationType (Justification::centredBottom);
    envAmpLabel->setEditable (false, false, false);
    envAmpLabel->setColour (TextEditor::textColourId, Colours::black);
    envAmpLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (decaySlider1 = new Slider ("Decay Slider 1"));
    decaySlider1->setRange (0, 10, 0);
    decaySlider1->setSliderStyle (Slider::LinearVertical);
    decaySlider1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    decaySlider1->addListener (this);

    addAndMakeVisible (sustainSlider1 = new Slider ("Sustain Slider 1"));
    sustainSlider1->setRange (0, 10, 0);
    sustainSlider1->setSliderStyle (Slider::LinearVertical);
    sustainSlider1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sustainSlider1->addListener (this);

    addAndMakeVisible (releaseSlider1 = new Slider ("Release Slider 1"));
    releaseSlider1->setRange (0, 10, 0);
    releaseSlider1->setSliderStyle (Slider::LinearVertical);
    releaseSlider1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    releaseSlider1->addListener (this);

    addAndMakeVisible (envFilterLabel = new Label ("Envelope Filter Label",
                                                   TRANS("Filter Cutoff")));
    envFilterLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    envFilterLabel->setJustificationType (Justification::centredBottom);
    envFilterLabel->setEditable (false, false, false);
    envFilterLabel->setColour (TextEditor::textColourId, Colours::black);
    envFilterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (decaySlider2 = new Slider ("Decay Slider 2"));
    decaySlider2->setRange (0, 10, 0);
    decaySlider2->setSliderStyle (Slider::LinearVertical);
    decaySlider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    decaySlider2->addListener (this);

    addAndMakeVisible (sustainSlider2 = new Slider ("Attack Slider 1"));
    sustainSlider2->setRange (0, 10, 0);
    sustainSlider2->setSliderStyle (Slider::LinearVertical);
    sustainSlider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sustainSlider2->addListener (this);

    addAndMakeVisible (releaseSlider2 = new Slider ("Attack Slider 1"));
    releaseSlider2->setRange (0, 10, 0);
    releaseSlider2->setSliderStyle (Slider::LinearVertical);
    releaseSlider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    releaseSlider2->addListener (this);

    addAndMakeVisible (attackSlider3 = new Slider ("Attack Slider 3"));
    attackSlider3->setRange (0, 10, 0);
    attackSlider3->setSliderStyle (Slider::LinearVertical);
    attackSlider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    attackSlider3->addListener (this);

    addAndMakeVisible (envPitchLabel = new Label ("Envelope Pitch Label",
                                                  TRANS("Pitch Mod.")));
    envPitchLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    envPitchLabel->setJustificationType (Justification::centredBottom);
    envPitchLabel->setEditable (false, false, false);
    envPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    envPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (decaySlider3 = new Slider ("Attack Slider 1"));
    decaySlider3->setRange (0, 10, 0);
    decaySlider3->setSliderStyle (Slider::LinearVertical);
    decaySlider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    decaySlider3->addListener (this);

    addAndMakeVisible (sustainSlider3 = new Slider ("Attack Slider 1"));
    sustainSlider3->setRange (0, 10, 0);
    sustainSlider3->setSliderStyle (Slider::LinearVertical);
    sustainSlider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sustainSlider3->addListener (this);

    addAndMakeVisible (releaseSlider3 = new Slider ("Attack Slider 1"));
    releaseSlider3->setRange (0, 10, 0);
    releaseSlider3->setSliderStyle (Slider::LinearVertical);
    releaseSlider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    releaseSlider3->addListener (this);

    addAndMakeVisible (volumeSlider = new Slider ("Volume Slider"));
    volumeSlider->setRange (0, 10, 0);
    volumeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    volumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    volumeSlider->addListener (this);

    addAndMakeVisible (volumeLabel = new Label ("Main Volume Label",
                                                TRANS("Volume")));
    volumeLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    volumeLabel->setJustificationType (Justification::centredBottom);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (mainLabel = new Label ("Main Label",
                                              TRANS("Main")));
    mainLabel->setFont (Font ("Futura", 20.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.108f));
    mainLabel->setJustificationType (Justification::centredTop);
    mainLabel->setEditable (false, false, false);
    mainLabel->setColour (TextEditor::textColourId, Colours::black);
    mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (titleLabel = new Label ("Title",
                                               TRANS("Vermeer Monosynth-1")));
    titleLabel->setFont (Font ("Futura", 24.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    titleLabel->setJustificationType (Justification::centredBottom);
    titleLabel->setEditable (false, false, false);
    titleLabel->setColour (Label::textColourId, Colours::white);
    titleLabel->setColour (TextEditor::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (attackCurveSlider1 = new Slider ("Attack Curve Slider 1"));
    attackCurveSlider1->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider1->setRange (0, 10, 0);
    attackCurveSlider1->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider1->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    attackCurveSlider1->addListener (this);

    addAndMakeVisible (decRelCurveSlider1 = new Slider ("Decay Release Curve Slider"));
    decRelCurveSlider1->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    decRelCurveSlider1->setRange (0, 10, 0);
    decRelCurveSlider1->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider1->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    decRelCurveSlider1->addListener (this);

    addAndMakeVisible (pitchModSlider = new Slider ("Pitch Mod"));
    pitchModSlider->setRange (0, 10, 0);
    pitchModSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchModSlider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    pitchModSlider->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    pitchModSlider->addListener (this);

    addAndMakeVisible (pitchModLabel = new Label ("OSC2 Gain Label",
                                                  TRANS("Pitch Mod")));
    pitchModLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    pitchModLabel->setJustificationType (Justification::centredBottom);
    pitchModLabel->setEditable (false, false, false);
    pitchModLabel->setColour (TextEditor::textColourId, Colours::black);
    pitchModLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1WaveformLabel = new Label ("OSC1 Gain Label",
                                                      TRANS("Shape")));
    osc1WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc1WaveformLabel->setJustificationType (Justification::centredBottom);
    osc1WaveformLabel->setEditable (false, false, false);
    osc1WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc2WaveformLabel = new Label ("OSC1 Gain Label",
                                                      TRANS("Shape")));
    osc2WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc2WaveformLabel->setJustificationType (Justification::centredBottom);
    osc2WaveformLabel->setEditable (false, false, false);
    osc2WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc2WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc3WaveformLabel = new Label ("OSC1 Gain Label",
                                                      TRANS("Shape")));
    osc3WaveformLabel->setFont (Font ("Futura", 13.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.150f));
    osc3WaveformLabel->setJustificationType (Justification::centredBottom);
    osc3WaveformLabel->setEditable (false, false, false);
    osc3WaveformLabel->setColour (TextEditor::textColourId, Colours::black);
    osc3WaveformLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (attackCurveSlider2 = new Slider ("Attack Curve Slider 1"));
    attackCurveSlider2->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider2->setRange (0, 10, 0);
    attackCurveSlider2->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    attackCurveSlider2->addListener (this);

    addAndMakeVisible (decRelCurveSlider2 = new Slider ("Decay Release Curve Slider"));
    decRelCurveSlider2->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    decRelCurveSlider2->setRange (0, 10, 0);
    decRelCurveSlider2->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    decRelCurveSlider2->addListener (this);

    addAndMakeVisible (attackCurveSlider3 = new Slider ("Attack Curve Slider 1"));
    attackCurveSlider3->setTooltip (TRANS("set attack curve from exponential to linear"));
    attackCurveSlider3->setRange (0, 10, 0);
    attackCurveSlider3->setSliderStyle (Slider::LinearVertical);
    attackCurveSlider3->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    attackCurveSlider3->addListener (this);

    addAndMakeVisible (decRelCurveSlider3 = new Slider ("Decay Release Curve Slider"));
    decRelCurveSlider3->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    decRelCurveSlider3->setRange (0, 10, 0);
    decRelCurveSlider3->setSliderStyle (Slider::LinearVertical);
    decRelCurveSlider3->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    decRelCurveSlider3->addListener (this);

    drawable1 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable2 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable3 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    drawable4 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable5 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable6 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    drawable7 = Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size);
    drawable8 = Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize);
    drawable9 = Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize);
    drawable10 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable11 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
    drawable12 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable13 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);
    drawable14 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable15 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
    drawable16 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable17 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);
    drawable18 = Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize);
    drawable19 = Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize);
    drawable20 = Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize);
    drawable21 = Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 440);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

CustomPluginEditor::~CustomPluginEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    osc1GainSlider = nullptr;
    osc1GainLabel = nullptr;
    osc1OffsetSlider = nullptr;
    label2 = nullptr;
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
    osc2GainSlider2 = nullptr;
    osc2GainLabel2 = nullptr;
    osc3OffsetSlider = nullptr;
    osc3OffsetLabel = nullptr;
    osc3TuneSlider = nullptr;
    osc2TuneLabel2 = nullptr;
    osc3WaveformSlider = nullptr;
    filterLabel = nullptr;
    filterCutoffSider = nullptr;
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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CustomPluginEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff0e0e0e));

    {
        int x = 240, y = 62, width = 1, height = 356;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 337, y = 64, width = 1, height = 356;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 552, y = 64, width = 1, height = 356;
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
        int x = 474, y = 168, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable10 != 0);
        if (drawable10 != 0)
            drawable10->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 474, y = 128, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable11 != 0);
        if (drawable11 != 0)
            drawable11->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 168, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable12 != 0);
        if (drawable12 != 0)
            drawable12->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 128, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable13 != 0);
        if (drawable13 != 0)
            drawable13->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 474, y = 278, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable14 != 0);
        if (drawable14 != 0)
            drawable14->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 474, y = 238, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable15 != 0);
        if (drawable15 != 0)
            drawable15->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 278, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable16 != 0);
        if (drawable16 != 0)
            drawable16->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 238, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable17 != 0);
        if (drawable17 != 0)
            drawable17->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 474, y = 390, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable18 != 0);
        if (drawable18 != 0)
            drawable18->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 474, y = 350, width = 14, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable19 != 0);
        if (drawable19 != 0)
            drawable19->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 390, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable20 != 0);
        if (drawable20 != 0)
            drawable20->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    {
        int x = 518, y = 350, width = 12, height = 8;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable21 != 0);
        if (drawable21 != 0)
            drawable21->drawWithin (g, Rectangle<float> (x, y, width, height),
                                    RectanglePlacement::centred, 1.000f);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CustomPluginEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    osc1GainSlider->setBounds (16, 104, 64, 64);
    osc1GainLabel->setBounds (16, 88, 65, 24);
    osc1OffsetSlider->setBounds (16, 344, 64, 74);
    label2->setBounds (16, 328, 65, 24);
    oscillatorsLabel->setBounds (48, 64, 150, 24);
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
    osc2GainSlider2->setBounds (160, 104, 64, 64);
    osc2GainLabel2->setBounds (160, 88, 65, 24);
    osc3OffsetSlider->setBounds (160, 344, 64, 74);
    osc3OffsetLabel->setBounds (160, 328, 65, 24);
    osc3TuneSlider->setBounds (160, 263, 64, 64);
    osc2TuneLabel2->setBounds (160, 247, 65, 24);
    osc3WaveformSlider->setBounds (160, 178, 64, 64);
    filterLabel->setBounds (256, 64, 65, 24);
    filterCutoffSider->setBounds (256, 104, 64, 64);
    filterCutoffLabel->setBounds (256, 88, 65, 24);
    filterResonanceSlider->setBounds (256, 184, 64, 64);
    filterResonanceLabel->setBounds (240, 168, 100, 24);
    filterContourSlider->setBounds (568, 216, 64, 64);
    filterContourLabel->setBounds (568, 312, 65, 24);
    filterDriveSlider->setBounds (256, 264, 64, 64);
    filterDriveLabel->setBounds (256, 248, 65, 24);
    envelopesLabel->setBounds (400, 64, 100, 24);
    attackSlider1->setBounds (360, 112, 14, 80);
    attackSlider2->setBounds (360, 224, 14, 80);
    envAmpLabel->setBounds (352, 88, 100, 24);
    decaySlider1->setBounds (384, 112, 14, 80);
    sustainSlider1->setBounds (408, 112, 14, 80);
    releaseSlider1->setBounds (432, 112, 14, 80);
    envFilterLabel->setBounds (352, 200, 100, 24);
    decaySlider2->setBounds (384, 224, 14, 80);
    sustainSlider2->setBounds (408, 224, 14, 80);
    releaseSlider2->setBounds (432, 224, 14, 80);
    attackSlider3->setBounds (360, 336, 14, 80);
    envPitchLabel->setBounds (352, 312, 100, 24);
    decaySlider3->setBounds (384, 336, 14, 80);
    sustainSlider3->setBounds (408, 336, 14, 80);
    releaseSlider3->setBounds (432, 336, 14, 80);
    volumeSlider->setBounds (getWidth() - 18 - 64, 104, 64, 65);
    volumeLabel->setBounds (getWidth() - 18 - 65, 88, 65, 24);
    mainLabel->setBounds (getWidth() - 18 - 64, 64, 64, 24);
    titleLabel->setBounds ((getWidth() / 2) - (400 / 2), 12, 400, 24);
    attackCurveSlider1->setBounds (490, 122, 12, 60);
    decRelCurveSlider1->setBounds (504, 122, 12, 60);
    pitchModSlider->setBounds (568, 328, 64, 64);
    pitchModLabel->setBounds (560, 200, 80, 24);
    osc1WaveformLabel->setBounds (16, 168, 65, 24);
    osc2WaveformLabel->setBounds (88, 168, 64, 24);
    osc3WaveformLabel->setBounds (160, 168, 64, 24);
    attackCurveSlider2->setBounds (490, 232, 12, 60);
    decRelCurveSlider2->setBounds (504, 232, 12, 60);
    attackCurveSlider3->setBounds (490, 346, 12, 60);
    decRelCurveSlider3->setBounds (504, 346, 12, 60);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void CustomPluginEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == osc1GainSlider)
    {
        //[UserSliderCode_osc1GainSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc1GainSlider]
    }
    else if (sliderThatWasMoved == osc1OffsetSlider)
    {
        //[UserSliderCode_osc1OffsetSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc1OffsetSlider]
    }
    else if (sliderThatWasMoved == osc1TuneSlider)
    {
        //[UserSliderCode_osc1TuneSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc1TuneSlider]
    }
    else if (sliderThatWasMoved == osc1WaveformSlider)
    {
        //[UserSliderCode_osc1WaveformSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc1WaveformSlider]
    }
    else if (sliderThatWasMoved == osc2GainSlider)
    {
        //[UserSliderCode_osc2GainSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc2GainSlider]
    }
    else if (sliderThatWasMoved == osc2OffsetSlider)
    {
        //[UserSliderCode_osc2OffsetSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc2OffsetSlider]
    }
    else if (sliderThatWasMoved == osc2TuneSlider)
    {
        //[UserSliderCode_osc2TuneSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc2TuneSlider]
    }
    else if (sliderThatWasMoved == osc2WaveformSlider)
    {
        //[UserSliderCode_osc2WaveformSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc2WaveformSlider]
    }
    else if (sliderThatWasMoved == osc2GainSlider2)
    {
        //[UserSliderCode_osc2GainSlider2] -- add your slider handling code here..
        //[/UserSliderCode_osc2GainSlider2]
    }
    else if (sliderThatWasMoved == osc3OffsetSlider)
    {
        //[UserSliderCode_osc3OffsetSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc3OffsetSlider]
    }
    else if (sliderThatWasMoved == osc3TuneSlider)
    {
        //[UserSliderCode_osc3TuneSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc3TuneSlider]
    }
    else if (sliderThatWasMoved == osc3WaveformSlider)
    {
        //[UserSliderCode_osc3WaveformSlider] -- add your slider handling code here..
        //[/UserSliderCode_osc3WaveformSlider]
    }
    else if (sliderThatWasMoved == filterCutoffSider)
    {
        //[UserSliderCode_filterCutoffSider] -- add your slider handling code here..
        //[/UserSliderCode_filterCutoffSider]
    }
    else if (sliderThatWasMoved == filterResonanceSlider)
    {
        //[UserSliderCode_filterResonanceSlider] -- add your slider handling code here..
        //[/UserSliderCode_filterResonanceSlider]
    }
    else if (sliderThatWasMoved == filterContourSlider)
    {
        //[UserSliderCode_filterContourSlider] -- add your slider handling code here..
        //[/UserSliderCode_filterContourSlider]
    }
    else if (sliderThatWasMoved == filterDriveSlider)
    {
        //[UserSliderCode_filterDriveSlider] -- add your slider handling code here..
        //[/UserSliderCode_filterDriveSlider]
    }
    else if (sliderThatWasMoved == attackSlider1)
    {
        //[UserSliderCode_attackSlider1] -- add your slider handling code here..
        //[/UserSliderCode_attackSlider1]
    }
    else if (sliderThatWasMoved == attackSlider2)
    {
        //[UserSliderCode_attackSlider2] -- add your slider handling code here..
        //[/UserSliderCode_attackSlider2]
    }
    else if (sliderThatWasMoved == decaySlider1)
    {
        //[UserSliderCode_decaySlider1] -- add your slider handling code here..
        //[/UserSliderCode_decaySlider1]
    }
    else if (sliderThatWasMoved == sustainSlider1)
    {
        //[UserSliderCode_sustainSlider1] -- add your slider handling code here..
        //[/UserSliderCode_sustainSlider1]
    }
    else if (sliderThatWasMoved == releaseSlider1)
    {
        //[UserSliderCode_releaseSlider1] -- add your slider handling code here..
        //[/UserSliderCode_releaseSlider1]
    }
    else if (sliderThatWasMoved == decaySlider2)
    {
        //[UserSliderCode_decaySlider2] -- add your slider handling code here..
        //[/UserSliderCode_decaySlider2]
    }
    else if (sliderThatWasMoved == sustainSlider2)
    {
        //[UserSliderCode_sustainSlider2] -- add your slider handling code here..
        //[/UserSliderCode_sustainSlider2]
    }
    else if (sliderThatWasMoved == releaseSlider2)
    {
        //[UserSliderCode_releaseSlider2] -- add your slider handling code here..
        //[/UserSliderCode_releaseSlider2]
    }
    else if (sliderThatWasMoved == attackSlider3)
    {
        //[UserSliderCode_attackSlider3] -- add your slider handling code here..
        //[/UserSliderCode_attackSlider3]
    }
    else if (sliderThatWasMoved == decaySlider3)
    {
        //[UserSliderCode_decaySlider3] -- add your slider handling code here..
        //[/UserSliderCode_decaySlider3]
    }
    else if (sliderThatWasMoved == sustainSlider3)
    {
        //[UserSliderCode_sustainSlider3] -- add your slider handling code here..
        //[/UserSliderCode_sustainSlider3]
    }
    else if (sliderThatWasMoved == releaseSlider3)
    {
        //[UserSliderCode_releaseSlider3] -- add your slider handling code here..
        //[/UserSliderCode_releaseSlider3]
    }
    else if (sliderThatWasMoved == volumeSlider)
    {
        //[UserSliderCode_volumeSlider] -- add your slider handling code here..
        //[/UserSliderCode_volumeSlider]
    }
    else if (sliderThatWasMoved == attackCurveSlider1)
    {
        //[UserSliderCode_attackCurveSlider1] -- add your slider handling code here..
        //[/UserSliderCode_attackCurveSlider1]
    }
    else if (sliderThatWasMoved == decRelCurveSlider1)
    {
        //[UserSliderCode_decRelCurveSlider1] -- add your slider handling code here..
        //[/UserSliderCode_decRelCurveSlider1]
    }
    else if (sliderThatWasMoved == pitchModSlider)
    {
        //[UserSliderCode_pitchModSlider] -- add your slider handling code here..
        //[/UserSliderCode_pitchModSlider]
    }
    else if (sliderThatWasMoved == attackCurveSlider2)
    {
        //[UserSliderCode_attackCurveSlider2] -- add your slider handling code here..
        //[/UserSliderCode_attackCurveSlider2]
    }
    else if (sliderThatWasMoved == decRelCurveSlider2)
    {
        //[UserSliderCode_decRelCurveSlider2] -- add your slider handling code here..
        //[/UserSliderCode_decRelCurveSlider2]
    }
    else if (sliderThatWasMoved == attackCurveSlider3)
    {
        //[UserSliderCode_attackCurveSlider3] -- add your slider handling code here..
        //[/UserSliderCode_attackCurveSlider3]
    }
    else if (sliderThatWasMoved == decRelCurveSlider3)
    {
        //[UserSliderCode_decRelCurveSlider3] -- add your slider handling code here..
        //[/UserSliderCode_decRelCurveSlider3]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CustomPluginEditor" componentName="GUIEditor"
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="0" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="650" initialHeight="440">
  <BACKGROUND backgroundColour="ff0e0e0e">
    <RECT pos="240 62 1 356" fill="solid: ffcfcfcf" hasStroke="0"/>
    <RECT pos="337 64 1 356" fill="solid: ffcfcfcf" hasStroke="0"/>
    <RECT pos="552 64 1 356" fill="solid: ffcfcfcf" hasStroke="0"/>
    <RECT pos="50%c 49 95% 1" fill="solid: ffcfcfcf" hasStroke="0"/>
    <IMAGE pos="66 224 10 6" resource="oscSquareWaveSymbol_svg2" opacity="1"
           mode="1"/>
    <IMAGE pos="44 224 10 6" resource="oscSawWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="20 224 10 6" resource="oscSineWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="138 224 10 6" resource="oscSquareWaveSymbol_svg2" opacity="1"
           mode="1"/>
    <IMAGE pos="116 224 10 6" resource="oscSawWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="92 224 10 6" resource="oscSineWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="210 224 10 6" resource="oscSquareWaveSymbol_svg2" opacity="1"
           mode="1"/>
    <IMAGE pos="188 224 10 6" resource="oscSawWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="164 224 10 6" resource="oscSineWaveSymbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="474 168 14 8" resource="attackCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="474 128 14 8" resource="attackCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
    <IMAGE pos="518 168 12 8" resource="decayCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="518 128 12 8" resource="decayCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
    <IMAGE pos="474 278 14 8" resource="attackCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="474 238 14 8" resource="attackCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
    <IMAGE pos="518 278 12 8" resource="decayCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="518 238 12 8" resource="decayCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
    <IMAGE pos="474 390 14 8" resource="attackCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="474 350 14 8" resource="attackCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
    <IMAGE pos="518 390 12 8" resource="decayCurveLinear_symbol_svg" opacity="1"
           mode="1"/>
    <IMAGE pos="518 350 12 8" resource="decayCurveExponential_symbol_svg"
           opacity="1" mode="1"/>
  </BACKGROUND>
  <SLIDER name="OSC1 Gain" id="cf30401ea9ec13b7" memberName="osc1GainSlider"
          virtualName="" explicitFocusOrder="0" pos="16 104 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC1 Gain Label" id="8c1dba7561c29d0e" memberName="osc1GainLabel"
         virtualName="" explicitFocusOrder="0" pos="16 88 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC1 Offset Slider" id="2c01aced8ab3f969" memberName="osc1OffsetSlider"
          virtualName="" explicitFocusOrder="0" pos="16 344 64 74" textboxoutline="8e989b"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="10" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="new label" id="130e19613ef45fa2" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="16 328 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Offset&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="13" kerning="0.14999999999999999445" bold="0" italic="0"
         justification="20"/>
  <LABEL name="Oscillators" id="bdf12a7c74546521" memberName="oscillatorsLabel"
         virtualName="" explicitFocusOrder="0" pos="48 64 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Oscillators" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="20" kerning="0.10799999999999999878" bold="0" italic="0"
         justification="12"/>
  <SLIDER name="OSC1 Tune" id="f104badc53c571be" memberName="osc1TuneSlider"
          virtualName="" explicitFocusOrder="0" pos="16 263 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC1 Tune Label" id="51f1e36b065375f5" memberName="osc1TuneLabel"
         virtualName="" explicitFocusOrder="0" pos="16 247 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Tune" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC1 Waveform Slider" id="46001fa6ddb7745f" memberName="osc1WaveformSlider"
          virtualName="" explicitFocusOrder="0" pos="16 178 64 64" textboxoutline="8e989b"
          min="0" max="10" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="OSC2 Gain" id="cc4cbf0247eaeadc" memberName="osc2GainSlider"
          virtualName="" explicitFocusOrder="0" pos="88 104 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Gain Label" id="e1724479d077893e" memberName="osc2GainLabel"
         virtualName="" explicitFocusOrder="0" pos="88 88 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC2 Offset Slider" id="28bff1c0c886e6c1" memberName="osc2OffsetSlider"
          virtualName="" explicitFocusOrder="0" pos="88 344 64 74" textboxoutline="8e989b"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="10" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Offset Label" id="3df52e876f4857f8" memberName="osc2OffsetLabel"
         virtualName="" explicitFocusOrder="0" pos="88 328 65 24" outlineCol="9e1919"
         edTextCol="ff000000" edBkgCol="0" labelText="Offset&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="13" kerning="0.14999999999999999445" bold="0" italic="0"
         justification="20"/>
  <SLIDER name="OSC2 Tune" id="8aed4c3dfed5789a" memberName="osc2TuneSlider"
          virtualName="" explicitFocusOrder="0" pos="88 263 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Tune Label" id="eab97706afe9f1d4" memberName="osc2TuneLabel"
         virtualName="" explicitFocusOrder="0" pos="88 247 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Tune" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC2 Waveform Slider" id="a34a22b9a86f2678" memberName="osc2WaveformSlider"
          virtualName="" explicitFocusOrder="0" pos="88 178 64 64" textboxoutline="8e989b"
          min="0" max="10" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="OSC2 Gain" id="d28acbcd0a88c47e" memberName="osc2GainSlider2"
          virtualName="" explicitFocusOrder="0" pos="160 104 64 64" textboxtext="fff0f8ff"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Gain Label" id="f75a4adced662ca1" memberName="osc2GainLabel2"
         virtualName="" explicitFocusOrder="0" pos="160 88 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC2 Offset Slider" id="d32e1a0320a827f2" memberName="osc3OffsetSlider"
          virtualName="" explicitFocusOrder="0" pos="160 344 64 74" thumbcol="ff42a2c8"
          textboxtext="ffffffff" textboxoutline="8e989b" min="0" max="10"
          int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="10" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Offset Label" id="e85270873605d47" memberName="osc3OffsetLabel"
         virtualName="" explicitFocusOrder="0" pos="160 328 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Offset&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="13" kerning="0.14999999999999999445" bold="0" italic="0"
         justification="20"/>
  <SLIDER name="OSC2 Tune" id="99f3444850d24206" memberName="osc3TuneSlider"
          virtualName="" explicitFocusOrder="0" pos="160 263 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Tune Label" id="bfa939774c50327b" memberName="osc2TuneLabel2"
         virtualName="" explicitFocusOrder="0" pos="160 247 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Tune" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="OSC2 Waveform Slider" id="97e76758ff29a0c1" memberName="osc3WaveformSlider"
          virtualName="" explicitFocusOrder="0" pos="160 178 64 64" textboxoutline="8e989b"
          min="0" max="10" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="20" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Filter" id="b56cf54043f691ee" memberName="filterLabel"
         virtualName="" explicitFocusOrder="0" pos="256 64 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filter" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="20" kerning="0.10799999999999999878"
         bold="0" italic="0" justification="12"/>
  <SLIDER name="Filter Cutoff Slider" id="55b541149dbbf120" memberName="filterCutoffSider"
          virtualName="" explicitFocusOrder="0" pos="256 104 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Filter Cutoff Label" id="a4f8ca99709ab601" memberName="filterCutoffLabel"
         virtualName="" explicitFocusOrder="0" pos="256 88 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Filter Resonance Slider" id="82df4e64b64a0a53" memberName="filterResonanceSlider"
          virtualName="" explicitFocusOrder="0" pos="256 184 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Filter Resonance Label" id="989af3edd39865f1" memberName="filterResonanceLabel"
         virtualName="" explicitFocusOrder="0" pos="240 168 100 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Resonance" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Filter Contour Slider" id="b5881d01d35fcdf" memberName="filterContourSlider"
          virtualName="" explicitFocusOrder="0" pos="568 216 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Filter Contour Label" id="17c5d6b6256ae1d3" memberName="filterContourLabel"
         virtualName="" explicitFocusOrder="0" pos="568 312 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Contour" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Filter Drive Slider" id="8943fff55078a784" memberName="filterDriveSlider"
          virtualName="" explicitFocusOrder="0" pos="256 264 64 64" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Filter Drive Label" id="c57979373c6cf703" memberName="filterDriveLabel"
         virtualName="" explicitFocusOrder="0" pos="256 248 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Drive" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <LABEL name="Envelopes" id="ba5043e83d82975b" memberName="envelopesLabel"
         virtualName="" explicitFocusOrder="0" pos="400 64 100 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Envelopes" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="20" kerning="0.10799999999999999878"
         bold="0" italic="0" justification="12"/>
  <SLIDER name="Attack Slider 1" id="a1c30a5a1772456f" memberName="attackSlider1"
          virtualName="" explicitFocusOrder="0" pos="360 112 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 1" id="7d1305d4891f2f0a" memberName="attackSlider2"
          virtualName="" explicitFocusOrder="0" pos="360 224 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Envelope Amp Label" id="a6b5157ca349be15" memberName="envAmpLabel"
         virtualName="" explicitFocusOrder="0" pos="352 88 100 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Amplitude" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Decay Slider 1" id="17131aa3b5949b70" memberName="decaySlider1"
          virtualName="" explicitFocusOrder="0" pos="384 112 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Sustain Slider 1" id="b1e8fa99d7490016" memberName="sustainSlider1"
          virtualName="" explicitFocusOrder="0" pos="408 112 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Release Slider 1" id="5289f3c25f8aeed3" memberName="releaseSlider1"
          virtualName="" explicitFocusOrder="0" pos="432 112 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Envelope Filter Label" id="1685d0af57af2036" memberName="envFilterLabel"
         virtualName="" explicitFocusOrder="0" pos="352 200 100 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filter Cutoff" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="13" kerning="0.14999999999999999445" bold="0" italic="0"
         justification="20"/>
  <SLIDER name="Decay Slider 2" id="24ece39cc281d642" memberName="decaySlider2"
          virtualName="" explicitFocusOrder="0" pos="384 224 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 1" id="bf2b4982fc3f3634" memberName="sustainSlider2"
          virtualName="" explicitFocusOrder="0" pos="408 224 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 1" id="b9c3135123abfde2" memberName="releaseSlider2"
          virtualName="" explicitFocusOrder="0" pos="432 224 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 3" id="629d5ed74dcedc1c" memberName="attackSlider3"
          virtualName="" explicitFocusOrder="0" pos="360 336 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Envelope Pitch Label" id="2d5b35a0c998371" memberName="envPitchLabel"
         virtualName="" explicitFocusOrder="0" pos="352 312 100 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch Mod." editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Attack Slider 1" id="e88f021a7e6ea428" memberName="decaySlider3"
          virtualName="" explicitFocusOrder="0" pos="384 336 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 1" id="aa2dea5d765ca18f" memberName="sustainSlider3"
          virtualName="" explicitFocusOrder="0" pos="408 336 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Slider 1" id="f25cc2a6a41bc3d4" memberName="releaseSlider3"
          virtualName="" explicitFocusOrder="0" pos="432 336 14 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Volume Slider" id="e71e67d6e577b730" memberName="volumeSlider"
          virtualName="" explicitFocusOrder="0" pos="18Rr 104 64 65" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Main Volume Label" id="47e5d09b695d3ce" memberName="volumeLabel"
         virtualName="" explicitFocusOrder="0" pos="18Rr 88 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Volume" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <LABEL name="Main Label" id="eb30067a68a30ee5" memberName="mainLabel"
         virtualName="" explicitFocusOrder="0" pos="18Rr 64 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Main" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="20" kerning="0.10799999999999999878"
         bold="0" italic="0" justification="12"/>
  <LABEL name="Title" id="b31029b9f2a6eb9e" memberName="titleLabel" virtualName=""
         explicitFocusOrder="0" pos="0Cc 12 400 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Vermeer Monosynth-1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Futura" fontsize="24" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Attack Curve Slider 1" id="f066fb14f2ea65c7" memberName="attackCurveSlider1"
          virtualName="" explicitFocusOrder="0" pos="490 122 12 60" tooltip="set attack curve from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Decay Release Curve Slider" id="d41eac1b694bbece" memberName="decRelCurveSlider1"
          virtualName="" explicitFocusOrder="0" pos="504 122 12 60" tooltip="set decay and release curves from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Pitch Mod" id="8fbce18f484a61c9" memberName="pitchModSlider"
          virtualName="" explicitFocusOrder="0" pos="568 328 64 64" textboxtext="fff0f8ff"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="OSC2 Gain Label" id="751d97ce4eeb9847" memberName="pitchModLabel"
         virtualName="" explicitFocusOrder="0" pos="560 200 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch Mod" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <LABEL name="OSC1 Gain Label" id="721ccc8325a468df" memberName="osc1WaveformLabel"
         virtualName="" explicitFocusOrder="0" pos="16 168 65 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Shape" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <LABEL name="OSC1 Gain Label" id="9dfe899572400547" memberName="osc2WaveformLabel"
         virtualName="" explicitFocusOrder="0" pos="88 168 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Shape" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <LABEL name="OSC1 Gain Label" id="d96630dd602f7757" memberName="osc3WaveformLabel"
         virtualName="" explicitFocusOrder="0" pos="160 168 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Shape" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Futura" fontsize="13" kerning="0.14999999999999999445"
         bold="0" italic="0" justification="20"/>
  <SLIDER name="Attack Curve Slider 1" id="83e786b97f601c43" memberName="attackCurveSlider2"
          virtualName="" explicitFocusOrder="0" pos="490 232 12 60" tooltip="set attack curve from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Decay Release Curve Slider" id="aed01f36a1a3381b" memberName="decRelCurveSlider2"
          virtualName="" explicitFocusOrder="0" pos="504 232 12 60" tooltip="set decay and release curves from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Attack Curve Slider 1" id="2c14c358dccb3e4f" memberName="attackCurveSlider3"
          virtualName="" explicitFocusOrder="0" pos="490 346 12 60" tooltip="set attack curve from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Decay Release Curve Slider" id="1600ec17d43af31b" memberName="decRelCurveSlider3"
          virtualName="" explicitFocusOrder="0" pos="504 346 12 60" tooltip="set decay and release curves from exponential to linear"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: oscSquareWaveSymbol_svg, 599, "../../../../../Pictures/OscSquareWaveSymbol.svg"
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
44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::oscSquareWaveSymbol_svg = (const char*) resource_CustomPluginEditor_oscSquareWaveSymbol_svg;
const int CustomPluginEditor::oscSquareWaveSymbol_svgSize = 599;

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
44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::oscSquareWaveSymbol_svg2 = (const char*) resource_CustomPluginEditor_oscSquareWaveSymbol_svg2;
const int CustomPluginEditor::oscSquareWaveSymbol_svg2Size = 599;

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
44,49,54,32,55,46,56,44,49,54,32,55,46,56,44,53,46,55,32,49,54,46,49,44,49,54,32,50,48,44,49,54,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::oscSawWaveSymbol_svg = (const char*) resource_CustomPluginEditor_oscSawWaveSymbol_svg;
const int CustomPluginEditor::oscSawWaveSymbol_svgSize = 559;

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
13,10,0,0};

const char* CustomPluginEditor::oscSineWaveSymbol_svg = (const char*) resource_CustomPluginEditor_oscSineWaveSymbol_svg;
const int CustomPluginEditor::oscSineWaveSymbol_svgSize = 711;

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
49,54,44,49,49,46,52,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::attackCurveLinear_symbol_svg = (const char*) resource_CustomPluginEditor_attackCurveLinear_symbol_svg;
const int CustomPluginEditor::attackCurveLinear_symbol_svgSize = 531;

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
99,48,45,54,46,49,44,53,45,49,49,44,49,49,46,49,45,49,49,104,49,86,49,56,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::attackCurveExponential_symbol_svg = (const char*) resource_CustomPluginEditor_attackCurveExponential_symbol_svg;
const int CustomPluginEditor::attackCurveExponential_symbol_svgSize = 547;

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
49,46,52,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::decayCurveLinear_symbol_svg = (const char*) resource_CustomPluginEditor_decayCurveLinear_symbol_svg;
const int CustomPluginEditor::decayCurveLinear_symbol_svgSize = 527;

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
53,44,49,48,44,49,48,44,49,48,86,49,57,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0};

const char* CustomPluginEditor::decayCurveExponential_symbol_svg = (const char*) resource_CustomPluginEditor_decayCurveExponential_symbol_svg;
const int CustomPluginEditor::decayCurveExponential_symbol_svgSize = 537;


//[EndFile] You can add extra defines here...
//[/EndFile]
