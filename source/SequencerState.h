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

class SequencerState;

class SequencerStateListener
{
public:

	SequencerStateListener() {};
	virtual~SequencerStateListener() {};

	//pure virtual
	virtual void handleSequencerNoteOn(SequencerState* source, int midiChannel, int midiNoteNumber, float velocity) = 0;
	virtual void handleSequencerNoteOff(SequencerState* source, int midiChannel, int midiNoteNumber, float velocity) = 0;


};

class SequencerState
{
public:
	
	SequencerState();

	~SequencerState();

	void reset();
//	void setSampleRate(float sr) noexcept { sampleRate = sr; };

	//METHODS
	void addNote(const int midiChannel, const int midiNoteNumber, const float velocity, const int noteLengthSamples, const int curTimeSamples);
	void allNotesOff(const int midiChannel, const int curTime);

	void processBuffer(MidiBuffer& buffer, const int startSample, const int numSamples, const bool inject);

	void addListener(SequencerStateListener* const listener);
	void removeListener(SequencerStateListener* const listener);

private:


	//MEMBERS

	//CriticalSection lock;
    
	MidiBuffer internalBuffer;
	Array<SequencerStateListener*> listeners;




	//METHODS

	void noteOnInternal(const int midiChannel, const int midiNoteNumber, float velocity);
	void noteOffInternal(const int midiChannel, const int midiNoteNumber, float velocity);
	

	void processMidiEvent(const MidiMessage& message);

};



#endif // SEQUENCER_STATE_H
