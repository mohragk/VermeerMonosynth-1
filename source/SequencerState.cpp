/*
  ==============================================================================

    SequencerState.cpp
    Created: 20 Feb 2018 5:00:34pm
    Author:  VLSDESK003

  ==============================================================================
*/

#include "SequencerState.h"



SequencerState::SequencerState() : startTime(Time::getMillisecondCounter())
{
    
}

SequencerState::~SequencerState()
{}


void SequencerState::reset()
{
    internalBuffer.clear();
}


void SequencerState::addNote(const int midiChannel, const int midiNoteNumber, const float velocity,const int noteLengthSamples, const int curTimeSamples)
{
    jassert( midiChannel > 0 && midiChannel <= 16 );
    jassert( isPositiveAndBelow(midiNoteNumber, 128 ) );
    
    
    if ( isPositiveAndBelow ( midiNoteNumber, 128 ) )
    {
       
        
        internalBuffer.addEvent(MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity), curTimeSamples);
        
        const int futureTime = curTimeSamples + noteLengthSamples;
        internalBuffer.addEvent(MidiMessage::noteOff(midiChannel, midiNoteNumber, velocity), futureTime);
    }
    
}


void SequencerState::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    jassert( midiChannel > 0 && midiChannel <= 16 );
    jassert( isPositiveAndBelow(midiNoteNumber, 128 ) );
    
    ScopedLock s1 (lock);
    
    if ( isPositiveAndBelow ( midiNoteNumber, 128 ) )
    {
        int time = Time::getMillisecondCounter() - startTime;
        internalBuffer.addEvent(MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity), time);
        
    }
}


void SequencerState::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    jassert( midiChannel > 0 && midiChannel <= 16 );
    jassert( isPositiveAndBelow(midiNoteNumber, 128 ) );
    
    ScopedLock s1 (lock);
    
    if ( isPositiveAndBelow ( midiNoteNumber, 128 ) )
    {
        
        int time = Time::getMillisecondCounter() - startTime;
        internalBuffer.addEvent(MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity), time);
        
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
    ScopedLock s1 (lock);

	if (midiChannel <= 0)
	{
		for (int channel = 1; channel <= 16; channel++)
			allNotesOff(channel);
	}
	else
	{
		for (int note = 0; note < 128; ++note)
		{
            int now = Time::getMillisecondCounter() - startTime;
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


    ScopedLock s1 (lock);
    
	// add messages to internalBuffer, seems unnecessary..
	//while (i.getNextEvent(message, time))
	//	processMidiEvent(message);

    //i don't want any previous notes dammit
    buffer.clear();

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

	listeners.addIfNotAlreadyThere(listener);
}

void SequencerState::removeListener(SequencerStateListener* const listener)
{

	listeners.removeFirstMatchingValue(listener);
}
