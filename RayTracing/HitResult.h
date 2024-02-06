#pragma once

#include <glm/glm.hpp>

#include "Materials.h"

struct HitResult
{
	glm::vec3 p;
	glm::vec3 normal;
	float t{};
	bool isFrontFace{};
	const Material* material;
};