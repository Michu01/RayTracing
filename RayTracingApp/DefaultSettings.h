#pragma once

#include "MinMaxDefaultValue.h"
#include "Vector3.h"

namespace DefaultSettings
{
	static constexpr MinMaxValue<size_t> SamplesPerPixel{ 1, 1024, 8 };

	static constexpr Point3d LookFrom{ 0, 0.5, -1 };
};