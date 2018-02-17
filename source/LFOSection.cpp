/*
  ==============================================================================

    LFOSection.cpp
    Created: 17 Feb 2018 11:32:31am
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOSection.h"

//==============================================================================
LFOSection::LFOSection(MonosynthPluginAudioProcessor& p) : 
    processor(p)
{
    font = "Futura";
    
    typedef ParameterSlider::style knobStyle;

    
    lfoLabel = std::unique_ptr<Label>( new Label ("lfoLabel", TRANS("LFO")));
    addAndMakeVisible (lfoLabel.get());                        //
    lfoLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    lfoLabel->setJustificationType (Justification::centredTop);
    lfoLabel->setEditable (false, false, false);
    lfoLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    lfoRateLabel = std::unique_ptr<Label> ( new Label ("lfoRateLabel", TRANS("Rate")));
    addAndMakeVisible (lfoRateLabel.get());          //
    lfoRateLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoRateLabel->setJustificationType (Justification::centredBottom);
    lfoRateLabel->setEditable (false, false, false);
    lfoRateLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoRateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
	lfoRateSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.lfoRateParam, knobStyle(ROTARY)));
	addAndMakeVisible(lfoRateSlider.get());  //
    
    lfoModeLabel = std::unique_ptr<Label> ( new Label ("lfoModeLabel",TRANS("Shape")));
    addAndMakeVisible (lfoModeLabel.get());          //
    lfoModeLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoModeLabel->setJustificationType (Justification::centredBottom);
    lfoModeLabel->setEditable (false, false, false);
    lfoModeLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoModeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	lfoModeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.lfoModeParam, LINEARHORIZONTAL));
	addAndMakeVisible(lfoModeSlider.get());  //
	
    lfoIntensityLabel = std::unique_ptr<Label> ( new Label ("lfoIntensityLabel", TRANS("Mod Amt")));
    addAndMakeVisible (lfoIntensityLabel.get());          //
    lfoIntensityLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    lfoIntensityLabel->setJustificationType (Justification::centredBottom);
    lfoIntensityLabel->setEditable (false, false, false);
    lfoIntensityLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoIntensityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
	lfoIntensitySlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.lfoIntensityParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoIntensitySlider.get());  //
    
	lfoSyncedFreqSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.lfoDivisionParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoSyncedFreqSlider.get());

	lfoSyncSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.lfoSyncParam, LINEARHORIZONTAL));
	addAndMakeVisible(lfoSyncSlider.get());
	
	
    
    
    //
    // Modulation Target
    //
	modTargetSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.modTargetParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(modTargetSlider.get());  //
  
    modTargetLabel = std::unique_ptr<Label> ( new Label ("modTargetLabel", TRANS("Target")));
    addAndMakeVisible (modTargetLabel.get());          //
    modTargetLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetLabel->setJustificationType (Justification::centredBottom);
    modTargetLabel->setEditable (false, false, false);
    modTargetLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    modTargetOffLabel = std::unique_ptr<Label> ( new Label ("modTargetOffLabel", TRANS("-off")));
    addAndMakeVisible (modTargetOffLabel.get());          //
    modTargetOffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetOffLabel->setJustificationType (Justification::centredLeft);
    modTargetOffLabel->setEditable (false, false, false);
    modTargetOffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetOffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    modTargetPitchLabel = std::unique_ptr<Label> (new Label ("modTargetPitchLabel", TRANS("-pitch")));
    addAndMakeVisible (modTargetPitchLabel.get());
    modTargetPitchLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetPitchLabel->setJustificationType (Justification::centredLeft);
    modTargetPitchLabel->setEditable (false, false, false);
    modTargetPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    modTargetCutoffLabel = std::unique_ptr<Label> ( new Label ("modTargetCutoffLabel", TRANS("-cutoff")));
    addAndMakeVisible (modTargetCutoffLabel.get());         
    modTargetCutoffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    modTargetCutoffLabel->setJustificationType (Justification::centredLeft);
    modTargetCutoffLabel->setEditable (false, false, false);
    modTargetCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
	
    
    startTimerHz (60);

}

LFOSection::~LFOSection()
{
}

void LFOSection::paint (Graphics& g)
{
    int marginY = 8;
    
    {
        int x = getWidth() - 1, y = 8, width = 1, height = getHeight() - marginY;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
}

void LFOSection::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int headerHeight = 48;
    
    lfoLabel->setBounds (area.removeFromTop(headerHeight));
    lfoLabel->setJustificationType(Justification::centred);
    
    
    int stripWith = 72;
    int labelHeight = 24;
    int rotaryHeight = 64;
    int rotaryTextHeight = 10;
    int vertSliderHeight = 24;
    int marginX = 8;
    int marginY = 4;
    
    {
        Rectangle<int> lfoArea (area.removeFromTop(600));
        lfoRateLabel->setBounds(lfoArea.removeFromTop(labelHeight));
        
        lfoRateSlider->setBounds(lfoArea.removeFromTop(rotaryHeight));
        lfoSyncedFreqSlider->setBounds(lfoRateSlider->getBounds()); //Same spot as normal rate slider
        
        lfoModeLabel->setBounds(lfoArea.removeFromTop(labelHeight));
        lfoModeSlider->setBounds(lfoArea.removeFromTop(rotaryHeight).reduced(marginX, 0));
        
        lfoIntensityLabel->setBounds(lfoArea.removeFromTop(labelHeight));
        lfoIntensitySlider->setBounds(lfoArea.removeFromTop(rotaryHeight));
        
        modTargetLabel->setBounds(lfoArea.removeFromTop(labelHeight));
        
        int selectSize = 48;
        Rectangle<int> selectArea (lfoArea.removeFromTop(selectSize));
        selectArea.translate(12,8);
        modTargetSlider->setBounds(selectArea.removeFromLeft(selectSize / 3));
        
        modTargetOffLabel->setBounds(selectArea.removeFromTop(selectSize / 3));
        modTargetOffLabel->setJustificationType(Justification::topLeft);
        
        modTargetCutoffLabel->setBounds(selectArea.removeFromTop(selectSize/ 3));
        modTargetCutoffLabel->setJustificationType(Justification::centredLeft);
        
        modTargetPitchLabel->setBounds(selectArea.removeFromTop(selectSize/ 3));
        modTargetPitchLabel->setJustificationType(Justification::bottomLeft);
        
        lfoSyncSlider->setBounds(lfoArea.removeFromTop(rotaryHeight).reduced(marginX, 0));
        
        
    }

}

void LFOSection::timerCallback()
{
    if (processor.lfoSynced()) // free rate LFO
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
}
