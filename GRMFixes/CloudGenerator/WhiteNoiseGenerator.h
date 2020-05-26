#pragma once
#include "INoiseGenerator.h"
#include <math.h>
#include <cstdlib>

class WhiteNoiseGenerator : public INoiseGenerator
{
private:
    int noiseWidth;
    int noiseHeight;
    double* noise;
    double SmoothNoise(double x, double y);

public:
    WhiteNoiseGenerator();
    ~WhiteNoiseGenerator();
    virtual double* GenerateNoise(int width, int height);
    double Turbulence(int x, int y, int size);
};

