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
    
    
    void noteOn(const int midiChannel, const int midiNoteNumber, const float velocity);
    void noteOff(const int midiChannel, const int midiNoteNumber, const float velocity);
	void allNotesOff(const int midiChannel);

	void processBuffer(MidiBuffer& buffer, const int startSample, const int numSamples, const bool insertPreviousNotes);
    
    
	void addListener(SequencerStateListener* const listener);
	void removeListener(SequencerStateListener* const listener);
    
    
    
    
    

private:


	//MEMBERS

	CriticalSection lock;
    
    
	MidiBuffer internalBuffer;
	Array<SequencerStateListener*> listeners;


    int startTime;


	//METHODS

	void noteOnInternal(const int midiChannel, const int midiNoteNumber, float velocity);
	void noteOffInternal(const int midiChannel, const int midiNoteNumber, float velocity);
	

	void processMidiEvent(const MidiMessage& message);

    JUCE_LEAK_DETECTOR (SequencerState)
};



#endif // SEQUENCER_STATE_H
