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
	
	g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void LevelMeter::resized()
{
	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

    float scaler = 0.8;

	if (getWidth() > getHeight())
	{
		currentOrientation = HORIZONTAL;
		scale = scaler * getWidth();
	}
	else
	{
		currentOrientation = VERTICAL;
		scale = scaler * getHeight();
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


    const Rectangle<float> floored ( bounds.reduced(1,1) );
   
    Colour bg = Colours::darkgrey.darker().darker();
    g.setColour(bg);
    g.fillRect(floored);

  
	if (currentOrientation == HORIZONTAL)
	{
        auto safeWidth = scale; //iec_scale(-0.3f);
        auto levelWidth = currentLevel * scale;//iec_scale(levelDb);
        
        if (levelWidth > safeWidth)
        {
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
        auto safeHeight = scale; //iec_scale(-0.3f);
        auto levelHeight = currentLevel * scale;//iec_scale(levelDb);
        
        if (levelHeight > safeHeight)
        {
            Rectangle<float> safeBounds (floored.withTop(floored.getY() + safeHeight));
            
            g.setColour(lowLevelColour.darker());
            g.fillRect(safeBounds);
            
            Rectangle<float> restBounds (floored.withBottom(floored.getY() + floored.getHeight() - safeHeight));
            // restBounds = restBounds.withRight(restBounds.getRight() - restWidth);
            g.setColour(maxLevelColour);
            g.fillRect(restBounds);
        }
        else
        {
            Rectangle<float> levelBounds (floored.withTop(floored.getY() + levelHeight));
            
            g.setColour(lowLevelColour.darker());
            g.fillRect(levelBounds);
        }
	}

	needToRepaint = true;
}

// UNUSED
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

