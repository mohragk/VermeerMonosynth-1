/*
  ==============================================================================

    LevelMeter.h
    Created: 31 Oct 2018 7:30:37pm
    Author:  Meneer Vermeer

  ==============================================================================
*/

#ifndef LEVEL_METER_H
#define LEVEL_METER_H

#include "../JuceLibraryCode/JuceHeader.h"


class LevelMeter : public Component, public Timer
{
public :

	LevelMeter();
	~LevelMeter();

	enum orientation
	{
		HORIZONTAL,
		VERTICAL,
		numOrientations
	};

	void setLevel(double level);

	void setOrientation( orientation o ) { currentOrientation = o ; };

	void setBackgroundColour(Colour col) { backgroundColour = col; };
	void setLowLevelColour(Colour col) { lowLevelColour = col; };
	void setMidLevelColour(Colour col) { midLevelColour = col; }; // unused
	void setMaxLevelColour(Colour col) { maxLevelColour = col; };

    
    
	void paint(Graphics& g) override;
	void resized() override;

	void timerCallback() override;
    

private :

    
	double currentLevel;
	float scale;
    
    int maxLevel = 0;
    uint64 prevTime;
    int decayRateMillis = 500;
    int countDownTimer = decayRateMillis;

	orientation currentOrientation;

	bool needToRepaint;
    bool needToUpdate;

	Image image;

	Colour backgroundColour = Colours::black;
	Colour lowLevelColour = Colours::white;
	Colour midLevelColour = Colours::orange;
    Colour maxLevelColour = Colours::red;


	void renderImage();

	int iec_scale(const float dB);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};

#endif // LEVEL_METER_H
