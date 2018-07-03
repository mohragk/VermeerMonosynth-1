/*
  ==============================================================================

    SequencerState.h
    Created: 20 Feb 2018 5:00:34pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef SEQUENCER_STATE_H
#define SEQUENCER_STATE_H

#include "../JuceLibraryCode/JuceHeader.h"


class SequencerState
{
public:
	
    SequencerState()
    {}

    ~SequencerState()
    {}

   
    
	void prepareToPlay(double sr)
    {
        
    }

	//METHODS
	

    template <typename FloatType>
	void processBuffer(AudioBUffer<FloatType>& buffer, MidiBuffer& buffer, bool useSequencer)
    {
        // do shit
    }
    
    

    
    
    struct Step
    {
        int noteValue = -1;
        bool isActive = false;
        bool isReleased = false;
        
    }
    
    

private:


	//MEMBERS
    int maxSteps = 8;
    int numSteps = 8;

    double speedInHz;
    Array<Step> steps;
    

    JUCE_LEAK_DETECTOR (SequencerState)
};



#endif // SEQUENCER_STATE_H
