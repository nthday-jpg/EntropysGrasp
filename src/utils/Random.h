#pragma once
#include <random>
#include <limits>

class Random
{
public:
	static void init()
	{
		generator.seed(std::random_device{}());
	}

	// Returns a random float in the range [0, 1)
	static float getFloat()
	{
		return distribution(generator);
	}

	static float getFloat(float min, float max)
	{
		return min + getFloat() * (max - min);
	}

	// Returns a random integer in the given range [min, max]
	static int getInt(int min, int max)
	{
		std::uniform_int_distribution<int> intDist(min, max);
		return intDist(generator);
	}

private:
	static std::mt19937 generator; 
	static std::uniform_real_distribution<float> distribution; 
};