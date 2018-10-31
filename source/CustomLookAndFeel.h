/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 27 Oct 2018 3:50:31pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#ifndef CUSTOM_LOOKANDFEEL_H
#define CUSTOM_LOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"


class CustomLookAndFeel : public LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        
        setColour(ToggleButton::tickColourId, darkThumb);
        setColour(ToggleButton::tickDisabledColourId, Colours::darkgrey);
    }
    virtual ~CustomLookAndFeel() {}
    
    void drawTickBox (Graphics &g, Component &c, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown)
    {
        ignoreUnused(isEnabled, isMouseOverButton, isButtonDown);
        
        Rectangle<float> tickBounds (x, y, w, h / 2.0f);
        
        g.setColour(c.findColour (ToggleButton::tickDisabledColourId));
        g.drawRoundedRectangle (tickBounds, w / 4.0f, 1.8f);
        
        if(ticked)
        {
            g.setColour (lightThumb);
            g.fillEllipse(x + w / 2.0f,y, w / 2.0f, h / 2.0f);
            
        }
        else
        {
            g.setColour (c.findColour (ToggleButton::tickColourId));
            g.fillEllipse(x,y, w / 2.0f, h / 2.0f);
        }
    }
    
    
    void drawTickBoxAlt (Graphics &g, Component &c, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown)
    {
        ignoreUnused (isEnabled, isMouseOverButton, isButtonDown);
        
        Rectangle<float> tickBounds (x, y, w, h);
        
        g.setColour (c.findColour (ToggleButton::tickDisabledColourId));
        
        g.drawEllipse (tickBounds, 1.0f);
        //g.drawRoundedRectangle (tickBounds, 4.0f, 1.0f);
        
        if (ticked)
        {
            g.setColour (c.findColour (ToggleButton::tickColourId));
            auto tick = getTickShape (0.75f);
            auto circle = getCircleShape(1);
            g.fillPath (circle, circle.getTransformToScaleToFit (tickBounds.reduced (4, 4).toFloat(), false));
        }
        
        
    }
            
    Path getCircleShape(float radius)
    {
        Path p;
        p.addEllipse(0, 0, radius, radius);
        return p;
    }
    
private:
    Colour lightThumb = Colour(0xffdee5fc);
    Colour darkThumb = Colour(0xff3e7db3);
};

#endif // CUSTOM_LOOKANDFEEL_H
