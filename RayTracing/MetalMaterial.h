#pragma once

#include <optional>

#include "Color.h"
#include "Ray.h"

struct HitResult;

class MetalMaterial
{
private:
	Color color;
	double fuzz;

public:
	MetalMaterial(const Color& color, double fuzz);

	std::optional<std::pair<Color, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;
};
