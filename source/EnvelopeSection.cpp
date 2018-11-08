/*
  ==============================================================================

    EnvelopeSection.cpp
    Created: 17 Feb 2018 12:03:01am
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeSection.h"

//==============================================================================
EnvelopeSection::EnvelopeSection(MonosynthPluginAudioProcessor& p)
                                                :
                                                processor(p)
{
    font = "Futura";
    

	typedef ParameterSlider::style knobStyle;

	//Font f = Font(Font::getDefaultSansSerifFontName(), 22.0f, Font::plain);
    
    envelopesLabel.reset ( new Label ("Envelopes", TRANS("Envelopes")));
    addAndMakeVisible (envelopesLabel.get());
    envelopesLabel->setFont (Font (font, 20.00f, Font::plain).withExtraKerningFactor (0.108f));
    envelopesLabel->setJustificationType (Justification::centredTop);
    envelopesLabel->setEditable (false, false, false);
    envelopesLabel->setColour (TextEditor::textColourId, Colours::black);
    envelopesLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    envAmpLabel.reset (new Label ("Envelope Amp Label", TRANS("Amplitude")));
    addAndMakeVisible (envAmpLabel.get());
    envAmpLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envAmpLabel->setJustificationType (Justification::centredBottom);
    envAmpLabel->setEditable (false, false, false);
    envAmpLabel->setColour (TextEditor::textColourId, Colours::black);
    envAmpLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    // Amplitude Envelope
	attackSlider1.reset(new ParameterSlider(*processor.attackParam1, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (attackSlider1.get());      //
   
    
	decaySlider1.reset(new ParameterSlider(*processor.decayParam1, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (decaySlider1.get());        //
   
    
	sustainSlider1.reset(new ParameterSlider(*processor.sustainParam1, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(sustainSlider1.get());     //
   
    
	releaseSlider1.reset(new ParameterSlider(*processor.releaseParam1, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(releaseSlider1.get());     //
   
    
   
    
    envFilterLabel.reset ( new Label ("Envelope Filter Label", TRANS("Filter Cutoff")));
    addAndMakeVisible (envFilterLabel.get());
    envFilterLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envFilterLabel->setJustificationType (Justification::centredBottom);
    envFilterLabel->setEditable (false, false, false);
    envFilterLabel->setColour (TextEditor::textColourId, Colours::black);
    envFilterLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    // Filter Cutoff Envelope
	attackSlider2.reset(new ParameterSlider(*processor.attackParam3, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (attackSlider2.get());      //
   
    
	decaySlider2.reset(new ParameterSlider(*processor.decayParam3, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (decaySlider2.get());        //
  

	sustainSlider2.reset(new ParameterSlider(*processor.sustainParam3, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(sustainSlider2.get());     //
    
    
	releaseSlider2.reset(new ParameterSlider(*processor.releaseParam3, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(releaseSlider2.get());     //
   
    
    envPitchLabel.reset ( new Label ("Envelope Pitch Label", TRANS("Pitch Mod.")));
    addAndMakeVisible (envPitchLabel.get());
    envPitchLabel->setFont (Font (font, 13.00f, Font::plain).withExtraKerningFactor (0.150f));
    envPitchLabel->setJustificationType (Justification::centredBottom);
    envPitchLabel->setEditable (false, false, false);
    envPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    envPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    // Pitch Modulation Envelope
	attackSlider3.reset(new ParameterSlider(*processor.attackParam2, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (attackSlider3.get());      //
   
    
	decaySlider3.reset(new ParameterSlider(*processor.decayParam2, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (decaySlider3.get());        //
    
    
	sustainSlider3.reset(new ParameterSlider(*processor.sustainParam2, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(sustainSlider3.get());     //
    
    
	releaseSlider3.reset(new ParameterSlider(*processor.releaseParam2, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible(releaseSlider3.get());     //
   
    
    
    
    
	attackCurveSlider1.reset(new ParameterSlider(*processor.attackCurve1Param, knobStyle(LINEARVERTICAL)));
	addAndMakeVisible(attackCurveSlider1.get());//
    attackCurveSlider1->setTooltip (TRANS("set attack curve from exponential to linear"));
   
	
    decRelCurveSlider1.reset(new ParameterSlider(*processor.decayRelCurve1Param, knobStyle(LINEARVERTICAL)));
	addAndMakeVisible(decRelCurveSlider1.get());     //
    decRelCurveSlider1->setTooltip (TRANS("set decay and release curves from exponential to linear"));
   

    
	attackCurveSlider2.reset(new ParameterSlider(*processor.attackCurve3Param, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (attackCurveSlider2.get());    //@TODO: add parameter in Processor
    attackCurveSlider2->setTooltip (TRANS("set attack curve from exponential to linear"));
   

	decRelCurveSlider2.reset(new ParameterSlider(*processor.decayRelCurve3Param, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (decRelCurveSlider2.get());  //
    decRelCurveSlider2->setTooltip (TRANS("set decay and release curves from exponential to linear"));//@TODO: add parameter in Processor
    

	attackCurveSlider3.reset(new ParameterSlider(*processor.attackCurve2Param, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (attackCurveSlider3.get());      //@TODO: add parameter in Processor
    attackCurveSlider3->setTooltip (TRANS("set attack curve from exponential to linear"));
   

	decRelCurveSlider3.reset(new ParameterSlider(*processor.decayRelCurve2Param, knobStyle(LINEARVERTICAL)));
    addAndMakeVisible (decRelCurveSlider3.get()); //@TODO: add parameter in Processor
    decRelCurveSlider3->setTooltip (TRANS("set decay and release curves from exponential to linear"));
    
    
    retriggerButton.reset(new ParamToggleButton(*processor.envelopeRetriggerParam));
    addAndMakeVisible(retriggerButton.get());
    
   
    

	led1.reset(new LED(processor.envelopeLED1));
	addAndMakeVisible(led1.get());
    
    led2.reset(new LED(processor.envelopeLED2));
    addAndMakeVisible(led2.get());
    
    led3.reset(new LED(processor.envelopeLED3));
    addAndMakeVisible(led3.get());
    
    
    setSize(300,300); //Gets reset by plugineditor
    

}

EnvelopeSection::~EnvelopeSection()
{
}

void EnvelopeSection::paint (Graphics& g)
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

void EnvelopeSection::resized()
{
	Rectangle<int> area(getLocalBounds());

	int headerHeight = 48;

	//Rectangle<int> headerArea (area.removeFromTop (headerHeight));
	envelopesLabel->setBounds(area.removeFromTop(headerHeight));
	envelopesLabel->setJustificationType(Justification::centred);


	int labelHeight = 24;
	int envelopeWidth = 72;
	int envelopeHeight = 96;
	//int envMargin = 0;
	int sliderWidth = 18;
	int sliderMargin = 2;
	//int rotarySize = 48;

	{
		Rectangle<int> envArea(area.removeFromTop(envelopeHeight + labelHeight).reduced(16, 0));

		envAmpLabel->setBounds(envArea.removeFromTop(labelHeight));
		envAmpLabel->setJustificationType(Justification::centredBottom);

        Rectangle<int> ledArea(envArea);
        led1->setBounds(ledArea.removeFromRight(16));
        led1->setBackgroundColour(Colour(0xff0e0e0e));
        
        
		Rectangle<int> envBlock(envArea.removeFromLeft(sliderWidth * 4));

		attackSlider1->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		decaySlider1->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		sustainSlider1->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		releaseSlider1->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));

		Rectangle<int> tweakArea(envArea.removeFromLeft(envelopeWidth));
		attackCurveSlider1->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));
		decRelCurveSlider1->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));



	}

	{
		Rectangle<int> envArea(area.removeFromTop(envelopeHeight + labelHeight).reduced(16, 0));

		envFilterLabel->setBounds(envArea.removeFromTop(labelHeight));
		envFilterLabel->setJustificationType(Justification::centredBottom);

        Rectangle<int> ledArea(envArea);
        led2->setBounds(ledArea.removeFromRight(16));
        led2->setBackgroundColour(Colour(0xff0e0e0e));
        
        Rectangle<int> envBlock(envArea.removeFromLeft(sliderWidth * 4));

		attackSlider2->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		decaySlider2->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		sustainSlider2->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		releaseSlider2->setBounds(envBlock.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));

		Rectangle<int> tweakArea(envArea.removeFromLeft(envelopeWidth));
		attackCurveSlider2->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));
		decRelCurveSlider2->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));



	}
	{
		Rectangle<int> envArea(area.removeFromTop(envelopeHeight + labelHeight).reduced(16, 0));

		envPitchLabel->setBounds(envArea.removeFromTop(labelHeight));
		envPitchLabel->setJustificationType(Justification::centredBottom);

        Rectangle<int> ledArea(envArea);
        led3->setBounds(ledArea.removeFromRight(16));
        led3->setBackgroundColour(Colour(0xff0e0e0e));
        
		attackSlider3->setBounds(envArea.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		decaySlider3->setBounds(envArea.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		sustainSlider3->setBounds(envArea.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));
		releaseSlider3->setBounds(envArea.removeFromLeft(sliderWidth).reduced(sliderMargin, 0));

		Rectangle<int> tweakArea(envArea.removeFromLeft(envelopeWidth));
		attackCurveSlider3->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));
		decRelCurveSlider3->setBounds(tweakArea.removeFromTop(envelopeHeight / 2).reduced(25, 0));
        
        



	}

	{
		Rectangle<int> buttonArea(area.removeFromTop(labelHeight * 4));

		retriggerButton->setBounds(buttonArea.removeFromLeft(area.getWidth()).reduced(12, 0));
		retriggerButton->setButtonText("Retrigger");
	}

}
	
