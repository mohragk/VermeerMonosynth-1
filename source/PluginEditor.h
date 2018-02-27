/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#ifndef PLUGIN_EDITOR_H
#define PLUGIN_EDITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OscillatorSection.h"
#include "FilterSection.h"
#include "EnvelopeSection.h"
#include "LFOSection.h"
#include "MasterSection.h"
#include "Sequencer.h"



//==============================================================================
/** This is the editor component that our filter will display.
*/
class MonosynthPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                            public Button::Listener,
										
                                            private Timer
                                            //private MidiKeyboardStateListener
{
public:
    MonosynthPluginAudioProcessorEditor (MonosynthPluginAudioProcessor&);
    ~MonosynthPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    

	// Binary resources:
	
	static const char* attackCurveLinear_symbol_svg;
	static const int attackCurveLinear_symbol_svgSize;
	static const char* attackCurveExponential_symbol_svg;
	static const int attackCurveExponential_symbol_svgSize;
	static const char* decayCurveLinear_symbol_svg;
	static const int decayCurveLinear_symbol_svgSize;
	static const char* decayCurveExponential_symbol_svg;
	static const int decayCurveExponential_symbol_svgSize;

    std::unique_ptr<OscillatorSection> oscillatorSection;
    std::unique_ptr<FilterSection> filterSection;
    std::unique_ptr<EnvelopeSection> envelopeSection;
    std::unique_ptr<LFOSection> lfoSection;
    std::unique_ptr<MasterSection> masterSection;
    std::unique_ptr<Sequencer> sequencerSection;
    
    
private:
    
    class ParamToggleButton;
    
    enum style
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    Colour lineColour;
    
    MidiKeyboardComponent midiKeyboard;

	Label timecodeDisplayLabel;
	
    std::unique_ptr<Label>
        titleLabel
   ;
    
   std::unique_ptr<ParameterSlider>
        oversampleSwitchSlider
    ;

    
    std::unique_ptr<Drawable>
        drawable10,
        drawable11,
        drawable12,
        drawable13,
        drawable14,
        drawable15,
        drawable16,
        drawable17,
        drawable18,
        drawable19,
        drawable20,
        drawable21,
        drawable22,
        drawable23,
        drawable24
    ;
    
    std::unique_ptr<ParamToggleButton> hqOversamplingButton, expandSequencerButton;
    
    std::unique_ptr<ComponentAnimator> animator;
   
    
    String font;
    //==============================================================================
    MonosynthPluginAudioProcessor& getProcessor() const
    {
        return static_cast<MonosynthPluginAudioProcessor&> (processor);
    }

    void updateStates();
    void showSequencer(bool shouldShow);
    void updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo);
    
 
};

#endif // PLUGIN_EDITOR_H
