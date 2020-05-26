#include "WhiteNoiseGenerator.h"

WhiteNoiseGenerator::WhiteNoiseGenerator()
{
	noiseWidth = 0;
	noiseHeight = 0;
	noise = 0;
}

WhiteNoiseGenerator::~WhiteNoiseGenerator()
{
	delete[] noise;
}

double* WhiteNoiseGenerator::GenerateNoise(int width, int height)
{
	noiseWidth = width;
	noiseHeight = height;
	noise = new double[noiseWidth * noiseHeight];

	for (int y = 0; y < noiseHeight; y++)
	{
		for (int x = 0; x < noiseWidth; x++)
		{
			noise[y * noiseHeight + x] = (rand() % 32768) / 32768.0;
		}
	}

	return noise;
}

double WhiteNoiseGenerator::SmoothNoise(double x, double y)
{
	// get fractional part of x and y
	double fractX = x - int(x);
	double fractY = y - int(y);

	// wrap around
	int x1 = (int(x) + noiseWidth) % noiseWidth;
	int y1 = (int(y) + noiseHeight) % noiseHeight;

	// neighbor values
	int x2 = (x1 + noiseWidth - 1) % noiseWidth;
	int y2 = (y1 + noiseHeight - 1) % noiseHeight;

	// smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX * fractY * noise[y1 * noiseHeight + x1];
	value += (1 - fractX) * fractY * noise[y1 * noiseHeight + x2];
	value += fractX * (1 - fractY) * noise[y2 * noiseHeight + x1];
	value += (1 - fractX) * (1 - fractY) * noise[y2 * noiseHeight + x2];

	return value;
}

double WhiteNoiseGenerator::Turbulence(int x, int y, int size)
{
	double value = 0.0;
	int initialSize = size;

	while (size >= 1)
	{
		value += SmoothNoise(x / (double)size, y / (double)size) * size;
		size /= 2;
	}

	// TODO: Warum dieser konstante Wert?
	return(128.0 * value / initialSize);
}