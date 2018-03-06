/*
  ==============================================================================

    TriggeredScope.h
    Created: 6 Mar 2018 8:54:14pm
    Author:  VLSDESK003

  ==============================================================================
*/

#ifndef TRIGGERED_SCOPE_H
#define TRIGGERED_SCOPE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "FifoBuffer.h"

//==============================================================================
/*
*/
class TriggeredScope    : public Component, public Timer, public TimeSliceClient
{
public:

    /** The enum to use when setting the trace trigger mode.
     */
    enum TriggerMode
    {
        None,   /**<< The trace will just refresh at a constant rate. */
        Up,     /**<< The start of the trace will be a rising edge. */
        Down    /**<< The start of the trace will be a falling edge. */
    };
    
    TriggeredScope(TimeSliceThread* backgroundThreadToUse = nullptr);
    ~TriggeredScope();
    
    /** Sets the number of samples represented by each pixel on the scope.
        Setting this to a low number will give a very zoomed in display, a high
        number zoom out.
     */
    void setNumSamplesPerPixel (int newNumSamplesPerPixel);
    
    /** Sets the vertical zoom facotr of the display.
     */
    void setVerticalZoomFactor (float newVerticalZoomFactor);
    
    /** Sets the type of change that will trigger a trace.
     */
    void setTriggerMode (TriggerMode newTriggerMode);
    
    /** Adds a block of samples to the scope.
        
        Simply call this from your audio callback or similar to render the scope.
        This is a very quick method as it only takes a copy of the samples. All the
        processing and image rendering is performed on a backgroudn thread.
     */
    void addSamples (const float* samples, int numSamples);

    void paint (Graphics&) override;
    void resized() override;
    
    /** @internal */
    void timerCallback();
    
    /** @internal */
    int useTimeSlice();

private:
    OptionalScopedPointer<TimeSliceThread> backgroundThreadToUse; //MIGHT MAKE THIS std
    
    int numSamplesPerPixel;
    float verticalZoomFactor;
    TriggerMode triggerMode;
    
    int numLeftToAverage;

    int bufferSize, bufferWritePos;
    HeapBlock<float> minBuffer, maxBuffer;
    
    float currentMax, currentMin;
    FifoBuffer<float> samplesToProcess;
    HeapBlock<float> tempProcessingBlock;
    bool needToUpdate;

    Image image;
    bool needToRepaint;
    CriticalSection imageLock;

    //==============================================================================
    void processPendingSamples();
    void renderImage();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggeredScope)
};

#endif // TRIGGERED_SCOPE