/*
  ==============================================================================

    FilterSection.h
    Created: 16 Feb 2018 10:50:16pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef FILTER_SECTION_H
#define FILTER_SECTION_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSlider.h"
#include "OscillatorSection.h"

//==============================================================================
/*
*/
class FilterSection    : public Component
{
public:
    FilterSection(MonosynthPluginAudioProcessor& p);
    ~FilterSection();

    void paint (Graphics&) override;
    void resized() override;
    
    

private:
    MonosynthPluginAudioProcessor& processor;

    
    

    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    Colour lineColour;
    
    std::unique_ptr<Label>
        filterLabel,
        filterCutoffLabel,
        filterResonanceLabel,
        filterContourLabel,
        filterDriveLabel,
    
        filterMoogLabel,
        filterMS20Label,
        filter303Label
    ;
    
    std::unique_ptr<ParameterSlider>
        filterCutoffSlider,
		filterKeyFollowSlider,
        filterResonanceSlider,
        filterContourSlider,
        filterDriveSlider,
        filterSelectSlider
    ;
    
    String font;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSection)
};

#endif // FILTER_SECTION_H
