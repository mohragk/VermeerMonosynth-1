# Vermeer Monosynth-1

#### Info
This is a software synthesizer, made to be used as a VST plug-in or standalone application on your platform of choice. It's realised in C++ using the [Juce framework](https://juce.com/). 
It features 3 oscillators, 3 different filter models, 3 envelope generators, an LFO and a simple yet powerfull sequencer.

### How to use this project
To use this synth it needs to be compiled using Visual Studio or XCode. To create the necessary solution or XCode project files, open the VermeerMonoSynthTest.jucer file using Juce's [Projucer](https://shop.juce.com/get-juce). 
Setup your desired target platforms and export from the Projucer to generate the Visual Studio solution/XCode project. For more information, check the Juce website.

### Caution!
This project was made using Juce 5.4.1 and some functions are deprecated in newer versions of Juce. You should build this using version 5.4.1 or older. You van find [older versions of Juce](https://github.com/juce-framework/JUCE/releases) over [here](https://github.com/juce-framework/JUCE/releases).


