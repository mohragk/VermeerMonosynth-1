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
	//const float peakDb = juce::Decibels::gainToDecibels(peak, infinity);

	if (levelDb < infinity)
		levelDb = infinity;
	else if (levelDb > 4.0)
		levelDb = 4.0;



    const juce::Rectangle<float> floored (ceilf (bounds.getX()) + 1.0f, ceilf (bounds.getY()) + 1.0f,
                                          floorf (bounds.getRight()) - (ceilf (bounds.getX() + 2.0f)),
                                          floorf (bounds.getBottom()) - (ceilf (bounds.getY()) + 2.0f));

   
  
	if (currentOrientation == HORIZONTAL)
	{
		int x;
		int x_over = 0;
		int x_curr = 0;

		x = iec_scale(levelDb);


		/*
		if (horizontalGradient.getNumColours() < 2) {
			horizontalGradient = ColourGradient(lowLevelColour,
				floored.getX(), floored.getY(),
				maxLevelColour,
				floored.getRight(), floored.getY(), false);
			horizontalGradient.addColour(0.5, lowLevelColour);
			horizontalGradient.addColour(0.75, midLevelColour);
		}
        
		g.setGradientFill(horizontalGradient);
		g.fillRect(floored.withRight(floored.getRight() - levelDb * floored.getWidth() / infinity));
        */
     
		/*
		if (levelDb > -1.0)
		{
			Rectangle<float> safeBar(floored.withRight(floored.getRight() - (-1.0f * floored.getWidth() / infinity)));
			g.setColour(lowLevelColour);
			g.fillRect(safeBar);

			Rectangle<float> overBar(
				floored
				.withLeft(floored.getX() + (-1.0f * floored.getWidth() / infinity))
				//.withRight(floored.getRight() - (levelDb * floored.getWidth() / infinity))
			);
			g.setColour(maxLevelColour);
			g.fillRect(overBar);
		}
		else
		{
			Rectangle<float> safeBar(floored.withRight(floored.getRight() - (levelDb * floored.getWidth() / infinity)));
			g.setColour(lowLevelColour);
			g.fillRect(safeBar);
		}
		*/


		
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

