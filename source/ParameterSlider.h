/*
  ==============================================================================

    ParameterSlider.h
    Created: 16 Feb 2018 4:36:52pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef PARAMETER_SLIDER_H
#define PARAMETER_SLIDER_H


class ParameterSlider   : public Slider, private Timer
{
public:
	ParameterSlider(AudioProcessorParameter& p, int type)
		: Slider(p.getName(256)), param(p)
	{
		setRange(0.0, 1.0, 0.0);
		startTimerHz(60);
		updateSliderPos();

		if (type == ROTARY) { setSliderStyle(Slider::RotaryVerticalDrag); }
		else if (type == LINEARHORIZONTAL) {setSliderStyle(Slider::LinearHorizontal);}
		else { setSliderStyle(Slider::LinearVertical); }

		setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        setColour(thumbColourId, Colour (0xff3e7db3));
    }

	enum style 
	{
		ROTARY = 0,
		LINEARHORIZONTAL,
		LINEARVERTICAL
	};

    void valueChanged() override
    {
        if (isMouseButtonDown())
            param.setValueNotifyingHost ((float) Slider::getValue());
        else
            param.setValue ((float) Slider::getValue());
    }

    void timerCallback() override       { updateSliderPos(); }

    void startedDragging() override     { param.beginChangeGesture();  }
    void stoppedDragging() override     { param.endChangeGesture();   }

	

    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }

    void updateSliderPos()
    {
        const float newValue = param.getValue();

        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
        {           
            Slider::setValue (newValue);
        }
        
    }

    AudioProcessorParameter& param;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

#endif //PARAMATER_SLIDER_H
