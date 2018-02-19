//
//  ADRS.h
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

#ifndef ADRS_h
#define ADRS_h


class ADSR {
public:
    ADSR(void);
    ~ADSR(void);
    enum envState {
        env_idle = 0,
        env_attack,
        env_decay,
        env_sustain,
        env_release
    };

    double process(void);
    void processSample(double* samples, int numSamples);
    double getOutput(void);
    int getState(void);
    void gate(int on);
    void setSampleRate(double sr);
    void setAttackRate(double rate);
    void setDecayRate(double rate);
    void setReleaseRate(double rate);
    void setSustainLevel(double level);
    void setTargetRatioA(double targetRatio);
    void setTargetRatioDR(double targetRatio);
    void setState(envState newState);
    void reset(void);
    void resetToAttack(void);
    bool getGate(){ return gateOn;};

    
protected:
    int state;
    double sampleRate;
    double output;
    double attackRate;
    double decayRate;
    double releaseRate;
    double attackCoef;
    double decayCoef;
    double releaseCoef;
    double sustainLevel;
    double targetRatioA;
    double targetRatioDR;
    double attackBase;
    double decayBase;
    double releaseBase;
    bool gateOn;
    
    double calcCoef(double rate, double targetRatio);
};

inline double ADSR::process() {
    switch (state) {
        case env_idle:
            break;
        case env_attack:
            output = attackBase + output * attackCoef;
            if (output >= 1.0) {
                output = 1.0;
                state = env_decay;
            }
            break;
        case env_decay:
            output = decayBase + output * decayCoef;
            if (output <= sustainLevel) {
                output = sustainLevel;
                state = env_sustain;
            }
            break;
        case env_sustain:
            break;
        case env_release:
            output = releaseBase + output * releaseCoef;
            if (output <= 0.0) {
                output = 0.0;
                state = env_idle;
            }
    }
    return output;
}

inline void ADSR::processSample(double *sample, int numSamples)
{
    for(int i = 0; i < numSamples; i++)
    {
        switch (state)
        {
            case env_idle:
                break;
            case env_attack:
                sample[i] = attackBase + sample[i] * attackCoef;
                if (sample[i] >= 1.0) {
                    sample[i] = 1.0;
                    state = env_decay;
                }
                break;
            case env_decay:
                sample[i] = decayBase + sample[i] * decayCoef;
                if (sample[i] <= sustainLevel) {
                    output = sustainLevel;
                    state = env_sustain;
                }
                break;
            case env_sustain:
                break;
            case env_release:
                sample[i] = releaseBase + sample[i] * releaseCoef;
                if (output <= 0.0) {
                    output = 0.0;
                    state = env_idle;
                }
        }
    }

}

inline void ADSR::gate(int gate) {
    if (gate)
    {
        state = env_attack;
        gateOn = true;
    }
    else if (state != env_idle)
    {
        state = env_release;
        gateOn = false;
    }
}

inline void ADSR::setState(envState newState)
{
    state = newState;
}

inline int ADSR::getState() {
    return state;
}

inline void ADSR::reset() {
    state = env_idle;
    output = 0.0;
}

inline void ADSR::resetToAttack() {
    output = 0.0;
    state = env_attack;
}

inline double ADSR::getOutput() {
    return output;
}

#endif
