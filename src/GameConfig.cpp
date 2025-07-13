#include "GameConfig.h"

float GameConfig::clamp(float value, float min, float max) const
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
