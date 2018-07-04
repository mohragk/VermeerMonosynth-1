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
	
    SequencerState() :
                        maxSteps(8),
                        numSteps(8),
                        currentStep(0),
                        sampleRate(44100.0),
                        speedInHz(10.0),
                        shouldPlay(false)
    {
        for (int i = 0; i < numSteps; i++)
        {
			Step s;
            steps.add(s);
        }
    }

    ~SequencerState()
    {
        steps.clear();
    }

   
    struct Step
    {
        int     noteValue = 60;
        int     pitch = 0;
        bool    isActive = false;
        bool    isReleased = false;
        
        int setAndCalculateNoteValue(int note)
        {
            return  note + pitch;
        }
        
        void setPitch(int p)
        {
            pitch = p;
        }
        
        void setActive (bool a)
        {
            isActive = a;
        }
        
    };
    
	

	//METHODS
    void prepareToPlay(double sr)
    {
        sampleRate = sr;
        currentStep = 0;
        time = 0;
        speedInHz = 10.0;
    }
	
    void setStepSpeedInHz (double hz)
    {
        speedInHz = hz;
    }
    
    void setNoteDuration(double amt)
    {
        noteLengthAmount = amt;
    }

    void setPitchAmountForStep(int step, int p)
    {
        steps[step].setPitch(p);
    }
    
	void setMaxSteps(int max)
	{
		maxSteps = max;
	}
    
    Step getStepData(int s) { return steps[s]; };
    
    
    template <typename FloatType>
	void processBuffer(AudioBuffer<FloatType>& buffer, MidiBuffer& midi, bool useSequencer)
    {
        if(useSequencer)
        {
            auto numSamples = buffer.getNumSamples();
            
            auto stepDuration = static_cast<int> ( std::ceil( sampleRate / speedInHz ) );
            auto noteDuration = static_cast<int> ( std::ceil( stepDuration * noteLengthAmount ) );
            auto difference = stepDuration - noteDuration;
            
            MidiMessage msg;
            int ignore;
            
            for(MidiBuffer::Iterator it(midi); it.getNextEvent(msg, ignore);)
            {
                if (msg.isNoteOn())
                {
                    currentNoteValue = msg.getNoteNumber();
                    shouldPlay = true;
                    currentStep = 0;
                }
                
                if (msg.isNoteOff())
                {
                    shouldPlay = false;
                    
                }
            }
            
            midi.clear();
            
            if ( (time + numSamples + difference) >= stepDuration && shouldPlay )
            {
                auto offset =  jmin((int)(noteDuration - time), numSamples - 1) ;
                
                if (offset >= 0)
                {
					//int note = steps[currentStep].noteValue;
					midi.addEvent(MidiMessage::noteOff(1, currentNoteValue), offset);
					steps[currentStep].setActive(false);

					
					   // currentNoteValue = -1;  
                }
                    
            }
            
            
            if ( (time + numSamples) >= stepDuration && shouldPlay )
            {
                auto offset =  jmax( 0, jmin((int)(stepDuration - time), numSamples - 1) );
                

				currentStep = (currentStep + 1) % maxSteps;
				currentNoteValue = steps[currentStep].setAndCalculateNoteValue(currentNoteValue);
                steps[currentStep].setActive(true);
                
                midi.addEvent(MidiMessage::noteOn(1, currentNoteValue, (uint8)127), offset);
            }
            
            
            time = (time + numSamples) % stepDuration;
        }
    }
    
    
    
    
    

private:


	//MEMBERS
    int maxSteps;
    int numSteps;
    int currentStep;
    
    int currentNoteValue;
    
    int time;

    double sampleRate;
    
    double speedInHz;
    double noteLengthAmount;
    
    bool shouldPlay;
    
    Array<Step> steps;
    

    JUCE_LEAK_DETECTOR (SequencerState)
};



#endif // SEQUENCER_STATE_H
