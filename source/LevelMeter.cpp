/*
  ==============================================================================

    LevelMeter.cpp
    Created: 31 Oct 2018 7:36:12pm
    Author:  Meneer Vermeer

  ==============================================================================
*/

#include "LevelMeter.h"

LevelMeter::LevelMeter() :
	currentLevel(0.0),
	needToRepaint(false)
{
	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

    
	startTimerHz(30);
}

LevelMeter::~LevelMeter() 
{
	stopTimer();
    
    
}

void LevelMeter::setLevel(double level)
{
    
	// TODO: make it logarithmic in response
    
  
    currentLevel = level;
    
    
}

void LevelMeter::paint(Graphics& g)
{
	
	//g.setImageResamplingQuality(Graphics::ResamplingQuality::highResamplingQuality);
	g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void LevelMeter::resized()
{
	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

	

	if (getWidth() > getHeight())
	{
		currentOrientation = HORIZONTAL;
		scale = 0.85f * getWidth();
	}
	else
	{
		currentOrientation = VERTICAL;
		scale = 0.85f * getHeight();
	}

	Graphics g(image);

	g.fillAll(backgroundColour);

	needToRepaint = true;
}

void LevelMeter::timerCallback()
{

    renderImage();
	if (needToRepaint)
		repaint();
}



void LevelMeter::renderImage()
{
	Graphics g(image);

    g.fillAll(backgroundColour);

	Rectangle<float> bounds = image.getBounds().toFloat();

	const float infinity = -70.0f;
	float levelDb = Decibels::gainToDecibels((float)currentLevel, infinity);

	if (levelDb < infinity)
		levelDb = infinity;
	else if (levelDb > 6.0)
		levelDb = 6.0;


    /*
    const juce::Rectangle<float> floored (ceilf (bounds.getX()) + 1.0f, ceilf (bounds.getY()) + 1.0f,
                                          floorf (bounds.getRight()) - (ceilf (bounds.getX() + 2.0f)),
                                          floorf (bounds.getBottom()) - (ceilf (bounds.getY()) + 2.0f));

     */
    const Rectangle<float> floored ( image.getBounds().toFloat().reduced(1,1) );
   
    Colour bg = Colours::darkgrey.darker().darker();
    g.setColour(bg);
    g.fillRect(floored);

  
	if (currentOrientation == HORIZONTAL)
	{
        scale = 0.85f * floored.getWidth();
        auto safeWidth = iec_scale(0.0f);
		auto levelWidth = iec_scale(levelDb);
        
        
        
        if (levelWidth > safeWidth)
        {
            int restWidth = floored.getWidth() - levelWidth;
            

            Rectangle<float> safeBounds (floored.withRight(floored.getX() + safeWidth));
            
            g.setColour(lowLevelColour.darker());
            g.fillRect(safeBounds);
            
            Rectangle<float> restBounds (floored.withLeft(floored.getX() + safeWidth));
           // restBounds = restBounds.withRight(restBounds.getRight() - restWidth);
            g.setColour(maxLevelColour);
            g.fillRect(restBounds);
            
        }
        else
        {
            Rectangle<float> levelBounds (floored.withRight(floored.getX() + levelWidth));
            
            g.setColour(lowLevelColour.darker());
            g.fillRect(levelBounds);
        }

		

		
	}
	else if (currentOrientation == VERTICAL)
	{
		//TODO
	}

	needToRepaint = true;
}

int LevelMeter::iec_scale(const float dB)
{
	float def = 1.0f;

	if (dB < -70.0f)
		def = 0.0;
	else if (dB < -60.0f)
		def = (dB + 70.0) * 0.0025;
	else if (dB < -50.0f)
		def = (dB + 60.0) * 0.005 + 0.025;
	else if (dB < -40.0f)
		def = (dB + 50.0) * 0.0075 + 0.075;
	else if (dB < -30.0f)
		def = (dB + 40.0) * 0.015 + 0.015;
	else if (dB < -20.0f)
		def = (dB + 30.0) * 0.02 + 0.3;
	else
		def = (dB + 20.0) * 0.025 + 0.5;

	return std::round(def * scale);
}

