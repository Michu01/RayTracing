#pragma once

#include <limits>

struct Range
{
	float min{};
	float max{};

	Range(float min, float max);

	bool Contains(float value) const;
};
