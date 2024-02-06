#pragma once

#include <random>
#include <numbers>

#include "Vector3.h"
#include "Color.h"

class Random
{
private:
	std::mt19937 generator{ std::random_device{}() };

public:
	static Random Instance;

public:
	double RandomDouble(std::uniform_real_distribution<double> distribution);

	Vector3d RandomUnitVector3d();

	Vector3d RandomUnitVector3dInHemisphere(const Vector3d& normal);

	Color RandomColor();
};

