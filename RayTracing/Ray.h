#pragma once

#include <limits>

#include <glm/glm.hpp>

#include "Random.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

public:
	Ray();

	Ray(const glm::vec3& origin, const glm::vec3& direction);

	glm::vec3 At(float t) const;
};