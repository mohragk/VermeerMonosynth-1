/*
  ==============================================================================

    LEDComponent.h
    Created: 7 Nov 2018 9:10:39pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef LED_COMPONENT_H
#define LED_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "LEDProcessor.h"

class LED : public Component, public Timer
{
public:
    LED(LEDProcessor& proc);
    ~LED();
    
	void setBackgroundColour(Colour col) { backgroundColour = col; };
	void setLEDColour(Colour col) { ledColour = col; };
    

    void paint(Graphics& g) override;
	void resized() override;

	void timerCallback() override;
	
private:
    LEDProcessor& source;

	CriticalSection imageLock;

	Image image;

	Colour backgroundColour = Colours::black;
	Colour ledColour = Colours::white;

	void renderImage();
    
};

#endif // LED_COMPONENT_H