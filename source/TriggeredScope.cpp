/*
  ==============================================================================

    TriggeredScope.cpp
    Created: 6 Mar 2018 8:54:14pm
    Author:  VLSDESK003

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TriggeredScope.h"

//==============================================================================
TriggeredScope::TriggeredScope (TimeSliceThread* backgroundThreadToUse_)
    : backgroundThreadToUse (backgroundThreadToUse_, backgroundThreadToUse_ == nullptr ? true : false),
      numSamplesPerPixel (64), verticalZoomFactor (1.0f), triggerMode (Up), numLeftToAverage (numSamplesPerPixel),
      bufferSize (2048), bufferWritePos (0),
      minBuffer (bufferSize), maxBuffer (bufferSize),
      currentMax (-1.0f), currentMin (1.0f),
      samplesToProcess (32768),
      tempProcessingBlock (32768),
      needToUpdate (false),
      needToRepaint (true)
{
    const ScopedLock sl (imageLock);
    image = Image (Image::ARGB, jmax (1, getWidth()), jmax (1, getHeight()), true);
    Graphics g (image);
    g.fillAll (Colours::black);
    
    if (backgroundThreadToUse == nullptr)
    {
        OptionalScopedPointer<TimeSliceThread> newThread (new TimeSliceThread ("Triggered Scope Rendering Thread"), true);
        backgroundThreadToUse = newThread;
        backgroundThreadToUse->startThread (1);
    }
        
    backgroundThreadToUse->addTimeSliceClient (this);
    
    minBuffer.clear (bufferSize);
    maxBuffer.clear (bufferSize);
     
    startTimerHz (30);
}

TriggeredScope::~TriggeredScope()
{
    const ScopedLock sl (imageLock);

    stopTimer();
    
    backgroundThreadToUse->removeTimeSliceClient (this);

    if (backgroundThreadToUse.willDeleteObject())
        backgroundThreadToUse->stopThread (500);
}
void TriggeredScope::setNumSamplesPerPixel (int newNumSamplesPerPixel)
{
    numSamplesPerPixel = newNumSamplesPerPixel;
}

void TriggeredScope::setVerticalZoomFactor (float newVerticalZoomFactor)
{
    verticalZoomFactor = newVerticalZoomFactor;
}

void TriggeredScope::setTriggerMode (TriggerMode newTriggerMode)
{
    
}

void TriggeredScope::addSamples (const float* samples, int numSamples)
{
    // if we don't have enough space in the fifo, clear out some old samples
    const int numFreeInBuffer = samplesToProcess.getNumFree();
    if (numFreeInBuffer < numSamples)
        samplesToProcess.removeSamples (numFreeInBuffer);

    samplesToProcess.writeSamples (samples, numSamples);
    
    numSamplesExt = numSamples;
    
    needToUpdate = true;
}

//==============================================================================
void TriggeredScope::resized()
{
    const ScopedLock sl (imageLock);

    image = Image (Image::ARGB, jmax (1, getWidth()*2), jmax (1, getHeight()*2), false);

    Graphics g (image);
    
    g.fillAll (backGroundColour);
   
    needToRepaint = true;
}

void TriggeredScope::paint (Graphics& g)
{
    const ScopedLock sl (imageLock);
	
    g.setImageResamplingQuality(Graphics::ResamplingQuality::highResamplingQuality);
    g.drawImageWithin(image, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
    //g.drawImageAt (image, 0, 0);
}

void TriggeredScope::timerCallback()
{
    if (needToRepaint)
        repaint();
}

int TriggeredScope::useTimeSlice()
{
    if (needToUpdate)
    {
        processPendingSamples();
        renderImage();
        
        needToUpdate = false;
    }

    
    return 10;
}

void TriggeredScope::setNewTriggerPoint(double newPos)
{
    overriddenBufferReadPos = newPos;
}

//==============================================================================
void TriggeredScope::processPendingSamples()
{
    int numSamples = samplesToProcess.getNumAvailable();
    samplesToProcess.readSamples (tempProcessingBlock, numSamples);
    float* samples = tempProcessingBlock.getData();
    
    while (--numSamples >= 0)
    {
        const float currentSample = *samples++;
        
        if (currentSample < currentMin)
            currentMin = currentSample;
        if (currentSample > currentMax)
            currentMax = currentSample;
        
        if (--numLeftToAverage == 0)
        {
            minBuffer[bufferWritePos] = currentMin;
            maxBuffer[bufferWritePos] = currentMax;

            currentMax = -1.0f;
            currentMin = 1.0f;
            
            
            numLeftToAverage = numSamplesPerPixel;
            trueBufferSize = bufferSize / numSamplesPerPixel;
            ++bufferWritePos %= bufferSize;
        }
    }
}

int val = 0;

void TriggeredScope::mouseDown (const MouseEvent &event)
{
    val++;
    
    if (val >= numTriggerModes)
        val = 0;
    
    triggerMode = (TriggerMode) val;
}

void TriggeredScope::renderImage()
{
    const ScopedLock sl (imageLock);
    
    Graphics g (image);
    
    g.fillAll (backGroundColour);
    
    
    const int w = image.getWidth();
    const int h = image.getHeight();
    
    int bufferReadPos = bufferWritePos - w;
    if (bufferReadPos < 0 )
        bufferReadPos += bufferSize;
    
    {
        int posToTest = bufferReadPos;
        int numToSearch = bufferSize;
        while (--numToSearch >= 0)
        {
            int prevPosToTest = posToTest - 1;
            if (prevPosToTest < 0)
                prevPosToTest += bufferSize;
            
            switch(triggerMode)
            {
                case Up:
                    if (maxBuffer[prevPosToTest] <= 0.0f && maxBuffer[posToTest] > 0.0f)
                        bufferReadPos = posToTest;
                    break;
                    
                case Phase:
                {
                    float scaler = (float)trueBufferSize / (float)numSamplesExt;
                    bufferReadPos =  overriddenBufferReadPos * scaler ;
                    break;
                }
                    
                default:
                    break;
            }
            
            
            
            if (--posToTest < 0)
                posToTest += bufferSize;
        }
    }
    
    float currentX = 0;


	Path oscilloLine;
	float lineW = 1.7f;

	oscilloLine.startNewSubPath(0, h / 2.0f);
    
    while (currentX < w)
    {
        ++bufferReadPos;
        if (bufferReadPos == bufferSize)
            bufferReadPos = 0;
        
        const float currentY = (1.0f - (0.5f + (0.5f * verticalZoomFactor * maxBuffer[bufferReadPos]))) * h;

		oscilloLine.lineTo(currentX, currentY);
        ++currentX;
    }

	g.setColour(Colours::white);
	g.strokePath(oscilloLine, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::butt));

	Colour fade = backGroundColour.withAlpha(1.0f);
	Colour unfade = backGroundColour.withAlpha(0.0f);

	int proportion = 4;
	Rectangle<int> fadeInBounds = image.getBounds().removeFromLeft(w / proportion);
	Rectangle<int> fadeOutBounds = image.getBounds().removeFromRight(w / proportion);

	ColourGradient fadeIn = ColourGradient::horizontal(fade, unfade,fadeInBounds );

	g.setGradientFill(fadeIn);
	g.fillAll();

	ColourGradient fadeOut = ColourGradient::horizontal(unfade, fade, fadeOutBounds);
	g.setGradientFill(fadeOut);
	g.fillAll();
	

    needToRepaint = true;
}
