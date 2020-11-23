#pragma once

#include <cmath>

#define PI 3.1415926

#define AMPLITUDE 25

struct PerlinNoise
{

	static float getNoise(int seed, int x, int y)
	{
		std::srand(seed + x * 827000 * y + 631000);
		return (std::rand() % (AMPLITUDE + 1 - (-AMPLITUDE))) + (-AMPLITUDE);
	}

	static float getSmoothNoise(int seed, int x, int y)
	{
		float corners = (getNoise(seed, x - 1, y - 1) + getNoise(seed, x + 1, y - 1) + getNoise(seed, - 1, y + 1)
			+ getNoise(seed, x + 1, y + 1)) / 16.0f;
		float sides = (getNoise(seed, x - 1, y) + getNoise(seed, x + 1, y) + getNoise(seed, x, y - 1)
			+ getNoise(seed, x, y + 1)) / 8.0f;
		float center = getNoise(seed, x, y) / 4.0f;
		return corners + sides + center;
	}

	static float interpolate(float a, float b, float blend) 
	{
		double theta = blend * PI;
		float f = (float)(1.0f - std::cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}

	static float getInterpolatedNoise(int seed, float x, float y)
	{
		int intX = (int)x;
		int intY = (int)y;
		float fracX = x - intX;
		float fracY = y - intY;

		float v1 = getSmoothNoise(seed, intX, intY);
		float v2 = getSmoothNoise(seed, intX + 1, intY);
		float v3 = getSmoothNoise(seed, intX, intY + 1);
		float v4 = getSmoothNoise(seed, intX + 1, intY + 1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);
		return interpolate(i1, i2, fracY);
	}

	static float getPerlinNoise(int seed, int x, int y)
	{
		float total = getInterpolatedNoise(seed, x / 48.0f, y / 48.0f) * 1.50f;
		total += getInterpolatedNoise(seed, x / 12.0f, y / 12.0f) * 1.15f;
		total += getInterpolatedNoise(seed, x / 8.0f, y / 8.0f) * .1f;
		return total;
	}
};