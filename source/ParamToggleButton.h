/*
  ==============================================================================

    ParamToggleButton.h
    Created: 9 Oct 2018 6:56:36pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef PARAM_TOGGLE_BUTTON_H
#define PARAM_TOGGLE_BUTTON_H



class ParamToggleButton : public ToggleButton, private Timer
{
public:
    ParamToggleButton(AudioProcessorParameter& p ) : ToggleButton(p.getName(256)), param(p)
    {
        
        updateToggleState();
        startTimerHz(30);
    };
    
    ~ParamToggleButton() {};
    
    void timerCallback() override { }; //updateToggleState(); };
    
    void clicked() override
    {
        int newState = ToggleButton::getToggleState();
            
        if (newState != param.getValue())
            param.setValue(newState);
    };
    
private:
    AudioProcessorParameter& param;
    
    void updateToggleState()
    {
        const int newState = param.getValue();
        
        if( newState != ToggleButton::getToggleState() )
        {
            ToggleButton::setToggleState ( newState, dontSendNotification );
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamToggleButton)
};

#endif //PARAM_TOGGLE_BUTTON_H