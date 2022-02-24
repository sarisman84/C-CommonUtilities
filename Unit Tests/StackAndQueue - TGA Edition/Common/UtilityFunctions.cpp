#include "stdafx.h"
#include "UtilityFunctions.hpp"

namespace TestUtility
{
	std::mt19937 rEngine(6);

	float GetRandomFloat()
	{
		std::uniform_real_distribution<float> rnd(-100000.0f, 100000.0f);
		return rnd(rEngine);
	}

	float GetRandomFloat(float aMin, float aMax)
	{
		std::uniform_real_distribution<float> rnd(aMin, aMax);
		return rnd(rEngine);
	}

	int GetRandomInt()
	{
		std::uniform_int_distribution<int> rnd(-100000, 100000);
		return rnd(rEngine);
	}

	int GetRandomInt(int aMin, int aMax)
	{
		std::uniform_int_distribution<int> rnd(aMin, aMax);
		return rnd(rEngine);
	}
}