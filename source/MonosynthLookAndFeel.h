/*
  ==============================================================================

    MonosynthLookAndFeel.h
    Created: 29 Oct 2018 5:30:42pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef MONOSYNTH_LOOK_AND_FEEL_H
#define MONOSYNTH_LOOK_AND_FEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

class MonosynthLookAndFeel : public LookAndFeel_V4
{
public:
	MonosynthLookAndFeel() 
	{
		//setColour(Slider::rotarySliderFillColourId, Colours::aquamarine);
	}


	virtual ~MonosynthLookAndFeel() {}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		auto fill = slider.findColour(Slider::rotarySliderFillColourId);
		auto thumbCol = slider.findColour(Slider::thumbColourId);

		auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
		

		auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
		auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto lineW = jmin(8.0f, radius * 0.20f);
		auto arcRadius = radius - lineW * 0.5f;

		auto innerBounds = bounds.reduced(lineW + 1.0f);

		Path backgroundArc;
		backgroundArc.addCentredArc(bounds.getCentreX(),
			bounds.getCentreY(),
			arcRadius,
			arcRadius,
			0.0f,
			rotaryStartAngle,
			rotaryEndAngle,
			true);

		g.setColour(outline);
		g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::butt));

		if (slider.isEnabled())
		{
			Path valueArc;
			valueArc.addCentredArc(bounds.getCentreX(),
				bounds.getCentreY(),
				arcRadius,
				arcRadius,
				0.0f,
				rotaryStartAngle,
				toAngle,
				true);

			g.setColour(thumbCol);
			g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::butt));


			// KNOB
			auto knobRadius = jmin(innerBounds.getWidth() / 2, innerBounds.getHeight() / 2);
			auto rx = innerBounds.getCentreX() - knobRadius;
			auto ry = innerBounds.getCentreY() - knobRadius;
			auto rw = knobRadius * 2.0f;
			auto knobAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

			g.setColour(fill);
			g.fillEllipse(rx, ry, rw, rw);
			//g.setColour(Colour(0xff0e0e0e));
			//g.drawEllipse(rx, ry, rw, rw, 2.0f);

			Path pointer;
			auto pointerLength = knobRadius * 0.4f;
			auto pointerThickness = 2.0f;
			pointer.addRectangle(-pointerThickness * 0.5f, -knobRadius + 2.0f, pointerThickness, pointerLength);
			pointer.applyTransform(AffineTransform::rotation(knobAngle).translated(innerBounds.getCentreX(), innerBounds.getCentreY()));

			g.setColour(thumbCol);
			g.fillPath(pointer);
		}
		else
		{

			// KNOB
			auto knobRadius = jmin(innerBounds.getWidth() / 2, innerBounds.getHeight() / 2);
			auto rx = innerBounds.getCentreX() - knobRadius;
			auto ry = innerBounds.getCentreY() - knobRadius;
			auto rw = knobRadius * 2.0f;
			auto knobAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

			g.setColour(outline);
			g.fillEllipse(rx, ry, rw, rw);
			//g.setColour(Colour(0xff0e0e0e));
			//g.drawEllipse(rx, ry, rw, rw, 2.0f);

			
		}

		/*
		auto thumbWidth = lineW * 2.0f;
		Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
			bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

		g.setColour(slider.findColour(Slider::thumbColourId));
		g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
		*/
	}

};

#endif // MONOSYNTH_LOOK_AND_FEEL_H
