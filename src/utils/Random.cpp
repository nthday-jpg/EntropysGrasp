#include "Random.h"

// Static member definitions
std::mt19937 Random::generator;
std::uniform_real_distribution<float> Random::distribution(0.0f, 1.0f);