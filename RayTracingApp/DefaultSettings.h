#pragma once

#include <glm/glm.hpp>

#include "MinMaxDefaultValue.h"

namespace DefaultSettings
{
	static constexpr MinMaxValue<size_t> SamplesPerPixel{ 1, 1024, 8 };

	static constexpr glm::vec3 LookFrom{ 0, 0.5, -1 };
};