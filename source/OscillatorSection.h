/*
  ==============================================================================

    OscillatorSection.h
    Created: 16 Feb 2018 5:40:26pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ParameterSlider.h"

//==============================================================================
/*
*/
class OscillatorSection    : public Component
{
public:
    OscillatorSection(MonosynthPluginAudioProcessor& p);
    ~OscillatorSection();

    void paint (Graphics&) override;
    void resized() override;
    
    
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

private:
    MonosynthPluginAudioProcessor& processor;


    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    Colour lineColour;

    
    ScopedPointer<Label>  
        osc1GainLabel,
        osc1WaveformLabel,
        osc1OffsetLabel,
        oscillatorsLabel,
        osc1TuneLabel,
        osc2GainLabel,
        osc2WaveformLabel,
        osc2OffsetLabel,
        osc2TuneLabel,
        osc3GainLabel,
        osc3WaveformLabel,
        osc3OffsetLabel,
        osc3TuneLabel,
    
        oscSyncLabel,
        oscSyncONLabel,
        oscSyncOFFLabel,
        pitchModLabel
    
    
        ;
        
    std::unique_ptr<ParameterSlider>
        osc1GainSlider,
        osc1OffsetSlider,
        osc1TuneSlider,
        osc1WaveformSlider,

        osc2GainSlider,
        osc2OffsetSlider,
        osc2TuneSlider,
        osc2WaveformSlider,

        osc3GainSlider,
        osc3OffsetSlider,
        osc3TuneSlider,
        osc3WaveformSlider,
        oscSyncSlider,

        pitchModSlider,
        pulsewidth1Slider,
        pulsewidth2Slider,
        pulsewidth3Slider,
        
        pulsewidthAmount1Slider,
        pulsewidthAmount2Slider,
        pulsewidthAmount3Slider
        ;
	   
	   
    std::unique_ptr<Drawable>
        drawable25,
        drawable1,
        drawable2,
        drawable3,

        drawable26,
        drawable4,
        drawable5,
        drawable6,

        drawable27,
        drawable7,
        drawable8,
        drawable9
        ;

    String font;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorSection)
};
