#pragma once

#include "Vector3.h"
#include "Color.h"
#include "Random.h"

#include <limits>

struct Ray
{
	Point3d origin;
	Vector3d direction;

public:
	Ray();

	Ray(const Point3d& origin, const Vector3d& direction);

	Point3d At(double t) const;
};