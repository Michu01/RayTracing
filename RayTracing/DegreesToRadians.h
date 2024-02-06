#pragma once

#include <numbers>

inline double DegreesToRadians(double degrees)
{
	return degrees * std::numbers::pi / 180.0;
}