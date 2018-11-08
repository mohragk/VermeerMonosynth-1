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
		defaultOutlineCol = Colours::darkgrey;
		defaultFillCol = Colours::black;
		setColour(ToggleButton::tickColourId, darkThumb);

        static Typeface::Ptr customTypeface = Typeface::createSystemTypefaceFor(BinaryData::NowMedium_ttf, BinaryData::NowMedium_ttfSize);
		//setDefaultSansSerifTypeface(customTypeface);
        
        LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(customTypeface);
	}


	virtual ~MonosynthLookAndFeel() {}
	
	
    
    static const Font& getCustomFont()
    {
        static Font customFont (Font (Typeface::createSystemTypefaceFor (BinaryData::NowBlack_ttf,
                                                                    BinaryData::NowBlack_ttfSize)));
        return customFont;
    }
    
    void drawTickBox (Graphics &g, Component &c, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override
    {
        ignoreUnused(isEnabled, isMouseOverButton);
        
        Rectangle<float> tickBounds (x, y, w, h);
        Rectangle<float> inner = tickBounds.reduced(0, h / 4.0f);
        auto lineW = 1.8f;
       
        Path backGround;
        backGround.addRoundedRectangle(inner, w / 4.0f);
        
        g.setColour(defaultFillCol);
        g.fillPath(backGround);
        
        g.setColour(defaultOutlineCol);
        g.strokePath(backGround, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::butt));
        
        
        if(ticked)
        {
            //Thumb
            g.setColour (isButtonDown ? c.findColour (ToggleButton::tickColourId): lightThumb);
            g.fillEllipse(inner.getX() + w / 2.0f, inner.getY(), w / 2.0f, h / 2.0f);
            
        }
        else
        {
            // Thumb
            g.setColour (isButtonDown ? lightThumb : c.findColour (ToggleButton::tickColourId));
            g.fillEllipse(inner.getX(),inner.getY(), w / 2.0f, h / 2.0f);
        }
    }

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		auto fill = slider.findColour(Slider::rotarySliderFillColourId);
		auto thumbCol = slider.findColour(Slider::thumbColourId);
        
        defaultOutlineCol = outline;
        defaultFillCol = fill;

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

			g.setColour(outline);
			g.fillEllipse(rx, ry, rw, rw);
			

			
		}
	}
    
    /** deprecated */
    Path getCircleShape(float radius)
    {
        Path p;
        p.addEllipse(0, 0, radius, radius);
        return p;
    }
    
private:
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
    
    Colour defaultOutlineCol;
    Colour defaultFillCol;

};

#endif // MONOSYNTH_LOOK_AND_FEEL_H
