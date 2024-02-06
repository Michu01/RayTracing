#pragma once

#include <optional>

#include "Color.h"
#include "Ray.h"

struct HitResult;

class LambertianMaterial
{
private:
	Color color;
	double reflectance;

public:
	LambertianMaterial(Color color, double reflectance);

	std::optional<std::pair<Color, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;
};

