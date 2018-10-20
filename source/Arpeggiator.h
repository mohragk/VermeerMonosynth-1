/*
  ==============================================================================

    Arpeggiator.h
    Created: 2 Jul 2018 6:57:17pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef ARPEGGIATOR_H
#define ARPEGGIATOR_H

#include "../JuceLibraryCode/JuceHeader.h"


class Arpeggiator 
{
	public:
		Arpeggiator()
		{}

		~Arpeggiator() {}

		void prepareToPlay(double sr, int)
		{
			sortedNotes.clear();
			playedNotes.clear();
			currentNote = 0;
			lastNoteValue = -1;
			time = 0;
			sampleRate = static_cast<float> (sr);
			speedInHz = 10.f;
		}

		void setSpeedInHz(double sp)
		{
			speedInHz = sp;
		}

		template <typename FloatType>
		void process(AudioBuffer<FloatType>& buffer, MidiBuffer& midi, bool useArpeggiator, bool sorted)
		{
			if (useArpeggiator)
			{
				auto numSamples = buffer.getNumSamples();

                
				auto noteDuration = static_cast<int> ( std::round( sampleRate / speedInHz) ) ;

				MidiMessage msg;
				int ignore;

				for (MidiBuffer::Iterator it(midi); it.getNextEvent(msg, ignore);)
				{
					if (msg.isNoteOn())  
					{
						sortedNotes.add(msg.getNoteNumber());
						playedNotes.add(msg.getNoteNumber());
                        
                        if(sortedNotes.size() == 1)
                            time=0;
					}
					else if (msg.isNoteOff()) 
					{
						sortedNotes.removeValue(msg.getNoteNumber());
						playedNotes.removeFirstMatchingValue(msg.getNoteNumber());
					}
					
				}

				midi.clear();

                if (prevSpeedInHz != speedInHz)
                {
                    midi.addEvent(MidiMessage::allNotesOff(1), 0);
                }
                
                int interval = jmin(256, numSamples);
                int samplesRemaining = numSamples;
                int position = 0;
                
                while (position < numSamples)
                {
                
                    if ((time + interval) >= noteDuration)
                    {
                        auto offset = jmax(0, jmin((int)(noteDuration - time) + position, numSamples - 1));

                        if (lastNoteValue > 0)
                        {
                            midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
                            lastNoteValue = -1;
                        }

                        if (sortedNotes.size() > 0)
                        {
                            if(sorted)
                            {
                                currentNote = (currentNote + 1) % sortedNotes.size();
                                lastNoteValue = sortedNotes[currentNote];
                                midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
                            }
                            else
                            {
                                currentNote = (currentNote + 1) % playedNotes.size();
                                lastNoteValue = playedNotes[currentNote];
                                midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
                            }
                            
                        }

                    }

                    if (samplesRemaining >= interval)
                    {
                        time = (time + interval) % noteDuration;
                        position += interval;
                    }
                    else
                    {
                        time = (time + samplesRemaining) % noteDuration;
                        position += samplesRemaining;
                    }
                    
                    samplesRemaining -= interval;
                    prevSpeedInHz = speedInHz;
                }
			}
		}

	private:
		float speedInHz;
        float prevSpeedInHz;
		int currentNote, lastNoteValue;
		int time;
		float sampleRate;
		SortedSet<int> sortedNotes;
		Array<int> playedNotes;

		//==============================================================================
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};

#endif // ARPEGGIATOR_H
