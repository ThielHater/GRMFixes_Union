#pragma once

class INoiseGenerator
{
public:
    virtual ~INoiseGenerator() {}
    virtual double* GenerateNoise(int width, int height) = 0;
};

