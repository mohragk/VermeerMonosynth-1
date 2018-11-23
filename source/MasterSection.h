/*
  ==============================================================================

    MasterSection.h
    Created: 17 Feb 2018 3:41:58pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef MASTER_SECTION_H
#define MASTER_SECTION_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSlider.h"
#include "MonosynthLookAndFeel.h"

//==============================================================================
/*
*/
class MasterSection    : public Component, private Timer
{
public:
    MasterSection(MonosynthPluginAudioProcessor& p);
    ~MasterSection();

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

    LevelMeter* meter_ptr;

private:
    MonosynthPluginAudioProcessor& processor;


    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};
	
	std::unique_ptr<Label>  
        mainLabel,
        volumeLabel,
        saturationLabel
   ;

   std::unique_ptr<ParameterSlider>
        volumeSlider,
        softClipSwitchSlider,

        filterOrderSlider, //@Rename
    
        saturationSlider,
        saturationSwitchSlider,
        saturationModeSlider,

	   arpeggiatorSpeedSlider,
	   arpeggioSwitchSlider
    
        

    ;
    
    Colour lineColour;
    
    String font;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterSection)
};

#endif // MASTER_SECTION_H
