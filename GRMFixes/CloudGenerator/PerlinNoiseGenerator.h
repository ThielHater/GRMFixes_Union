#pragma once
#include "INoiseGenerator.h"
#include "FastNoise.h"
#include <math.h>
#include <cstdlib>

class PerlinNoiseGenerator : public INoiseGenerator
{
private:
    double* noise;

public:
    PerlinNoiseGenerator();
    ~PerlinNoiseGenerator();
    virtual double* GenerateNoise(int width, int height);
};

