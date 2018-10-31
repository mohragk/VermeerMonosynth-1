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
	currentLevel = jmin(jmax(0.0, level), 1.0);
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


	auto h = image.getHeight();
	auto w = image.getWidth();


	if (currentOrientation == HORIZONTAL)
	{
		Rectangle<float> levelBar ( getBounds().toFloat().reduced(4) );

		float proportion = levelBar.getWidth() * currentLevel;

		Rectangle<float> filledBar(levelBar.removeFromLeft(proportion));

		g.setColour(underZeroLevelColour);
		g.fillRect(filledBar);
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
