/*
  ==============================================================================

    LFOSection.h
    Created: 17 Feb 2018 11:32:31am
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef LFO_SECTION_H
#define LFO_SECTION_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSlider.h"

//==============================================================================
/*
*/
class LFOSection    : public Component, private Timer
{
public:
    LFOSection(MonosynthPluginAudioProcessor& p);
    ~LFOSection();
    
    // Binary resources:
	static const char* oscSquareWaveSymbol_svg;
	static const int oscSquareWaveSymbol_svgSize;
	static const char* oscSquareWaveSymbol_svg2;
	static const int oscSquareWaveSymbol_svg2Size;
	static const char* oscSawWaveSymbol_svg;
	static const int oscSawWaveSymbol_svgSize;
	static const char* oscSineWaveSymbol_svg;
	static const int oscSineWaveSymbol_svgSize;
	static const char* oscNoiseWaveSymbol_svg;
    static const int oscNoiseWaveSymbol_svgSize;

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;


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
        lfoLabel,
        lfoRateLabel,
        lfoModeLabel,
        lfoIntensityLabel,
        modTargetLabel,
        modTargetCutoffLabel,
        modTargetPitchLabel,
        modTargetOffLabel
    ;
    
    std::unique_ptr<ParameterSlider>
        modTargetSlider,
        lfoRateSlider,
        lfoModeSlider,
        lfoIntensitySlider,
		lfoSyncSlider,
        lfoSyncedFreqSlider
    ;
    
    
    String font;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOSection)
};

#endif // LFO_SECTION_H
