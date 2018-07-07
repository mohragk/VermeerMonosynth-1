//
//  ADSR.cpp
//
//  Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//
//  1.01  2016-01-02  njr   added calcCoef to SetTargetRatio functions that were in the ADSR widget but missing in this code
//  1.02  2017-01-04  njr   in calcCoef, checked for rate 0, to support non-IEEE compliant compilers
//


//  Adjust the curves of the Attack, or Decay and Release segments, from the initial default values
//  (small number such as 0.0001 to 0.01 for mostly-exponential, large numbers like 100 for virtually linear)

#include "ADSR.h"
#include <math.h>


ADSR::ADSR(void) {
    reset();
    setAttackRate(0);
    setDecayRate(0);
    setReleaseRate(0);
    setSustainLevel(1.0);
    setTargetRatioA(0.01);
    setTargetRatioDR(0.0001);
}

ADSR::~ADSR(void) {
}

void ADSR::setSampleRate(double sr) {
    sampleRate = sr;
}

void ADSR::setAttackRate(double rate) {
    attackRate = rate * sampleRate;
    attackCoef = calcCoef(attackRate, targetRatioA);
    attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
}

void ADSR::setDecayRate(double rate) {
    decayRate = rate * sampleRate;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

void ADSR::setReleaseRate(double rate) {
    releaseRate = rate * sampleRate;
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    releaseBase = -targetRatioDR * (1.0 - releaseCoef);
}

double ADSR::calcCoef(double rate, double targetRatio) {
    return (rate <= 0) ? 0.0 : exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

void ADSR::setSustainLevel(double level) {
    sustainLevel = level;
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

void ADSR::setTargetRatioA(double targetRatio) {
    
    if (old_targetRatioA != targetRatio)
    {
        if (targetRatio < 0.000000001)
            targetRatio = 0.000000001;  // -180 dB
        targetRatioA = targetRatio;
        attackCoef = calcCoef(attackRate, targetRatioA);
        attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
        
        old_targetRatioA = targetRatioA;
    }
    
    
}

void ADSR::setTargetRatioDR(double targetRatio) {
    
    if (old_targetRatioDR != targetRatio)
    {
        if (targetRatio < 0.000000001)
            targetRatio = 0.000000001;  // -180 dB
        targetRatioDR = targetRatio;
        decayCoef = calcCoef(decayRate, targetRatioDR);
        releaseCoef = calcCoef(releaseRate, targetRatioDR);
        decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
        releaseBase = -targetRatioDR * (1.0 - releaseCoef);
        
        old_targetRatioDR = targetRatioDR;
    }
    
    
}
