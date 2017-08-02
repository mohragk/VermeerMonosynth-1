/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CustomPluginEditor  : public Component,
                            public SliderListener
{
public:
    //==============================================================================
    CustomPluginEditor ();
    ~CustomPluginEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* oscSquareWaveSymbol_svg;
    static const int oscSquareWaveSymbol_svgSize;
    static const char* oscSquareWaveSymbol_svg2;
    static const int oscSquareWaveSymbol_svg2Size;
    static const char* oscSawWaveSymbol_svg;
    static const int oscSawWaveSymbol_svgSize;
    static const char* oscSineWaveSymbol_svg;
    static const int oscSineWaveSymbol_svgSize;
    static const char* attackCurveLinear_symbol_svg;
    static const int attackCurveLinear_symbol_svgSize;
    static const char* attackCurveExponential_symbol_svg;
    static const int attackCurveExponential_symbol_svgSize;
    static const char* decayCurveLinear_symbol_svg;
    static const int decayCurveLinear_symbol_svgSize;
    static const char* decayCurveExponential_symbol_svg;
    static const int decayCurveExponential_symbol_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> osc1GainSlider;
    ScopedPointer<Label> osc1GainLabel;
    ScopedPointer<Slider> osc1OffsetSlider;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> oscillatorsLabel;
    ScopedPointer<Slider> osc1TuneSlider;
    ScopedPointer<Label> osc1TuneLabel;
    ScopedPointer<Slider> osc1WaveformSlider;
    ScopedPointer<Slider> osc2GainSlider;
    ScopedPointer<Label> osc2GainLabel;
    ScopedPointer<Slider> osc2OffsetSlider;
    ScopedPointer<Label> osc2OffsetLabel;
    ScopedPointer<Slider> osc2TuneSlider;
    ScopedPointer<Label> osc2TuneLabel;
    ScopedPointer<Slider> osc2WaveformSlider;
    ScopedPointer<Slider> osc2GainSlider2;
    ScopedPointer<Label> osc2GainLabel2;
    ScopedPointer<Slider> osc3OffsetSlider;
    ScopedPointer<Label> osc3OffsetLabel;
    ScopedPointer<Slider> osc3TuneSlider;
    ScopedPointer<Label> osc2TuneLabel2;
    ScopedPointer<Slider> osc3WaveformSlider;
    ScopedPointer<Label> filterLabel;
    ScopedPointer<Slider> filterCutoffSider;
    ScopedPointer<Label> filterCutoffLabel;
    ScopedPointer<Slider> filterResonanceSlider;
    ScopedPointer<Label> filterResonanceLabel;
    ScopedPointer<Slider> filterContourSlider;
    ScopedPointer<Label> filterContourLabel;
    ScopedPointer<Slider> filterDriveSlider;
    ScopedPointer<Label> filterDriveLabel;
    ScopedPointer<Label> envelopesLabel;
    ScopedPointer<Slider> attackSlider1;
    ScopedPointer<Slider> attackSlider2;
    ScopedPointer<Label> envAmpLabel;
    ScopedPointer<Slider> decaySlider1;
    ScopedPointer<Slider> sustainSlider1;
    ScopedPointer<Slider> releaseSlider1;
    ScopedPointer<Label> envFilterLabel;
    ScopedPointer<Slider> decaySlider2;
    ScopedPointer<Slider> sustainSlider2;
    ScopedPointer<Slider> releaseSlider2;
    ScopedPointer<Slider> attackSlider3;
    ScopedPointer<Label> envPitchLabel;
    ScopedPointer<Slider> decaySlider3;
    ScopedPointer<Slider> sustainSlider3;
    ScopedPointer<Slider> releaseSlider3;
    ScopedPointer<Slider> volumeSlider;
    ScopedPointer<Label> volumeLabel;
    ScopedPointer<Label> mainLabel;
    ScopedPointer<Label> titleLabel;
    ScopedPointer<Slider> attackCurveSlider1;
    ScopedPointer<Slider> decRelCurveSlider1;
    ScopedPointer<Slider> pitchModSlider;
    ScopedPointer<Label> pitchModLabel;
    ScopedPointer<Label> osc1WaveformLabel;
    ScopedPointer<Label> osc2WaveformLabel;
    ScopedPointer<Label> osc3WaveformLabel;
    ScopedPointer<Slider> attackCurveSlider2;
    ScopedPointer<Slider> decRelCurveSlider2;
    ScopedPointer<Slider> attackCurveSlider3;
    ScopedPointer<Slider> decRelCurveSlider3;
    ScopedPointer<Drawable> drawable1;
    ScopedPointer<Drawable> drawable2;
    ScopedPointer<Drawable> drawable3;
    ScopedPointer<Drawable> drawable4;
    ScopedPointer<Drawable> drawable5;
    ScopedPointer<Drawable> drawable6;
    ScopedPointer<Drawable> drawable7;
    ScopedPointer<Drawable> drawable8;
    ScopedPointer<Drawable> drawable9;
    ScopedPointer<Drawable> drawable10;
    ScopedPointer<Drawable> drawable11;
    ScopedPointer<Drawable> drawable12;
    ScopedPointer<Drawable> drawable13;
    ScopedPointer<Drawable> drawable14;
    ScopedPointer<Drawable> drawable15;
    ScopedPointer<Drawable> drawable16;
    ScopedPointer<Drawable> drawable17;
    ScopedPointer<Drawable> drawable18;
    ScopedPointer<Drawable> drawable19;
    ScopedPointer<Drawable> drawable20;
    ScopedPointer<Drawable> drawable21;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomPluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
