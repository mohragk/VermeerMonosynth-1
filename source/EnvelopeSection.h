/*
  ==============================================================================

    EnvelopeSection.h
    Created: 17 Feb 2018 12:03:01am
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef ENVELOPE_SECTION_H
#define ENVELOPE_SECTION_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSlider.h"
#include "ParamToggleButton.h"
#include "LEDComponent.h"
#include "MonosynthLookAndFeel.h"

//==============================================================================
/*
*/
class EnvelopeSection    : public Component
{
public:
    EnvelopeSection(MonosynthPluginAudioProcessor& p);
    ~EnvelopeSection();
    
    // Binary resources:
	
	static const char* attackCurveLinear_symbol_svg;
	static const int attackCurveLinear_symbol_svgSize;
	static const char* attackCurveExponential_symbol_svg;
	static const int attackCurveExponential_symbol_svgSize;
	static const char* decayCurveLinear_symbol_svg;
	static const int decayCurveLinear_symbol_svgSize;
	static const char* decayCurveExponential_symbol_svg;
	static const int decayCurveExponential_symbol_svgSize;
	
	std::unique_ptr<Label>
	    envelopesLabel,
        envAmpLabel,
        envFilterLabel,
        envPitchLabel,
        retriggerLabel
    ;
    
    std::unique_ptr<ParameterSlider>
        attackSlider1,
        decaySlider1,
        sustainSlider1,
        releaseSlider1,
        attackCurveSlider1,
        decRelCurveSlider1,


        attackSlider2,
        decaySlider2,
        sustainSlider2,
        releaseSlider2,
        attackCurveSlider2,
        decRelCurveSlider2,

        attackSlider3,
        decaySlider3,
        sustainSlider3,
        releaseSlider3,
        attackCurveSlider3,
        decRelCurveSlider3
    ;
    
    std::unique_ptr<ParamToggleButton>
        retriggerButton
    ;

    void paint (Graphics&) override;
    void resized() override;

private:
   MonosynthPluginAudioProcessor& processor;

   std::unique_ptr<LED> led1, led2, led3;

    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    Colour lineColour;
    
    String font;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeSection)
};

#endif // LFO_SECTION_H
