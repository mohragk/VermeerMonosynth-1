//
//  NoiseGen.h
//  Vermeer Monosynth
//
//  Created by Sander Vermeer on 29-07-17.
//
//

#ifndef NoiseGen_h
#define NoiseGen_h

#include <random>


struct WhiteNoiseSource
{
    WhiteNoiseSource() : dist(-1, 1) {}
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
};

// Full spectrum noise
struct WhiteNoise : public WhiteNoiseSource
{
	float operator()() { return dist(engine); }
};


struct NoiseGenerator
{
    float produce()
    {
      WhiteNoise n;
      return n();
    }
};

#endif /* NoiseGen_h */
