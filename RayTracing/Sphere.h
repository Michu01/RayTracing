#pragma once

#include "Range.h"
#include "Ray.h"
#include "Materials.h"
#include "HitResult.h"

struct Sphere
{
	glm::vec3 center;
	float radius;
	Material material;

	Sphere();

	Sphere(const glm::vec3& center, float radius, const Material& material);

	std::optional<HitResult> Hit(const Ray& ray, Range range) const;
};
