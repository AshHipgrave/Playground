#pragma once

#include <ostream>
#include <cmath>

namespace MathLib
{
	constexpr float PI = 3.1415926535f;

	inline float DegreesToRadians(const float degrees)
	{
		return degrees * PI / 180.0f;
	}

	inline float RadiansToDegrees(const float radians)
	{
		return radians * 180.0f / PI;
	}

	inline float Cot(float value)
	{
		return (1 / std::tan(value));
	}
};