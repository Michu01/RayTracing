#pragma once

#include "Vector3.h"
#include "Materials.h"

struct HitResult
{
	Point3d p;
	Vector3d normal;
	double t{};
	bool isFrontFace{};
	const Material* material;
};