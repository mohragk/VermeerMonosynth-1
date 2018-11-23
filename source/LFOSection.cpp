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
    font = Font::getDefaultSansSerifFontName();

	float kerning = 0.05f;
    
    typedef ParameterSlider::style knobStyle;

    
    lfoLabel.reset( new Label ("lfoLabel", TRANS("LFO")));
    addAndMakeVisible (lfoLabel.get());                        //
    lfoLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (kerning));
    lfoLabel->setJustificationType (Justification::centredTop);
    lfoLabel->setEditable (false, false, false);
    lfoLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    lfoRateLabel.reset ( new Label ("lfoRateLabel", TRANS("Rate")));
    addAndMakeVisible (lfoRateLabel.get());          //
	MonosynthLookAndFeel::setLabelStyle(lfoRateLabel.get());
    
	lfoRateSlider.reset(new ParameterSlider(*processor.lfoRateParam, knobStyle(ROTARY)));
	addAndMakeVisible(lfoRateSlider.get());  //
    
    lfoModeLabel.reset ( new Label ("lfoModeLabel",TRANS("Shape")));
    addAndMakeVisible (lfoModeLabel.get());          //
	MonosynthLookAndFeel::setLabelStyle(lfoModeLabel.get());

	lfoModeSlider.reset(new ParameterSlider(*processor.lfoModeParam, LINEARHORIZONTAL));
	addAndMakeVisible(lfoModeSlider.get());  //
	
    lfoIntensityLabel.reset ( new Label ("lfoIntensityLabel", TRANS("Mod Amt")));
    addAndMakeVisible (lfoIntensityLabel.get());          //
	MonosynthLookAndFeel::setLabelStyle(lfoIntensityLabel.get());

	lfoIntensitySlider.reset(new ParameterSlider(*processor.lfoIntensityParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoIntensitySlider.get());  //
    
	lfoSyncedFreqSlider.reset(new ParameterSlider(*processor.lfoDivisionParam, knobStyle(ROTARY)));
    addAndMakeVisible(lfoSyncedFreqSlider.get());

	lfoSyncSlider.reset(new ParameterSlider(*processor.lfoSyncParam, LINEARVERTICAL));
	addAndMakeVisible(lfoSyncSlider.get());
    lfoSyncSlider.get()->setTooltip("Tempo sync ON/OFF");
	
	
    
    
    //
    // Modulation Target
    //
	modTargetSlider.reset(new ParameterSlider(*processor.modTargetParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(modTargetSlider.get());  //
  
    modTargetLabel.reset ( new Label ("modTargetLabel", TRANS("Target")));
    addAndMakeVisible (modTargetLabel.get());          //
	MonosynthLookAndFeel::setLabelStyle(modTargetLabel.get());	

    modTargetOffLabel.reset ( new Label ("modTargetOffLabel", TRANS("-off")));
    addAndMakeVisible (modTargetOffLabel.get());          //
    modTargetOffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (kerning));
    modTargetOffLabel->setJustificationType (Justification::centredLeft);
    modTargetOffLabel->setEditable (false, false, false);
    modTargetOffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetOffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    modTargetPitchLabel.reset (new Label ("modTargetPitchLabel", TRANS("-pitch")));
    addAndMakeVisible (modTargetPitchLabel.get());
    modTargetPitchLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (kerning));
    modTargetPitchLabel->setJustificationType (Justification::centredLeft);
    modTargetPitchLabel->setEditable (false, false, false);
    modTargetPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    modTargetCutoffLabel.reset ( new Label ("modTargetCutoffLabel", TRANS("-cutoff")));
    addAndMakeVisible (modTargetCutoffLabel.get());         
    modTargetCutoffLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (kerning));
    modTargetCutoffLabel->setJustificationType (Justification::centredLeft);
    modTargetCutoffLabel->setEditable (false, false, false);
    modTargetCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    modTargetCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    //
    // Drawables for symbols/icons
    //
    
    drawable1.reset(Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size));
    drawable2.reset(Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize));
    drawable3.reset(Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize));
	
    
    startTimerHz (60);

}

LFOSection::~LFOSection()
{
}

void LFOSection::paint (Graphics& g)
{
    
  // g.fillAll(Colours::darkgreen);
    
    int marginY = 8;
    
    {
        int x = getWidth() - 1, y = 8, width = 1, height = getHeight() - marginY;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    
    //OSC1 Shape Symbols
    {
        Rectangle<int> shapeArea (lfoModeSlider.get()->getBounds());
        
        Rectangle<int> shapeStrip(shapeArea.removeFromBottom(12).reduced(8,0));
        
        int shapeWidth = 28;
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable3 != 0);
            if (drawable3 != 0)
                drawable3->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable2 != 0);
            if (drawable2 != 0)
                drawable2->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable1 != 0);
            if (drawable1 != 0)
                drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
    }
}

void LFOSection::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int headerHeight = 48;
    
    lfoLabel->setBounds (area.removeFromTop(headerHeight));
    lfoLabel->setJustificationType(Justification::centred);
    
    
    int labelHeight = 24;
    int rotaryHeight = 60;
    int marginX = 8;
    
    
    {
        Rectangle<int> strip (area.removeFromTop(600));
        lfoRateLabel->setBounds(strip.removeFromTop(labelHeight));
        
        lfoRateSlider->setBounds(strip.removeFromTop(rotaryHeight));
        lfoSyncedFreqSlider->setBounds(lfoRateSlider->getBounds()); //Same spot as normal rate slider
        
        Rectangle<int> mini2(lfoRateSlider->getBounds());
        lfoSyncSlider->setBounds(mini2.removeFromRight(24).reduced(6, 8));
        
        
        
        lfoModeLabel->setBounds(strip.removeFromTop(labelHeight));
		lfoModeLabel->setJustificationType(Justification::centredBottom);
        lfoModeSlider->setBounds(strip.removeFromTop(rotaryHeight).reduced(marginX, 0));
        
        lfoIntensityLabel->setBounds(strip.removeFromTop(labelHeight));
		lfoIntensityLabel->setJustificationType(Justification::centredBottom);
		//lfoIntensityLabel->setBounds(strip.removeFromTop(rotaryHeight).reduced(marginX, 0));
        lfoIntensitySlider->setBounds(strip.removeFromTop(rotaryHeight));
        
        modTargetLabel->setBounds(strip.removeFromTop(labelHeight));
		modTargetLabel->setJustificationType(Justification::centredBottom);
        
        int selectSize = 48;
        Rectangle<int> selectArea (strip.removeFromTop(selectSize));
        selectArea.translate(12,8);
        modTargetSlider->setBounds(selectArea.removeFromLeft(selectSize / 3));
        
        modTargetOffLabel->setBounds(selectArea.removeFromTop(selectSize / 3));
        modTargetOffLabel->setJustificationType(Justification::topLeft);
        
        modTargetCutoffLabel->setBounds(selectArea.removeFromTop(selectSize/ 3));
        modTargetCutoffLabel->setJustificationType(Justification::centredLeft);
        
        modTargetPitchLabel->setBounds(selectArea.removeFromTop(selectSize/ 3));
        modTargetPitchLabel->setJustificationType(Justification::bottomLeft);
        
        
        
        
        
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

const char* LFOSection::oscSquareWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg;
const int LFOSection::oscSquareWaveSymbol_svgSize = 599;

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

const char* LFOSection::oscSquareWaveSymbol_svg2 = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg2;
const int LFOSection::oscSquareWaveSymbol_svg2Size = 599;

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

const char* LFOSection::oscSawWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSawWaveSymbol_svg;
const int LFOSection::oscSawWaveSymbol_svgSize = 559;

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

const char* LFOSection::oscSineWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSineWaveSymbol_svg;
const int LFOSection::oscSineWaveSymbol_svgSize = 711;

