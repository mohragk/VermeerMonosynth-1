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
	
	g.setImageResamplingQuality(Graphics::ResamplingQuality::highResamplingQuality);
	g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void LevelMeter::resized()
{
	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

	if (getWidth() > getHeight())
		currentOrientation = HORIZONTAL;
	else
		currentOrientation = VERTICAL;

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


    float db = Decibels::gainToDecibels(currentLevel);
   
  
	if (currentOrientation == HORIZONTAL)
	{
		Rectangle<float> levelBar ( getBounds().toFloat() );
        
        float maxWidth = levelBar.getWidth();
        
        
        float levelWidth = maxWidth * currentLevel;
        float safeZone = maxWidth * 0.5;
        
       if (db > 0)
       {
          
           auto maxDb = 6.0;
           
           auto difference = maxDb - db;
           auto proportion = Decibels::decibelsToGain(difference);
           
           auto rest = safeZone * proportion;
           
           Rectangle<float> safeBar (levelBar.removeFromLeft(safeZone));
           g.setColour(underZeroLevelColour);
           g.fillRect(safeBar);
           
           Rectangle<float> overBar (levelBar.removeFromLeft(rest));
           g.setColour(Colours::red);
           g.fillRect(overBar);
       }
       else
       {
           Rectangle<float> filledBar(levelBar.removeFromLeft(levelWidth));
           
           g.setColour(underZeroLevelColour);
           g.fillRect(filledBar);
       }

		
	}
	else if (currentOrientation == VERTICAL)
	{
		Rectangle<float> levelBar(getBounds().toFloat().reduced(4));
		float proportion = levelBar.getHeight() * currentLevel;

		Rectangle<float> filledBar(levelBar.removeFromBottom(proportion));

		g.setColour(underZeroLevelColour);
		g.fillRect(filledBar);
	}

	needToRepaint = true;
}



