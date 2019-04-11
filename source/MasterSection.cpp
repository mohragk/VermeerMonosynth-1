/*
  ==============================================================================

    MasterSection.cpp
    Created: 17 Feb 2018 3:41:58pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterSection.h"

//==============================================================================
MasterSection::MasterSection(MonosynthPluginAudioProcessor&p) :
	processor(p),
	mainLabel(nullptr),
	volumeLabel(nullptr),
	saturationLabel(nullptr),
	volumeSlider(nullptr),
	softClipSwitchSlider(nullptr),
	filterOrderSlider(nullptr),
	saturationSlider(nullptr),
	saturationSwitchSlider(nullptr),
	arpeggiatorSpeedSlider(nullptr),
	arpeggioSwitchSlider(nullptr),
	skipChorusSlider(nullptr)


{
 
	float kerning = 0.05;
	String font = Font::getDefaultSansSerifFontName();
    
    typedef ParameterSlider::style knobStyle;

    mainLabel.reset ( new Label("Main Label", TRANS("Main")));
    addAndMakeVisible (mainLabel.get());
    mainLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (kerning));
    mainLabel->setJustificationType (Justification::centredTop);
    mainLabel->setEditable (false, false, false);
    mainLabel->setColour (TextEditor::textColourId, Colours::black);
    mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    volumeSlider.reset(new ParameterSlider(*processor.gainParam, knobStyle(ROTARY)));
    addAndMakeVisible (volumeSlider.get());  //
    
    volumeLabel.reset ( new Label ("Main Volume Label", TRANS("Volume")) );
    addAndMakeVisible (volumeLabel.get());  //
	MonosynthLookAndFeel::setLabelStyle(volumeLabel.get());
    
    //Softclip switch
    softClipSwitchSlider.reset(new ParameterSlider(*processor.softClipSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(softClipSwitchSlider.get());
    softClipSwitchSlider.get()->setTooltip("Soft Clip ON/OFF");
    
    
    //Saturation
    saturationSlider.reset(new ParameterSlider(*processor.saturationParam, knobStyle(ROTARY)));
    addAndMakeVisible(saturationSlider.get());  //
    
    
    saturationSwitchSlider.reset(new ParameterSlider(*processor.waveshapeSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(saturationSwitchSlider.get());
    saturationSwitchSlider.get()->setTooltip("Saturation ON/OFF");
    
    
    saturationModeSlider.reset(new ParameterSlider(*processor.waveshapeModeParam, knobStyle(LINEARHORIZONTAL)));
    addAndMakeVisible(saturationModeSlider.get());
    
    saturationLabel.reset ( new Label ("saturationLabel", TRANS("Saturation")));
    addAndMakeVisible (saturationLabel.get());        //
	MonosynthLookAndFeel::setLabelStyle(saturationLabel.get());
    

	//ARPEGGIATOR
	arpeggiatorSpeedSlider.reset(new ParameterSlider(*processor.arpeggioNoteLengthParam, knobStyle(ROTARY)));
	addAndMakeVisible(arpeggiatorSpeedSlider.get());
	arpeggiatorSpeedSlider->setTooltip("Arpeggiator Speed");

	arpeggioSwitchSlider.reset(new ParameterSlider(*processor.arpeggioUseParam, knobStyle(LINEARVERTICAL)));
	addAndMakeVisible(arpeggioSwitchSlider.get());
	arpeggioSwitchSlider->setTooltip("Arpeggio ON/OFF");
    
    // Level Meter
    meter_ptr = &processor.meter;
    addAndMakeVisible(meter_ptr);
    

	//CHORUS
	skipChorusSlider.reset(new ParameterSlider(*processor.skipChorusParam, knobStyle(ROTARY)));
	addAndMakeVisible(skipChorusSlider.get());
    
    startTimerHz(60);

}

MasterSection::~MasterSection()
{
}

void MasterSection::paint (Graphics& g)
{
   // g.fillAll(Colours::crimson);
}

void MasterSection::timerCallback()
{
    if (processor.saturationOn())
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

void MasterSection::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int headerHeight = 48;
    
    //Rectangle<int> headerArea (area.removeFromTop (headerHeight));
    mainLabel->setBounds (area.removeFromTop(headerHeight));
    mainLabel->setJustificationType(Justification::centred);
    
    
    int stripWidth = area.getWidth();
    int labelHeight = 24;
    int rotaryHeight = 60;
    
    {
        
        Rectangle<int> strip (area.removeFromLeft((stripWidth)));
        
        volumeLabel->setBounds (strip.removeFromTop(labelHeight));
        volumeLabel->setJustificationType(Justification::centredBottom);
        volumeSlider->setBounds (strip.removeFromTop(rotaryHeight));
        
        // LEVEL METER
        
       
        
        
        Rectangle<int> mini (volumeSlider->getBounds());
        softClipSwitchSlider->setBounds(mini.removeFromRight(12).reduced(0, 8));
        
        Rectangle<int> meterbounds (strip.removeFromTop(labelHeight));
        meter_ptr->setBounds(meterbounds.reduced(18, 0));
        meter_ptr->setLowLevelColour(Colours::white);
        meter_ptr->setBackgroundColour(Colour(0xff0e0e0e));
        
        saturationLabel->setBounds (strip.removeFromTop(labelHeight));
        saturationLabel->setJustificationType(Justification::centredBottom);
        saturationSlider->setBounds (strip.removeFromTop(rotaryHeight));
        
        Rectangle<int> mini2 (saturationSlider->getBounds());
        saturationSwitchSlider->setBounds(mini2.removeFromRight(12).reduced(0, 8));
        
      
        saturationModeSlider->setBounds (strip.removeFromTop(rotaryHeight).reduced(8, 0));

		arpeggiatorSpeedSlider->setBounds(strip.removeFromTop(rotaryHeight));
		Rectangle<int> mini3(arpeggiatorSpeedSlider->getBounds());
		arpeggioSwitchSlider->setBounds(mini3.removeFromRight(12).reduced(0, 8));

		skipChorusSlider->setBounds(strip.removeFromTop(rotaryHeight));

        
		
               
    }
    
    

}
