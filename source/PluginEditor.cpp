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
MonosynthPluginAudioProcessorEditor::MonosynthPluginAudioProcessorEditor (MonosynthPluginAudioProcessor& owner)
    : AudioProcessorEditor (owner),
        midiKeyboard (owner.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        timecodeDisplayLabel (String()),
	
	mainLabel(nullptr),
	titleLabel(nullptr),


	volumeLabel(nullptr),


	lfoLabel(nullptr),

	lfoRateLabel(nullptr),
	lfoModeLabel(nullptr),
	lfoIntensityLabel(nullptr),
	modTargetLabel(nullptr),
	modTargetCutoffLabel(nullptr),
	modTargetPitchLabel(nullptr),
	modTargetOffLabel(nullptr),

	saturationLabel(nullptr),


	volumeSlider(nullptr),




	modTargetSlider(nullptr),

	lfoRateSlider(nullptr),
	lfoModeSlider(nullptr),
	lfoIntensitySlider(nullptr),
	lfoSyncSlider(nullptr),
	lfoSyncedFreqSlider(nullptr),




	filterOrderSlider(nullptr),

	saturationSwitchSlider(nullptr),

	saturationSlider(nullptr),

oversampleSwitchSlider(nullptr),
    softClipSwitchSlider(nullptr)
	
      
{
    // add all the sliders..
    
    font = "Futura";
    

	typedef ParameterSlider::style knobStyle;
    
    
    
    
    
    
    
    // OSCILLATOR SECTION
    oscillatorSection = std::unique_ptr<OscillatorSection>(new OscillatorSection(owner));
    addAndMakeVisible(oscillatorSection.get());
    
    
    // FILTER SECTION
    filterSection = std::unique_ptr<FilterSection> (new FilterSection(owner));
    addAndMakeVisible(filterSection.get());
    
    
    //Envelope Section
    envelopeSection = std::unique_ptr<EnvelopeSection> (new EnvelopeSection(owner));
    addAndMakeVisible(envelopeSection.get());
    
    //
    // TITLE
    //
    addAndMakeVisible (titleLabel = new Label ("Title",
                                               TRANS("Vermeer Monosynth-1")));
    titleLabel->setFont (Font (font, 24.00f, Font::plain).withExtraKerningFactor (0.150f));
    titleLabel->setJustificationType (Justification::centredBottom);
    titleLabel->setEditable (false, false, false);
    titleLabel->setColour (Label::textColourId, Colours::white);
    titleLabel->setColour (TextEditor::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    //
    // MAIN
    //
	
	
    addAndMakeVisible (mainLabel = new Label("Main Label", TRANS("Main")));
    mainLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    mainLabel->setJustificationType (Justification::centredTop);
    mainLabel->setEditable (false, false, false);
    mainLabel->setColour (TextEditor::textColourId, Colours::black);
    mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    //
    // main volume
    //
	volumeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.gainParam, knobStyle(ROTARY)));
    addAndMakeVisible (volumeSlider.get());  //
    
    addAndMakeVisible (volumeLabel = new Label ("Main Volume Label",
                                                TRANS("Volume")));              //
    volumeLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    volumeLabel->setJustificationType (Justification::centredBottom);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
   
    
    
   
   
    
    
   


	


	//
	// LFO
	//
    addAndMakeVisible (lfoLabel = new Label ("lfoLabel",
                                                TRANS("LFO")));                          //
    lfoLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    lfoLabel->setJustificationType (Justification::centredTop);
    lfoLabel->setEditable (false, false, false);
    lfoLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (lfoRateLabel = new Label ("lfoRateLabel",
                                                    TRANS("Rate")));          //
    lfoRateLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoRateLabel->setJustificationType (Justification::centredBottom);
    lfoRateLabel->setEditable (false, false, false);
    lfoRateLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoRateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
	lfoRateSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.lfoRateParam, knobStyle(ROTARY)));
	addAndMakeVisible(lfoRateSlider.get());  //
    
    addAndMakeVisible (lfoModeLabel = new Label ("lfoModeLabel",
                                                 TRANS("Shape")));          //
    lfoModeLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoModeLabel->setJustificationType (Justification::centredBottom);
    lfoModeLabel->setEditable (false, false, false);
    lfoModeLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoModeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	lfoModeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.lfoModeParam, LINEARHORIZONTAL));
	addAndMakeVisible(lfoModeSlider.get());  //
	

    addAndMakeVisible (lfoIntensityLabel = new Label ("lfoIntensityLabel",
                                                 TRANS("Mod Amt")));          //
    lfoIntensityLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoIntensityLabel->setJustificationType (Justification::centredBottom);
    lfoIntensityLabel->setEditable (false, false, false);
    lfoIntensityLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoIntensityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
	lfoIntensitySlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.lfoIntensityParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoIntensitySlider.get());  //
    
	lfoSyncedFreqSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.lfoDivisionParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoSyncedFreqSlider.get());

	lfoSyncSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.lfoSyncParam, LINEARHORIZONTAL));
	addAndMakeVisible(lfoSyncSlider.get());
	
	
    
    
    //
    // Modulation Target
    //
	modTargetSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.modTargetParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(modTargetSlider.get());  //
  
    
    addAndMakeVisible (modTargetLabel = new Label ("modTargetLabel",
                                                      TRANS("Target")));          //
    modTargetLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetLabel->setJustificationType (Justification::centredBottom);
    modTargetLabel->setEditable (false, false, false);
    modTargetLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (modTargetOffLabel = new Label ("modTargetOffLabel",
                                                        TRANS("-off")));          //
    modTargetOffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetOffLabel->setJustificationType (Justification::centredLeft);
    modTargetOffLabel->setEditable (false, false, false);
    modTargetOffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetOffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    addAndMakeVisible (modTargetPitchLabel = new Label ("modTargetPitchLabel",
                                                      TRANS("-pitch")));          //
    modTargetPitchLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetPitchLabel->setJustificationType (Justification::centredLeft);
    modTargetPitchLabel->setEditable (false, false, false);
    modTargetPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    addAndMakeVisible (modTargetCutoffLabel = new Label ("modTargetCutoffLabel",
                                                        TRANS("-cutoff")));          //
    modTargetCutoffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetCutoffLabel->setJustificationType (Justification::centredLeft);
    modTargetCutoffLabel->setEditable (false, false, false);
    modTargetCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
	
    
    
    // Filter Order; switch from VCA->filter to filter->VCA
	filterOrderSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.filterOrderParam, LINEARHORIZONTAL));
    addAndMakeVisible(filterOrderSlider.get());  //
   
    
    
    
   

	//Saturation
	saturationSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.saturationParam, knobStyle(ROTARY)));
	addAndMakeVisible(saturationSlider.get());  //


	saturationSwitchSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.waveshapeSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(saturationSwitchSlider.get());
    
    
    saturationModeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.waveshapeModeParam, knobStyle(LINEARHORIZONTAL)));
    addAndMakeVisible(saturationModeSlider.get());
   
    
    addAndMakeVisible (saturationLabel = new Label ("saturationLabel",
                                                         TRANS("Saturation")));          //
    saturationLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    saturationLabel->setJustificationType (Justification::centredBottom);
    saturationLabel->setEditable (false, false, false);
    saturationLabel->setColour (TextEditor::textColourId, Colours::black);
    saturationLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
	
	
    
    //Oversample switch
    oversampleSwitchSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.oversampleSwitchParam, knobStyle(LINEARHORIZONTAL)));
    addAndMakeVisible(oversampleSwitchSlider.get());
    
    
    //Softclip switch
    softClipSwitchSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*owner.softClipSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(softClipSwitchSlider.get());
    
    
    // Keyboard
    addAndMakeVisible(midiKeyboard);
    midiKeyboard.setAvailableRange(12, 115);
    midiKeyboard.setLowestVisibleKey(36);
    midiKeyboard.setKeyWidth(midiKeyboard.getKeyWidth() * 1.5);
    
    //SEQUENCER
    //sequencer = std::unique_ptr<Sequencer>(new Sequencer(owner));
    //addAndMakeVisible(sequencer.get());
    
    
    
    
    
    
    
    // OSCILLATOR SECTION
    oscillatorSection = std::unique_ptr<OscillatorSection>(new OscillatorSection(owner));
    addAndMakeVisible(oscillatorSection.get());
    
    
    
    
    
                       
    //
    // Drawables for symbols/icons
    //
    
   
    
    

	// SIGH.. swapped the graphics out
    drawable11 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable10 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
    
	drawable13 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable12 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
    
	drawable15 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable14 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
    
	drawable17 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable16 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
 
	drawable19 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveLinear_symbol_svg, attackCurveLinear_symbol_svgSize));
    drawable18 = std::unique_ptr<Drawable>(Drawable::createFromImageData (attackCurveExponential_symbol_svg, attackCurveExponential_symbol_svgSize));
 
	drawable21 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveLinear_symbol_svg, decayCurveLinear_symbol_svgSize));
    drawable20 = std::unique_ptr<Drawable>(Drawable::createFromImageData (decayCurveExponential_symbol_svg, decayCurveExponential_symbol_svgSize));
    
   
    

    // set resize limits for this plug-in
	
    setResizeLimits (840, 720, 840, 720);
	

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (owner.lastUIWidth,
             owner.lastUIHeight);

    // start a timer which will keep our timecode display updated
    startTimerHz (60);
    
    
}

MonosynthPluginAudioProcessorEditor::~MonosynthPluginAudioProcessorEditor()
{

}

//==============================================================================
void MonosynthPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour (0xff0e0e0e));
    
    {
        int x = 305, y = 64, width = 1, height = 398;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = 401, y = 64, width = 1, height = 398;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    {
        int x = 617, y = 64, width = 1, height = 398;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    
    {
        int x = 713, y = 64, width = 1, height = 398;
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

void MonosynthPluginAudioProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    int stripWidth = 72;
    int moduleMarginX = 16;
    
    // TITLE
    
    int titleHeight = 48;
    titleLabel->setBounds (area.removeFromTop(titleHeight));
    titleLabel->setJustificationType(Justification::centred);
    
    Rectangle<int> parameterArea (area.removeFromTop(600));
    // OSCILLATOR SECTION
    int oscHeight = 520;
    int oscWidth = stripWidth * 4;
    oscillatorSection->setBounds(parameterArea.removeFromLeft(oscWidth + moduleMarginX));
    
	
    // FILTER
    filterSection->setBounds (parameterArea.removeFromLeft(stripWidth + moduleMarginX));
	
	
    //ENVELOPES
    envelopeSection->setBounds(parameterArea.removeFromLeft((stripWidth * 3) + moduleMarginX));
    envelopeSection->setAlwaysOnTop(true);
    
    
    // MAIN
    mainLabel->setBounds (getWidth() - 24 - 64, 64, 64, 24);
    volumeSlider->setBounds (getWidth() - 24 - 64, 104, 64, 65);
    softClipSwitchSlider->setBounds(getWidth() - 24, 110, 12, 40);
    volumeLabel->setBounds (getWidth() - 23 - 65, 88, 65, 24);

   
    filterOrderSlider->setBounds(getWidth() - 24-  64, 340, 64,64);
    
  

	saturationSlider->setBounds(getWidth() - 24 - 64, 184, 64, 65);
    saturationLabel->setBounds (getWidth() - 23 - 65, 168, 65, 24);
    saturationSwitchSlider->setBounds(getWidth() - 24, 190, 12,40);
    saturationModeSlider->setBounds(getWidth() - 24 - 64, 240, 64,64);
    
    
    oversampleSwitchSlider->setBounds(getWidth() - 24 - 24, 8, 36, 36);
    
    
    
   

    lfoLabel->setBounds             (628, 64, 80, 24);

    lfoRateLabel->setBounds         (634, 88, 64, 24);
    lfoRateSlider->setBounds        (634, 104, 64, 64);
	lfoSyncedFreqSlider->setBounds	(634, 104, 64, 64);

    lfoModeLabel->setBounds         (634, 168, 64, 24);
    lfoModeSlider->setBounds        (634, 178, 64, 64);

    lfoIntensityLabel->setBounds    (634, 247, 64, 24);
    lfoIntensitySlider->setBounds   (634, 263, 64, 64);
	lfoSyncSlider->setBounds		(634, 408, 64, 64);

    modTargetLabel->setBounds       (634, 328, 64, 24);
    modTargetSlider->setBounds      (612, 352, 64, 54);

    modTargetOffLabel->setBounds  (650, 352, 64, 24);
    modTargetCutoffLabel->setBounds (650, 368, 64, 24);
    modTargetPitchLabel->setBounds  (650, 384, 64, 24);


   
    
    
    
   
    
    Rectangle<int> r(getLocalBounds().reduced(8));
    midiKeyboard.setBounds(r.removeFromBottom(100));
    
    //sequencer->setBounds(r.removeFromBottom(midiKeyboard.getHeight() + 40));
    
   

    getProcessor().lastUIWidth = getWidth();
    getProcessor().lastUIHeight = getHeight();
}

//==============================================================================
void MonosynthPluginAudioProcessorEditor::timerCallback()
{
    updateTimecodeDisplay (getProcessor().lastPosInfo);

    
    /*
	if (filterSelectSlider->getValue() == 1)
	{
		filterDriveSlider->setAlpha(0.2);
		filterDriveLabel->setAlpha(0.2);
		filterDriveSlider->setEnabled(false);
	}
	else
	{
		filterDriveSlider->setAlpha(1.0);
		filterDriveSlider->setEnabled(true);
		filterDriveLabel->setAlpha(1.0);
	}
     */

	if (getProcessor().lfoSynced()) // free rate LFO
	{
		lfoRateSlider->setVisible(false);
		lfoRateSlider->setEnabled(false);

		lfoSyncedFreqSlider->setVisible(true);
		lfoSyncedFreqSlider->setEnabled(true);
	}
	else
	{
		lfoRateSlider->setVisible(true);
		lfoRateSlider->setEnabled(true);

		lfoSyncedFreqSlider->setVisible(false);
		lfoSyncedFreqSlider->setEnabled(false);
	}
	
	/*
	if (getProcessor().noteIsBeingPlayed())
	{
		filterSelectSlider->setAlpha(0.2);
		filterSelectSlider->setEnabled(false);
	}		
	else
	{
		filterSelectSlider->setAlpha(1.0);
		filterSelectSlider->setEnabled(true);
	}
		*/
	
    if (getProcessor().saturationOn())
    {
        saturationSlider->setAlpha(1.0f);
        saturationSlider->setEnabled(true);
    }
    else
    {
        saturationSlider->setAlpha(0.6f);
        saturationSlider->setEnabled(false);
    }
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










