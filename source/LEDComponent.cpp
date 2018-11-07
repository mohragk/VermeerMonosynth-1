/*
  ==============================================================================

    LEDComponent.cpp
    Created: 7 Nov 2018 9:15:06pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "LEDComponent.h"


LED::LED(LEDProcessor& proc) : source(proc)
{
	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

	startTimerHz(30);
}

LED::~LED() 
{
	stopTimer();
}


void LED::paint(Graphics& g)
{
	const ScopedLock sl(imageLock);
	g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void LED::resized()
{
	const ScopedLock sl(imageLock);

	image = Image(Image::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);

	Graphics g(image);

	g.fillAll(backgroundColour);
}

void LED::timerCallback()
{
	renderImage();
	repaint();

}

void LED::renderImage()
{
	const ScopedLock sl(imageLock);

	Graphics g(image);

	g.fillAll(backgroundColour);

	float w = image.getWidth();
	float h = image.getHeight();

	float maxRadius = jmin(w, h);
	float innerRadius = maxRadius * 0.75;

	float centreX = image.getBounds().getCentreX();
	float centreY = image.getBounds().getCentreY();

	Rectangle<float> outerBounds(centreX - maxRadius / 2.0f, centreY - maxRadius / 2.0f, maxRadius, maxRadius);
	Rectangle<float> innerBounds (centreX - innerRadius / 2.0f, centreY - innerRadius / 2.0f, innerRadius, innerRadius );

	//g.setColour(Colours::darkgrey);
	//g.drawEllipse(outerBounds, 1.0f);

	g.setColour(Colours::white.withAlpha( source.getBrightness() ) );
	g.fillEllipse(innerBounds);
}
