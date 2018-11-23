/*
  ==============================================================================

    OscillatorSection.h
    Created: 16 Feb 2018 5:40:26pm
    Author:  Sander Vermeer

  ==============================================================================
*/



#ifndef OSCILLATOR_SECTION_H
#define OSCILLATOR_SECTION_H


#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSlider.h"
#include "MonosynthLookAndFeel.h"

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
    
   // MonosynthLookAndFeel customLook;

    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    Colour lineColour;

    
    std::unique_ptr<Label>
        oscillatorsLabel,
    
        oscGainLabel[3],
        oscWaveformLabel[3],
        oscOffsetLabel[3],
        oscTuneLabel[3],
    
    
        oscSyncLabel,
        oscSyncONLabel,
        oscSyncOFFLabel,
        pitchModLabel,
        glideLabel
    
    
        ;
        
    std::unique_ptr<ParameterSlider>
        oscGainSlider[3],
        oscOffsetSlider[3],
        oscTuneSlider[3],
        oscWaveformSlider[3],

        pitchModSlider,
        pulsewidthSlider[3],
        pulsewidthAmountSlider[3],
    
        oscSyncSlider,
        glideTimeSlider

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

#endif // OSCILLATOR_SECTION_H
