#pragma once

#include <optional>

#include "Ray.h"

struct HitResult;

class DielectricMaterial
{
private:
	float refraction;

private:
	static float GetReflectance(float cos, float ref);

public:
	DielectricMaterial(float refraction);

	std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;	
};
