#pragma once

#include <optional>

#include "Ray.h"

struct HitResult;

class DielectricMaterial
{
private:
	double refraction;

private:
	static double GetReflectance(double cos, double ref);

public:
	DielectricMaterial(double refraction);

	std::optional<std::pair<Color, Ray>> Scatter(const Ray& inputRay, const HitResult& hitResult) const;	
};
