/*
  ==============================================================================

    LEDProcessor.h
    Created: 7 Nov 2018 9:10:49pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef LED_PROCESSOR_H
#define LED_PROCESSOR_H

#include "../JuceLibraryCode/JuceHeader.h"

class LEDProcessor
{
public:
	LEDProcessor() : brightness(0.0) {}
	~LEDProcessor() {}

	float getBrightness() { return brightness; };
	
	void setBrightness(float level)
	{
		brightness = jlimit(0.0f, 1.0f, level);
	}

private:
	float brightness;
};

#endif // LED_PROCESSOR_H
