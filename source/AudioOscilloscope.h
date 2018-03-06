/*
  ==============================================================================

    AudioOscilloscope.h
    Created: 6 Mar 2018 7:24:06pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef AUDIO_OSCILLOSCOPE_H
#define AUDIO_OSCILLOSCOPE_H

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AudioOscilloscope    : public Component, public Timer
{
public:
    AudioOscilloscope();
    ~AudioOscilloscope();
    
    
    void processBlock (const float* inputChannelData,
                       int numSamples);
                       
    
                       
    /**	Clears the internal buffers.
        It is a good idea to call this when an audio device is started or stopped
        to avoid clicks on the scope.
	 */
	void clear();
	
	/** Adjusts the vertical zoom of the scope.
        0 = nothing, 1 = normal, > 1 = zoomed.
	 */
	void setVerticalZoom (float newVerticalZoomFactor)      {   verticalZoomFactor = newVerticalZoomFactor;     }
    
	/**	Adjusts the horizontal zoom of the scope.
        The lower this value is the more zoomed in and detailed the image will be.
        However, as more information is passing the scope will move quicker and be
        difficult to see. Consider using Component::createComponentSnapshot() to
        capture an image of the scope.
	 */
	void setHorizontalZoom (float newHorizontalZoomFactor)  {	horizontalZoomFactor = newHorizontalZoomFactor; }
    
	/** Sets the background colour of the scope. */
	void setBackgroundColour (Colour newBackgroundColour)	{	backgroundColour = newBackgroundColour;         }
	
	/** Sets the trace colour of the scope. */
	void setTraceColour (Colour newTraceColour)             {	traceColour = newTraceColour;                   }
    

    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback();
    
    void addSample(const float sample);

private:
    HeapBlock<float> circularBufferMax, circularBufferMin;
	int bufferSizeMask;
    float currentMax, currentMin;
    int volatile bufferPos, lastBufferPos, bufferSize, numSamplesIn;
	float bufferLastMax, bufferLastMin;
	
    Image waveformImage;
    
	float verticalZoomFactor, horizontalZoomFactor;
	Colour backgroundColour, traceColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioOscilloscope)
};

#endif // AUDIO_OSCILLOSCOPE_H