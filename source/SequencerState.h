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
						time(0),
                        sampleRate(44100.0),
                        speedInHz(10.0),
						noteLengthAmount(0.5)
    {

    }

    ~SequencerState()
    {
    }

    struct Step
    {
        
        int     noteValue;
        int     pitch;
        bool    isActive = false;
        
        int getPitchedNoteValue()
        {
            return noteValue + pitch;
        }
        
        void setInitialNoteValue (int note)
        {
            noteValue = note;
        }
        
        void setPitch(int p)
        {
            pitch = p;
        }
        
        void setActive (bool a)
        {
            isActive = a;
        }
        
    } steps[8];

    
    Step getStepData (int s) { return steps[s]; };

	//METHODS
    void prepareToPlay(double sr)
    {
	    pressedKeys.clear();
        sampleRate = sr;
        currentStep = 0;
        time = 0;
        speedInHz = 10.0;
    }
	
    void setSpeedInHz (double hz)
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
                    int note = msg.getNoteNumber();
                    
                    for (int i = 0; i < numSteps; ++i)
                        steps[i].setInitialNoteValue(note);
                    
                    pressedKeys.add(note);
                    
                    if (pressedKeys.size() == 1)
                    {
                        currentStep = 0;
                        time = 0;
                    }
					steps[currentStep].setActive(true);
                    
                    
                }
                
                
                if (msg.isNoteOff())
                {
                    int note = msg.getNoteNumber();
                    pressedKeys.removeFirstMatchingValue(note);
                }
                
            }
            
            

            if ( (time + numSamples + difference) >= stepDuration)
            {
                auto offset =  jmin((int)(noteDuration - time), numSamples - 1) ;
                
                if (offset >= 0)
                {
                    for (int i = 0; i < numSteps; i++)
                    {
                        if (steps[i].isActive)
                        {
                            int note = steps[i].getPitchedNoteValue();
                            midi.addEvent(MidiMessage::noteOff(1, note), offset);
                            steps[i].setActive(false);
                        }
                    }
                }
            }
            
            
            if ( (time + numSamples) >= stepDuration )
            {
                auto offset =  jmax( 0, jmin((int)(stepDuration - time), numSamples - 1) );
             
                if (pressedKeys.size() > 0)
                {
                    int note = pressedKeys.getLast();
                    for (int i = 0; i < numSteps; ++i)
                        steps[i].setInitialNoteValue(note);
                    
					currentStep = (currentStep + 1) % (maxSteps + 1);

                    steps[currentStep].setActive(true);
                    
                    int pitchedNote = steps[currentStep].getPitchedNoteValue();
                    midi.addEvent(MidiMessage::noteOn(1, pitchedNote, (uint8)127), offset);
                }
                
            }
            
            
            time = (time + numSamples) % stepDuration;
        }
    }
    

private:
    int maxSteps;
    int numSteps;
    int currentStep;
    
    int time;

    double sampleRate;
    
    double speedInHz;
    double noteLengthAmount;
        
    Array<int> pressedKeys;

    JUCE_LEAK_DETECTOR (SequencerState)
};



#endif // SEQUENCER_STATE_H
