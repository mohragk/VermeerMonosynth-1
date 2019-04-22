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
						noteLengthAmount(0.5),
						swingOffset(0.5)
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
		bool 	shouldPlay = true;
        
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
	    
		void setPlayable( bool b )
		{
			shouldPlay = b;
		}
        
        bool shouldTrigger()
        {
            return shouldPlay;
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
		swingOffset = 0.5;
    }
	
 
    void setSpeedInHz (double hz);
    void setSwingAmount(int amountPerc);
    void setNoteDuration(double amt);
    void setPitchAmountForStep(int step, int p);
    void setShouldPlayForStep(int step, bool choice);
    void setMaxSteps(int max);
    
    template <typename FloatType>
    void processBuffer(AudioBuffer<FloatType>& buffer, MidiBuffer& midi, bool useSequencer)
    {
        if(useSequencer)
        {
            auto numSamples = buffer.getNumSamples();
            
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
                    
                    
                    // keep playing during sequence
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
            

            if (steps[currentStep].shouldTrigger() == false)
                midi.clear();
            
            
            int interval = jmin(128, numSamples);
            int samplesRemaining = numSamples;
            int position = 0;
            
            while (position < numSamples)
            {
                double sd = sampleRate / speedInHz;
                
                sd = currentStep % 2 == 0 ? sd * 2.0 * swingOffset : sd * 2.0 * (1.0 - swingOffset);
                
                auto nd = sd * noteLengthAmount;
                
                int stepDuration = std::round(sd);
                int noteDuration = std::round(nd);
                
                if ( (time + interval) >= noteDuration)
                {
                    auto offset =  jmin((noteDuration - time) + position, numSamples - 1) ;
                    
                    if (offset >= 0)
                    {
                        for (int i = 0; i < numSteps; i++)
                        {
                            
                            if (steps[i].isActive)
                            {
                                int note = steps[i].getPitchedNoteValue();
                                midi.addEvent(MidiMessage::noteOff(1,note), offset);
                                steps[i].setActive(false);
                                
                            }
                        }
                    }
                }
                
                
                if ( (time + interval) >= stepDuration)
                {
                    auto offset =  std::max( 0, std::min((stepDuration - time) + position, numSamples - 1) );
                    
                    if (pressedKeys.size() > 0)
                    {
                        int note = pressedKeys.getLast();
                        for (int i = 0; i < numSteps; ++i)
                            steps[i].setInitialNoteValue(note);
                        
                        currentStep = (currentStep + 1) % (maxSteps + 1);
                        
                        if (steps[currentStep].shouldTrigger())
                        {
                            steps[currentStep].setActive(true);
                            int pitchedNote = steps[currentStep].getPitchedNoteValue();
                            midi.addEvent(MidiMessage::noteOn(1, pitchedNote, (uint8)127), offset);
                        }
                    }
                    
                }
                if (samplesRemaining >= interval)
                {
                    time = (time + interval) % stepDuration;
                    position += interval;
                }
                else
                {
                    time = (time + samplesRemaining) % stepDuration;
                    position += samplesRemaining;
                }
                
                samplesRemaining -= interval;
            }
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
	double swingOffset;
        
    Array<int> pressedKeys;

    JUCE_LEAK_DETECTOR (SequencerState)
};


inline void SequencerState::setSpeedInHz (double hz)
{
    speedInHz = hz;
}

inline void SequencerState::setSwingAmount(int amountPerc)
{
    swingOffset = (double)amountPerc / 100;
}

inline void SequencerState::setNoteDuration(double amt)
{
    noteLengthAmount = amt;
}

inline void SequencerState::setPitchAmountForStep(int step, int p)
{
    steps[step].setPitch(p);
}

inline void SequencerState::setShouldPlayForStep(int step, bool choice)
{
    steps[step].setPlayable(choice);
}

inline void SequencerState::setMaxSteps(int max)
{
    maxSteps = max;
}


#endif // SEQUENCER_STATE_H
