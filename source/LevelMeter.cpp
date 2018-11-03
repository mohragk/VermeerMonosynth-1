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



void LevelMeter::paint(Graphics& g)
{
    const ScopedLock sl (imageLock);
	g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void LevelMeter::resized()
{
    const ScopedLock sl (imageLock);
    
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
    const ScopedLock sl (imageLock);
    
	Graphics g(image);

    g.fillAll(backgroundColour);

	Rectangle<float> bounds = image.getBounds().toFloat();
    const Rectangle<float> innerBounds ( bounds );
    
    auto lineW = 4.0;
    auto lineCentreY = innerBounds.getCentreY() - (lineW / 2.0f);
    auto lineCentreX = innerBounds.getCentreX() - (lineW / 2.0f);
   
    

  
	if (currentOrientation == HORIZONTAL)
	{
        Colour bg = Colours::darkgrey.darker().darker();
        g.setColour(bg);
        
        Line<float> fullLine (innerBounds.getX(), lineCentreY, innerBounds.getRight(), lineCentreY);
        
        g.drawLine(fullLine, lineW);
        
        auto safeWidth  = scale;
        auto levelWidth = currentLevel * scale;
        
        if (levelWidth > safeWidth)
        {
            Line<float> safeLine (fullLine.withShortenedEnd(innerBounds.getWidth() - safeWidth));
            
            g.setColour(lowLevelColour.darker());
            g.drawLine(safeLine, lineW);
            
            Line<float> overLine (fullLine.withShortenedStart(safeWidth));
            g.setColour(maxLevelColour);
            g.drawLine(overLine, lineW);
        }
        else
        {
            Line<float> levelLine (fullLine.withShortenedEnd(innerBounds.getWidth() - levelWidth));
            
            g.setColour(lowLevelColour.darker());
            g.drawLine(levelLine, lineW);
        }
	}
	else if (currentOrientation == VERTICAL)
	{
        //
        // UNTESTED
        //
        
        
        Colour bg = Colours::darkgrey.darker().darker();
        g.setColour(bg);
        
        Line<float> fullLine (lineCentreX, innerBounds.getBottom(), lineCentreX, innerBounds.getY());
        
        g.drawLine(fullLine, lineW);
        
        auto safeHeight = scale; //iec_scale(-0.3f);
        auto levelHeight = currentLevel * scale;//iec_scale(levelDb);
        
        if (levelHeight > safeHeight)
        {
            Line<float> safeLine (fullLine.withShortenedEnd(innerBounds.getHeight() - safeHeight));
            
            g.setColour(lowLevelColour.darker());
            g.drawLine(safeLine, lineW);
            
            Line<float> overLine (fullLine.withShortenedStart(safeHeight));
            g.setColour(maxLevelColour);
            g.drawLine(overLine, lineW);
        }
        else
        {
            Line<float> levelLine (fullLine.withShortenedEnd(innerBounds.getHeight() - levelHeight));
            
            g.setColour(lowLevelColour.darker());
            g.drawLine(levelLine, lineW);
        }
	}

	needToRepaint = true;
}



