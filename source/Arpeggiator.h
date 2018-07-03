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
			notes.clear();
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
		void process(AudioBuffer<FloatType>& buffer, MidiBuffer& midi, bool useArpeggiator)
		{
			if (useArpeggiator)
			{
				auto numSamples = buffer.getNumSamples();

				auto noteDuration = static_cast<int> ( std::round( sampleRate / speedInHz) ) ;

				MidiMessage msg;
				int ignore;

				for (MidiBuffer::Iterator it(midi); it.getNextEvent(msg, ignore);)
				{
					if (msg.isNoteOn())  notes.add(msg.getNoteNumber());
					else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
				}

				midi.clear();

				if ((time + numSamples) >= noteDuration)
				{
					auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples - 1));

					if (lastNoteValue > 0)
					{
						midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
						lastNoteValue = -1;
					}

					if (notes.size() > 0)
					{
						currentNote = (currentNote + 1) % notes.size();
						lastNoteValue = notes[currentNote];
						midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
					}

				}

				time = (time + numSamples) % noteDuration;
			}
			


		}

	private:
		float speedInHz; //CHANGE
		int currentNote, lastNoteValue;
		int time;
		float sampleRate;
		SortedSet<int> notes;

		//==============================================================================
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};

#endif // ARPEGGIATOR_H
