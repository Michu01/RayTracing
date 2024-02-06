#pragma once

#include <optional>

#include "Ray.h"

struct HitResult;

class LambertianMaterial
{
private:
	glm::vec3 color;
	float reflectance;

public:
	LambertianMaterial(const glm::vec3& color, float reflectance);

	std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;
};

