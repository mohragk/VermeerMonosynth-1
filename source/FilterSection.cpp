/*
  ==============================================================================

    FilterSection.cpp
    Created: 16 Feb 2018 10:50:16pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterSection.h"

//==============================================================================
FilterSection::FilterSection(MonosynthPluginAudioProcessor& p) :
                                                                processor(p),
                                                                filterLabel(nullptr),
                                                                filterCutoffLabel(nullptr),
                                                                filterResonanceLabel(nullptr),
                                                                filterContourLabel(nullptr),
                                                                filterDriveLabel(nullptr),

                                                                filterMoogLabel(nullptr),
                                                                filterMS20Label(nullptr),
                                                                filter303Label(nullptr)
{
    
    font = "Futura";
    
    
    typedef ParameterSlider::style knobStyle;
    
    
    
    filterLabel.reset (new Label ("Filter", TRANS("Filter")));
    addAndMakeVisible (filterLabel.get());                          //
    filterLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    filterLabel->setJustificationType (Justification::centredTop);
    filterLabel->setEditable (false, false, false);
    filterLabel->setColour (TextEditor::textColourId, Colours::black);
    filterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    filterCutoffSlider.reset(new ParameterSlider(*processor.filterCutoffParam, knobStyle(ROTARY)));
    addAndMakeVisible (filterCutoffSlider.get());   //
    
    filterCutoffLabel.reset(new Label ("Filter Cutoff Label", TRANS("Cutoff")));
    addAndMakeVisible (filterCutoffLabel.get());            //
    filterCutoffLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterCutoffLabel->setJustificationType (Justification::centredBottom);
    filterCutoffLabel->setEditable (false, false, false);
    filterCutoffLabel->setColour (TextEditor::textColourId, Colours::black);
    filterCutoffLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

	filterKeyFollowSlider.reset(new ParameterSlider(*processor.useFilterKeyFollowParam, knobStyle(LINEARVERTICAL)));
	addAndMakeVisible(filterKeyFollowSlider.get());
    filterKeyFollowSlider.get()->setTooltip("Key Follow ON/OFF");

    filterResonanceSlider.reset(new ParameterSlider(*processor.filterQParam, knobStyle(ROTARY)));
    addAndMakeVisible (filterResonanceSlider.get());   //
    
    
    filterResonanceLabel.reset( new Label ("Filter Resonance Label", TRANS("Resonance")));
    addAndMakeVisible (filterResonanceLabel.get());      //
    filterResonanceLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterResonanceLabel->setJustificationType (Justification::centredBottom);
    filterResonanceLabel->setEditable (false, false, false);
    filterResonanceLabel->setColour (TextEditor::textColourId, Colours::black);
    filterResonanceLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    filterContourSlider.reset(new ParameterSlider(*processor.filterContourParam, knobStyle(ROTARY)));
    addAndMakeVisible (filterContourSlider.get());  //
    
    filterSelectSlider.reset(new ParameterSlider(*processor.filterSelectParam, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(filterSelectSlider.get());  //todo:
    
    filterContourLabel.reset( new Label ("Filter Contour Label", TRANS("Contour")));
    
    addAndMakeVisible (filterContourLabel.get());              //
    filterContourLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterContourLabel->setJustificationType (Justification::centredBottom);
    filterContourLabel->setEditable (false, false, false);
    filterContourLabel->setColour (TextEditor::textColourId, Colours::black);
    filterContourLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    filterDriveSlider.reset(new ParameterSlider(*processor.filterDriveParam, knobStyle(ROTARY)));
    addAndMakeVisible (filterDriveSlider.get());  //
    
    filterDriveLabel.reset ( new Label ("Filter Drive Label", TRANS("Drive")));
    addAndMakeVisible (filterDriveLabel.get());                  //
    filterDriveLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterDriveLabel->setJustificationType (Justification::centredBottom);
    filterDriveLabel->setEditable (false, false, false);
    filterDriveLabel->setColour (TextEditor::textColourId, Colours::black);
    filterDriveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    filterMoogLabel.reset( new Label ("filterMoogLabel", TRANS("MOOG")));
    addAndMakeVisible (filterMoogLabel.get());          //
    filterMoogLabel->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterMoogLabel->setJustificationType (Justification::centredLeft);
    filterMoogLabel->setEditable (false, false, false);
    filterMoogLabel->setColour (TextEditor::textColourId, Colours::black);
    filterMoogLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    filterMS20Label.reset ( new Label ("filterMS20Label", TRANS("MS20")));
    addAndMakeVisible (filterMS20Label.get());          //
    filterMS20Label->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    filterMS20Label->setJustificationType (Justification::centredLeft);
    filterMS20Label->setEditable (false, false, false);
    filterMS20Label->setColour (TextEditor::textColourId, Colours::black);
    filterMS20Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    filter303Label.reset ( new Label ("filter303Label", TRANS("303")));
    addAndMakeVisible (filter303Label.get());          //
    filter303Label->setFont (Font (font, 11.00f, Font::plain).withExtraKerningFactor (0.150f));
    filter303Label->setJustificationType (Justification::centredLeft);
    filter303Label->setEditable (false, false, false);
    filter303Label->setColour (TextEditor::textColourId, Colours::black);
    filter303Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    
 
    // set our component's initial size
    setSize (300, 420);
    
}

FilterSection::~FilterSection()
{
}

void FilterSection::paint (Graphics& g)
{
    int marginY = 8;
    
    {
        int x = getWidth() - 1, y = 8, width = 1, height = getHeight() - marginY;
        Colour fillColour = Colour (0xffcfcfcf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }
}

void FilterSection::resized()
{
    Rectangle<int> area (getLocalBounds());
    
    int headerHeight = 48;
    
    //Rectangle<int> headerArea (area.removeFromTop (headerHeight));
    filterLabel->setBounds (area.removeFromTop(headerHeight));
    filterLabel->setJustificationType(Justification::centred);

    
    int stripWith = area.getWidth();
    int labelHeight = 24;
    int rotaryHeight = 60;
    //int rotaryTextHeight = 10;
    //int vertSliderHeight = 24;
    //int marginX = 8;
    //int marginY = 4;
    {
        
        Rectangle<int> stripArea (area.removeFromLeft((stripWith)));
        
        filterCutoffLabel->setBounds (stripArea.removeFromTop(labelHeight));
        filterCutoffLabel->setJustificationType(Justification::centredBottom);
        filterCutoffSlider->setBounds (stripArea.removeFromTop(rotaryHeight));

		Rectangle<int> mini2(filterCutoffSlider->getBounds());
		filterKeyFollowSlider->setBounds(mini2.removeFromRight(24).reduced(6, 8));
        
        filterResonanceLabel->setBounds (stripArea.removeFromTop(labelHeight));
        filterResonanceLabel->setJustificationType(Justification::centredBottom);
        filterResonanceSlider->setBounds (stripArea.removeFromTop(rotaryHeight));
        
        filterContourLabel->setBounds (stripArea.removeFromTop(labelHeight));
        filterContourLabel->setJustificationType(Justification::centredBottom);
        filterContourSlider->setBounds (stripArea.removeFromTop(rotaryHeight));
        
        filterDriveLabel->setBounds (stripArea.removeFromTop(labelHeight));
        filterDriveLabel->setJustificationType(Justification::centredBottom);
        filterDriveSlider->setBounds (stripArea.removeFromTop(rotaryHeight));
        
        int selectHeight = 48;
        Rectangle<int> selectArea (stripArea.removeFromTop(selectHeight));
        selectArea.translate(12,8);
        filterSelectSlider->setBounds (selectArea.removeFromLeft(14));
        
        filterSelectSlider->setAlwaysOnTop(true);
        filter303Label->setBounds(selectArea.removeFromTop(selectHeight / 3));
        filter303Label->setJustificationType(Justification::topLeft);
        filterMS20Label->setBounds(selectArea.removeFromTop(selectHeight / 3));
        filterMS20Label->setJustificationType(Justification::centredLeft);
        
        filterMoogLabel->setBounds(selectArea.removeFromTop(selectHeight / 3));
        filterMoogLabel->setJustificationType(Justification::bottomLeft);
    }
    
    

}



