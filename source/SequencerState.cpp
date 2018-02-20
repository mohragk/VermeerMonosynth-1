/*
  ==============================================================================

    SequencerState.cpp
    Created: 20 Feb 2018 5:00:34pm
    Author:  VLSDESK003

  ==============================================================================
*/

#include "SequencerState.h"



SequencerState::SequencerState()
{
    
}

SequencerState::~SequencerState()
{}


void SequencerState::reset()
{
    internalBuffer.clear();
}


void SequencerState::addNote(const int midiChannel, const int midiNoteNumber, float velocity, float noteLength)
{
    jassert( midiChannel >= 0 && midiChannel <= 16 );
    jassert( isPositiveAndBelow(midiNoteNumber, 128 ) );
    
    const ScopedLock s1 (lock); // dunno
    
    if ( isPositiveAndBelow ( midiNoteNumber, 128 ) )
    {
        const int curTime = (int) Time::getMillisecondCounter();
        internalBuffer.addEvent(MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity), curTime);
        
        const int futureTime =  (int) Time::getMillisecondCounter() + noteLength * (int)sampleRate;
        internalBuffer.addEvent(MidiMessage::noteOff(midiChannel, midiNoteNumber, velocity), futureTime);
    }
    
}


void SequencerState::noteOnInternal( const int midiChannel, const int midiNoteNumber, float velocity)
{
    if ( isPositiveAndBelow( midiNoteNumber, 128 ) )
    {
        for (int i = listeners.size(); --i >= 0 ;)
        {
            listeners.getUnchecked(i)->handleSequencerNoteOn( this, midiChannel, midiNoteNumber, velocity );
        }
    }
}

void SequencerState::noteOffInternal (const int midiChannel, const int midiNoteNumber, float velocity)
{

	for (int i = listeners.size(); --i >= 0; )
	{
		listeners.getUnchecked(i)->handleSequencerNoteOff(this, midiChannel, midiNoteNumber, velocity);
	}
    
}

void SequencerState::allNotesOff(const int midiChannel)
{
	const ScopedLock s1(lock); //dunno


	if (midiChannel <= 0)
	{
		for (int channel = 1; channel < 128; channel++)
			allNotesOff(channel);
	}
	else
	{
		for (int note = 0; note < 128; ++note)
		{
			int now = (int) Time::getMillisecondCounter();
			internalBuffer.addEvent(MidiMessage::noteOff(midiChannel, note, 0.0f), now);
		}
	}


}

void SequencerState::processMidiEvent(const MidiMessage& message)
{
	if (message.isNoteOn())
	{
		noteOnInternal ( message.getChannel(), message.getNoteNumber(), message.getFloatVelocity() );
	}
	else if (message.isNoteOff())
	{
		noteOffInternal( message.getChannel(), message.getNoteNumber(), message.getFloatVelocity() );
	}
	else if (message.isAllNotesOff())
	{
		for (int note = 0; note < 128; ++note)
		{
			noteOffInternal(message.getChannel(), note, 0.0f);
		}
	}
}


void SequencerState::processBuffer(MidiBuffer& buffer, const int startSample, const int numSamples, const bool inject)
{
	MidiBuffer::Iterator i(buffer);
	MidiMessage message;
	int time;

	const ScopedLock s1(lock);

	// add messages to internalBuffer
	while (i.getNextEvent(message, time))
		processMidiEvent(message);


	if (inject)
	{
		// search for first and last event in internalBuffer
		MidiBuffer::Iterator i2(internalBuffer);
		const int firstEventTime = internalBuffer.getFirstEventTime();
		const int lastEventTime = internalBuffer.getLastEventTime();
		const double scaleFactor = numSamples / (double)(lastEventTime + 1 - firstEventTime);

		// 
		while (i2.getNextEvent(message, time))
		{
			const int pos = jlimit(0, numSamples - 1, roundToInt((time - firstEventTime) * scaleFactor));
			buffer.addEvent(message, startSample + pos);
		}

		
	}
	internalBuffer.clear();
}



void SequencerState::addListener(SequencerStateListener* const listener)
{
	const ScopedLock s1(lock);

	listeners.addIfNotAlreadyThere(listener);
}

void SequencerState::removeListener(SequencerStateListener* const listener)
{
	const ScopedLock s1(lock);

	listeners.removeFirstMatchingValue(listener);
}