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
                                                                saturationSwitchSlider(nullptr)


{
    font = "Futura";
    
    
    typedef ParameterSlider::style knobStyle;

    mainLabel = std::unique_ptr<Label> ( new Label("Main Label", TRANS("Main")));
    addAndMakeVisible (mainLabel.get());
    mainLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    mainLabel->setJustificationType (Justification::centredTop);
    mainLabel->setEditable (false, false, false);
    mainLabel->setColour (TextEditor::textColourId, Colours::black);
    mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    volumeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.gainParam, knobStyle(ROTARY)));
    addAndMakeVisible (volumeSlider.get());  //
    
    volumeLabel = std::unique_ptr<Label> ( new Label ("Main Volume Label", TRANS("Volume")) );
    addAndMakeVisible (volumeLabel.get());  //
    volumeLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    volumeLabel->setJustificationType (Justification::centredBottom);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    //Softclip switch
    softClipSwitchSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.softClipSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(softClipSwitchSlider.get());
    
    // Filter Order; switch from VCA->filter to filter->VCA
    filterOrderSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.filterOrderParam, LINEARHORIZONTAL));
    addAndMakeVisible(filterOrderSlider.get());  //
    
    //Saturation
    saturationSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.saturationParam, knobStyle(ROTARY)));
    addAndMakeVisible(saturationSlider.get());  //
    
    
    saturationSwitchSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.waveshapeSwitchParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(saturationSwitchSlider.get());
    
    
    saturationModeSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.waveshapeModeParam, knobStyle(LINEARHORIZONTAL)));
    addAndMakeVisible(saturationModeSlider.get());
    
    saturationLabel = std::unique_ptr<Label> ( new Label ("saturationLabel", TRANS("Saturation")));
    addAndMakeVisible (saturationLabel.get());        //
    saturationLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    saturationLabel->setJustificationType (Justification::centredBottom);
    saturationLabel->setEditable (false, false, false);
    saturationLabel->setColour (TextEditor::textColourId, Colours::black);
    saturationLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

}

MasterSection::~MasterSection()
{
}

void MasterSection::paint (Graphics& g)
{
    
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
    
    
    int stripWidth = 72;
    int labelHeight = 24;
    int rotaryHeight = 64;
    
    {
        
        Rectangle<int> strip (area.removeFromLeft((stripWidth)));
        
        volumeLabel->setBounds (strip.removeFromTop(labelHeight));
        volumeLabel->setJustificationType(Justification::centredBottom);
        volumeSlider->setBounds (strip.removeFromTop(rotaryHeight));
        
        softClipSwitchSlider->setBounds(getWidth() - 24, 110, 12, 40); //TODO
        
        saturationLabel->setBounds (strip.removeFromTop(labelHeight));
        saturationLabel->setJustificationType(Justification::centredBottom);
        saturationSlider->setBounds (strip.removeFromTop(rotaryHeight));
        
        saturationSwitchSlider->setBounds(getWidth() - 24, 190, 12,40); //TODO
        
      
        saturationModeSlider->setBounds (strip.removeFromTop(rotaryHeight));
        
        filterOrderSlider->setBounds (strip.removeFromTop(rotaryHeight)); //TODO
       
    }
    
    

}
