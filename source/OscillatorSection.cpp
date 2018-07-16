/*
  ==============================================================================

    OscillatorSection.cpp
    Created: 16 Feb 2018 5:40:26pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorSection.h"

//==============================================================================
OscillatorSection::OscillatorSection(MonosynthPluginAudioProcessor& p) :
    processor(p),

	oscillatorsLabel(nullptr),


	oscSyncLabel(nullptr),
	oscSyncONLabel(nullptr),
	oscSyncOFFLabel(nullptr)
{
    font = "Futura";
    

	typedef ParameterSlider::style knobStyle;
	//
    // OSCILLATORS
    //
    
    oscillatorsLabel = std::unique_ptr<Label>  (new Label ("Oscillators",
                                                           TRANS("Oscillators")));
    addAndMakeVisible (oscillatorsLabel.get());        //
    oscillatorsLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    oscillatorsLabel->setJustificationType (Justification::centred);
    oscillatorsLabel->setEditable (false, false, false);
    oscillatorsLabel->setColour (TextEditor::textColourId, Colours::black);
    oscillatorsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    // OSC 1
    for (int osc = 0; osc < 3; osc++)
    {
        oscGainSlider[osc] = std::unique_ptr<ParameterSlider> ( new ParameterSlider(*processor.oscGainParam[osc], knobStyle(ROTARY)) );
        addAndMakeVisible (oscGainSlider[osc].get()); //
        
        oscGainLabel[osc] = std::unique_ptr<Label> (new Label("OSC"+std::to_string(osc)+" Gain Label", TRANS("Gain")));
        addAndMakeVisible(oscGainLabel[osc].get());
        setLabelStyle(*oscGainLabel[osc].get(), font);
        
        oscWaveformSlider[osc] = std::unique_ptr<ParameterSlider> ( new ParameterSlider(*processor.oscModeParam[osc], knobStyle(LINEARHORIZONTAL)) );
        addAndMakeVisible (oscWaveformSlider[osc].get()); //no label but graphics
        
        oscWaveformLabel[osc] = std::unique_ptr<Label>(new Label ("OSC"+std::to_string(osc)+" Waveform Label", TRANS("Shape")));
        addAndMakeVisible (oscWaveformLabel[osc].get());
        setLabelStyle(*oscWaveformLabel[osc].get(), font);
        
        oscTuneSlider[osc] = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.oscDetuneAmountParam[osc], knobStyle(ROTARY)));
        addAndMakeVisible(oscTuneSlider[osc].get()); //
        oscTuneSlider[osc]->setDoubleClickReturnValue(true, 0.5);
        
        oscTuneLabel[osc] = std::unique_ptr<Label> (new Label ("OSC"+std::to_string(osc)+" Tune Label", TRANS("Tune")));
        addAndMakeVisible (oscTuneLabel[osc].get());
        setLabelStyle(*oscTuneLabel[osc].get(), font);
        
        
        oscOffsetSlider[osc] = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.oscOffsetParam[osc], knobStyle(ROTARY)));
        addAndMakeVisible (oscOffsetSlider[osc].get()); //
        oscOffsetSlider[osc]->setTextBoxStyle (Slider::TextBoxBelow, true, 60, 10);
        oscOffsetSlider[osc]->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
        oscOffsetSlider[osc]->setDoubleClickReturnValue(true, 0.5);
        
        
        oscOffsetLabel[osc] = std::unique_ptr<Label> (new Label ("oscOffsetLabel"+std::to_string(osc), TRANS("Offset")));          //
        addAndMakeVisible (oscOffsetLabel[osc].get());
        setLabelStyle(*oscOffsetLabel[osc].get(), font);
        
        
        // PWM
        pulsewidthSlider[osc] = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.pulsewidthParam[osc], LINEARHORIZONTAL));
        addAndMakeVisible(pulsewidthSlider[osc].get());
        pulsewidthSlider[osc]->setDoubleClickReturnValue(true, 0.5);
        pulsewidthSlider[osc].get()->setTooltip("Adjust Pulsewidth: 0.5 -> 1.0");
        
        pulsewidthAmountSlider[osc] = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.pulsewidthAmountParam[osc], LINEARHORIZONTAL));
        addAndMakeVisible(pulsewidthAmountSlider[osc].get());  //
        pulsewidthAmountSlider[osc].get()->setTooltip("Set pulsewidth modulation amount from LFO");
        
        
        
    }
    
   
    //GLIDE
    
    glideLabel = std::unique_ptr<Label> (new Label("glideTimeLabel", TRANS("Glide")));
    addAndMakeVisible(glideLabel.get());
    setLabelStyle(*glideLabel.get(), font);
    
    
    glideTimeSlider = std::unique_ptr<ParameterSlider> ( new ParameterSlider(*processor.glideTimeParam, knobStyle(ROTARY) ) );
    addAndMakeVisible(glideTimeSlider.get());
    glideTimeSlider->setTextBoxStyle (Slider::TextBoxBelow, true, 60, 10);
    glideTimeSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    glideTimeSlider->setDoubleClickReturnValue(true, 0.5);
    glideTimeSlider->setTooltip("Glide Time (ms)");
    
	oscSyncSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.oscSyncParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (oscSyncSlider.get());
    oscSyncSlider->setColour (Slider::textBoxOutlineColourId, Colour (0x008e989b));
    //oscSyncSlider->setDoubleClickReturnValue(true, 0.5);
   
    oscSyncLabel = std::unique_ptr<Label> (new Label ("oscSyncLabel", TRANS("Sync")));                 //
    addAndMakeVisible (oscSyncLabel.get());
    setLabelStyle(*oscSyncLabel.get(), font);
    
    
    oscSyncONLabel = std::unique_ptr<Label> (new Label ("oscSyncONLabel",TRANS("-on")));
    addAndMakeVisible (oscSyncONLabel.get());
    setLabelStyle(*oscSyncONLabel.get(), font);
    
    oscSyncOFFLabel = std::unique_ptr<Label> (new Label ("oscSyncOFFLabel", TRANS("-off")));
    addAndMakeVisible (oscSyncOFFLabel.get());
    setLabelStyle(*oscSyncOFFLabel.get(), font);
    
    
    
    //Pitch MODULATION SLIDER
	pitchModSlider = std::unique_ptr<ParameterSlider>(new ParameterSlider(*processor.pitchModParam, knobStyle(ROTARY)));
    addAndMakeVisible (pitchModSlider.get());        //
    pitchModSlider->setDoubleClickReturnValue(true, 0.0);
    
    pitchModLabel = std::unique_ptr<Label> (new Label ("OSC2 Gain Label", TRANS("Pitch Mod")));                     //
    addAndMakeVisible (pitchModLabel.get());
    setLabelStyle(*pitchModLabel.get(), font);
    
    
    
    
    //
    // Drawables for symbols/icons
    //
    
    drawable25 = std::unique_ptr<Drawable>( Drawable::createFromImageData (oscNoiseWaveSymbol_svg, oscNoiseWaveSymbol_svgSize));
    drawable1 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size));
    drawable2 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize));
    drawable3 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize));
    
    drawable26 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscNoiseWaveSymbol_svg, oscNoiseWaveSymbol_svgSize));
    drawable4 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size));
    drawable5 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize));
    drawable6 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize));
    
    drawable27 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscNoiseWaveSymbol_svg, oscNoiseWaveSymbol_svgSize));
    drawable7 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSquareWaveSymbol_svg2, oscSquareWaveSymbol_svg2Size));
    drawable8 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSawWaveSymbol_svg, oscSawWaveSymbol_svgSize));
    drawable9 = std::unique_ptr<Drawable>(Drawable::createFromImageData (oscSineWaveSymbol_svg, oscSineWaveSymbol_svgSize));

    
    
   
    
    
    // set our component's initial size
    setSize (305, 620);
    
    
    
}

OscillatorSection::~OscillatorSection()
{
}

void OscillatorSection::setLabelStyle(Label& label, String f)
{
    label.setFont (Font (f, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    label.setJustificationType (Justification::centred);
    label.setEditable (false, false, false);
    label.setColour (TextEditor::textColourId, Colours::black);
    label.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

void OscillatorSection::paint (Graphics& g)
{
    //g.fillAll (Colours::seagreen);
    
    
    int marginY = 8;
    
    
    {
        int x = getWidth() - 1, y = marginY, width = 1, height = getHeight() - marginY;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
    
    //OSC1 Shape Symbols
    {
        Rectangle<int> shapeArea (oscWaveformSlider[0].get()->getBounds());
        
        Rectangle<int> shapeStrip(shapeArea.removeFromBottom(12).reduced(8,0));
        
        int shapeWidth = 16;
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable3 != 0);
            if (drawable3 != 0)
                drawable3->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable2 != 0);
            if (drawable2 != 0)
                drawable2->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable1 != 0);
            if (drawable1 != 0)
                drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable25 != 0);
            if (drawable25 != 0)
                drawable25->drawWithin (g, Rectangle<float> (x, y, width, height),
                                        RectanglePlacement::centred, 1.000f);
        }
    
    
    }
   
    
    // OSC2 Shape Symbols
    {
        Rectangle<int> shapeArea (oscWaveformSlider[1].get()->getBounds());
        
        Rectangle<int> shapeStrip(shapeArea.removeFromBottom(12).reduced(8,0));
        
        int shapeWidth = 16;
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable6 != 0);
            if (drawable6 != 0)
                drawable6->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable5 != 0);
            if (drawable5 != 0)
                drawable5->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable4 != 0);
            if (drawable4 != 0)
                drawable4->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable26 != 0);
            if (drawable26 != 0)
                drawable26->drawWithin (g, Rectangle<float> (x, y, width, height),
                                        RectanglePlacement::centred, 1.000f);
        }
    }
    
    
    
    // OSC3 Shape Symbols
    {
        Rectangle<int> shapeArea (oscWaveformSlider[2].get()->getBounds());
        
        Rectangle<int> shapeStrip(shapeArea.removeFromBottom(12).reduced(8,0));
        
        int shapeWidth = 16;
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable9 != 0);
            if (drawable9 != 0)
                drawable9->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable8 != 0);
            if (drawable8 != 0)
                drawable8->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
        
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable7 != 0);
            if (drawable7 != 0)
                drawable7->drawWithin (g, Rectangle<float> (x, y, width, height),
                                       RectanglePlacement::centred, 1.000f);
        }
    
        {
            Rectangle<int> mini (shapeStrip.removeFromLeft(shapeWidth));
            
            int x = mini.getX(), y = mini.getY(), width = 10, height = 6;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (Colours::black);
            jassert (drawable27 != 0);
            if (drawable27 != 0)
                drawable27->drawWithin (g, Rectangle<float> (x, y, width, height),
                                        RectanglePlacement::centred, 1.000f);
        }   
    }

}

void OscillatorSection::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int headerHeight = 48;
    
    //Rectangle<int> headerArea (area.removeFromTop (headerHeight));
    oscillatorsLabel->setBounds (area.removeFromTop(headerHeight));
    oscillatorsLabel->setJustificationType(Justification::centred);
    
    
    int stripWidth = 72;
    int labelHeight = 24;
    int rotaryHeight = 60;
    int rotaryTextHeight = 10;
    int vertSliderHeight = 24;
    int marginX = 8;
    int marginY = 4;
    
    for(int osc = 0; osc < 3; osc++)
    {
        
        Rectangle<int> oscArea (area.removeFromLeft((stripWidth)));
        
        oscGainLabel[osc]->setBounds (oscArea.removeFromTop(labelHeight));
        oscGainLabel[osc]->setJustificationType(Justification::centredBottom);
        oscGainSlider[osc]->setBounds (oscArea.removeFromTop(rotaryHeight));
        
        
        oscWaveformLabel[osc]->setBounds (oscArea.removeFromTop(labelHeight));
        oscWaveformLabel[osc]->setJustificationType(Justification::centredBottom);
        oscWaveformSlider[osc]->setBounds (oscArea.removeFromTop(rotaryHeight));
        
        oscTuneLabel[osc]->setBounds (oscArea.removeFromTop(labelHeight));
        oscTuneLabel[osc]->setJustificationType(Justification::centredBottom);
        oscTuneSlider[osc]->setBounds (oscArea.removeFromTop(rotaryHeight));
        
        oscOffsetLabel[osc]->setBounds (oscArea.removeFromTop(labelHeight));
        oscOffsetLabel[osc]->setJustificationType(Justification::centredBottom);
        oscOffsetSlider[osc]->setBounds (oscArea.removeFromTop(rotaryHeight + rotaryTextHeight));
        
        
        
        
        pulsewidthSlider[osc]->setBounds(oscArea.removeFromTop(vertSliderHeight).reduced(marginX, marginY));
        pulsewidthAmountSlider[osc]->setBounds(oscArea.removeFromTop(vertSliderHeight).reduced(marginX, marginY));
    }
    
    
    {
        Rectangle<int> oscArea (area.removeFromLeft((stripWidth)));
        
        pitchModLabel->setBounds (oscArea.removeFromTop(labelHeight));
        pitchModLabel->setJustificationType(Justification::centredBottom);
        pitchModSlider->setBounds (oscArea.removeFromTop(rotaryHeight));
        
        oscSyncLabel->setBounds(oscArea.removeFromTop(labelHeight));
        oscSyncLabel->setJustificationType(Justification::centredBottom);
        
        int syncHeight = 36;
        Rectangle<int> syncArea (oscArea.removeFromTop(syncHeight));
        syncArea.translate(12,0);
        oscSyncSlider->setBounds (syncArea.removeFromLeft(14));
    
        oscSyncSlider->setAlwaysOnTop(true);
        oscSyncONLabel->setBounds(syncArea.removeFromTop(syncHeight / 2));
        oscSyncONLabel->setJustificationType(Justification::topLeft);
        oscSyncOFFLabel->setBounds(syncArea.removeFromTop(syncHeight / 2));
        oscSyncOFFLabel->setJustificationType(Justification::bottomLeft);
    
    
        glideLabel->setBounds (oscArea.removeFromTop(labelHeight * 2));
        glideLabel->setJustificationType(Justification::centredBottom);
        glideTimeSlider.get()->setBounds(oscArea.removeFromTop(rotaryHeight + rotaryTextHeight));
        
        
    }
}


// JUCER_RESOURCE: oscNoiseWaveSymbol_svg, 650, "../Resources/OscNoiseWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscNoiseWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,102,45,
    56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,116,32,
    80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,49,46,
    49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,108,
    105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,101,119,
    66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,120,109,
    108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,108,108,
    58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,51,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,68,95,
    52,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,49,50,44,49,56,32,49,49,44,49,51,32,49,48,44,49,50,32,56,44,49,55,32,54,46,52,44,49,50,46,55,32,53,46,53,44,49,52,32,
    51,44,49,52,32,51,44,49,50,32,52,46,53,44,49,50,32,55,46,54,44,55,46,51,32,56,44,49,50,32,57,46,55,44,54,46,52,32,13,10,9,9,49,50,46,55,44,49,49,46,55,32,49,51,46,50,44,49,51,46,50,32,49,54,44,56,32,49,
    55,46,52,44,49,51,46,54,32,49,56,46,53,44,49,50,32,50,49,44,49,50,32,50,49,44,49,52,32,49,57,44,49,52,32,49,54,46,54,44,49,56,46,52,32,49,53,46,50,44,49,51,46,52,32,9,34,47,62,13,10,60,47,103,62,13,10,
    60,47,115,118,103,62,13,10,0,0};

const char* OscillatorSection::oscNoiseWaveSymbol_svg = (const char*) resource_CustomPluginEditor_oscNoiseWaveSymbol_svg;
const int OscillatorSection::oscNoiseWaveSymbol_svgSize = 650;

// JUCER_RESOURCE: oscSquareWaveSymbol_svg, 599, "../../../../../../Pictures/OscSquareWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSquareWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,
    102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,
    116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,
    49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,
    120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,
    101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,
    120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,
    108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,50,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
    68,95,49,56,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,46,52,44,49,55,46,50,32,49,52,46,55,44,49,55,46,50,32,49,52,46,55,44,57,46,49,32,56,46,55,44,57,46,49,
    32,56,46,55,44,49,55,46,50,32,51,44,49,55,46,50,32,51,44,49,53,46,50,32,54,46,55,44,49,53,46,50,32,54,46,55,44,55,46,49,32,13,10,9,9,49,54,46,55,44,55,46,49,32,49,54,46,55,44,49,53,46,50,32,50,48,46,52,
    44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* OscillatorSection::oscSquareWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg;
const int OscillatorSection::oscSquareWaveSymbol_svgSize = 599;

// JUCER_RESOURCE: oscSquareWaveSymbol_svg2, 599, "../Resources/OscSquareWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSquareWaveSymbol_svg2[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,
    102,45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,
    116,32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,
    49,46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,
    120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,
    101,119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,
    120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,
    108,108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,50,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
    68,95,49,56,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,46,52,44,49,55,46,50,32,49,52,46,55,44,49,55,46,50,32,49,52,46,55,44,57,46,49,32,56,46,55,44,57,46,49,
    32,56,46,55,44,49,55,46,50,32,51,44,49,55,46,50,32,51,44,49,53,46,50,32,54,46,55,44,49,53,46,50,32,54,46,55,44,55,46,49,32,13,10,9,9,49,54,46,55,44,55,46,49,32,49,54,46,55,44,49,53,46,50,32,50,48,46,52,
    44,49,53,46,50,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* OscillatorSection::oscSquareWaveSymbol_svg2 = (const char*)resource_CustomPluginEditor_oscSquareWaveSymbol_svg2;
const int OscillatorSection::oscSquareWaveSymbol_svg2Size = 599;

// JUCER_RESOURCE: oscSawWaveSymbol_svg, 559, "../Resources/OscSawWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSawWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,102,
    45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,116,
    32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,49,
    46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,
    108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,101,
    119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,120,
    109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,108,
    108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,49,54,95,34,62,13,10,9,60,112,111,108,121,103,111,110,32,105,100,61,34,88,77,76,73,
    68,95,49,57,95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,112,111,105,110,116,115,61,34,50,48,44,49,56,32,49,53,46,50,44,49,56,32,57,46,56,44,49,49,46,51,32,57,46,56,44,49,56,32,52,44,49,56,32,52,
    44,49,54,32,55,46,56,44,49,54,32,55,46,56,44,53,46,55,32,49,54,46,49,44,49,54,32,50,48,44,49,54,32,9,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,13,10,0,0 };

const char* OscillatorSection::oscSawWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSawWaveSymbol_svg;
const int OscillatorSection::oscSawWaveSymbol_svgSize = 559;

// JUCER_RESOURCE: oscSineWaveSymbol_svg, 711, "../Resources/OscSineWaveSymbol.svg"
static const unsigned char resource_CustomPluginEditor_oscSineWaveSymbol_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,117,116,102,
    45,56,34,63,62,13,10,60,33,45,45,32,71,101,110,101,114,97,116,111,114,58,32,65,100,111,98,101,32,73,108,108,117,115,116,114,97,116,111,114,32,49,57,46,48,46,48,44,32,83,86,71,32,69,120,112,111,114,116,
    32,80,108,117,103,45,73,110,32,46,32,83,86,71,32,86,101,114,115,105,111,110,58,32,54,46,48,48,32,66,117,105,108,100,32,48,41,32,32,45,45,62,13,10,60,115,118,103,32,118,101,114,115,105,111,110,61,34,49,
    46,49,34,32,105,100,61,34,76,97,97,103,95,49,34,32,120,109,108,110,115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,32,120,109,108,110,115,58,120,
    108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,32,120,61,34,48,112,120,34,32,121,61,34,48,112,120,34,13,10,9,32,118,105,101,
    119,66,111,120,61,34,48,32,48,32,50,52,32,50,52,34,32,115,116,121,108,101,61,34,101,110,97,98,108,101,45,98,97,99,107,103,114,111,117,110,100,58,110,101,119,32,48,32,48,32,50,52,32,50,52,59,34,32,120,
    109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,62,13,10,60,115,116,121,108,101,32,116,121,112,101,61,34,116,101,120,116,47,99,115,115,34,62,13,10,9,46,115,116,48,123,102,105,108,
    108,58,35,69,54,69,54,69,54,59,125,13,10,60,47,115,116,121,108,101,62,13,10,60,103,32,105,100,61,34,88,77,76,73,68,95,49,53,95,34,62,13,10,9,60,112,97,116,104,32,105,100,61,34,88,77,76,73,68,95,50,48,
    95,34,32,99,108,97,115,115,61,34,115,116,48,34,32,100,61,34,77,49,53,44,49,55,99,45,49,46,50,44,48,45,51,45,49,45,52,46,52,45,52,46,55,67,49,48,44,49,48,46,54,44,57,44,57,46,53,44,56,46,49,44,57,46,53,
    99,45,49,44,48,45,50,46,52,44,49,46,50,45,51,46,51,44,51,46,56,108,45,49,46,57,45,48,46,54,13,10,9,9,99,49,45,51,46,50,44,51,45,53,46,50,44,53,46,49,45,53,46,51,99,49,46,49,44,48,44,51,46,49,44,48,46,
    54,44,52,46,52,44,52,46,49,67,49,51,46,52,44,49,52,44,49,52,46,52,44,49,53,44,49,53,44,49,53,99,48,44,48,44,48,44,48,44,48,44,48,99,48,46,55,44,48,44,50,45,49,46,51,44,51,45,52,46,52,108,49,46,57,44,48,
    46,55,67,49,56,46,56,44,49,52,46,57,44,49,55,44,49,55,44,49,53,44,49,55,13,10,9,9,67,49,53,46,49,44,49,55,44,49,53,44,49,55,44,49,53,44,49,55,122,34,47,62,13,10,60,47,103,62,13,10,60,47,115,118,103,62,
    13,10,0,0 };

const char* OscillatorSection::oscSineWaveSymbol_svg = (const char*)resource_CustomPluginEditor_oscSineWaveSymbol_svg;
const int OscillatorSection::oscSineWaveSymbol_svgSize = 711;


