#pragma once

#include <optional>

#include "Ray.h"

struct HitResult;

class MetalMaterial
{
private:
	glm::vec3 color;
	float fuzz;

public:
	MetalMaterial();

	MetalMaterial(const glm::vec3& color, float fuzz);

	std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;
};
