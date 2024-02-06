#pragma once

#include "Range.h"
#include "Ray.h"
#include "Materials.h"
#include "HitResult.h"

class Sphere
{
private:
	Point3d center;
	double radius;
	Material material;

public:
	Sphere(const Point3d& center, double radius, const Material& material);

	std::optional<HitResult> Hit(const Ray& ray, Range range) const;
};
