#include "PerlinNoiseGenerator.h"

PerlinNoiseGenerator::PerlinNoiseGenerator()
{
	noise = 0;
}

PerlinNoiseGenerator::~PerlinNoiseGenerator()
{
	delete[] noise;
}

double* PerlinNoiseGenerator::GenerateNoise(int width, int height)
{
	if (noise != 0)
		delete[] noise;

	noise = new double[width * height];

	FastNoise fastNoise;
	const int period = 1;
	const float freq = period / (float)width;
	fastNoise.SetNoiseType(FastNoise::PerlinFractal);
	fastNoise.SetFrequency(freq);
	fastNoise.SetInterp(FastNoise::Linear);
	fastNoise.SetFractalType(FastNoise::FBM);
	fastNoise.SetFractalOctaves(7);
	fastNoise.SetFractalLacunarity(2.0);
	fastNoise.SetFractalGain(0.75);
	fastNoise.SetSeed(rand());

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			noise[y * height + x] = fabs(fastNoise.GetPeriodicNoise((float)x, (float)y, period, period));
		}
	}

	return noise;
}