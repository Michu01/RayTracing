#pragma once

#include <glm/glm.hpp>

#include "Size.h"
#include "Vector3.h"
#include "Ray.h"
#include "Random.h"
#include "DegreesToRadians.h"

class Camera
{
private:
	double vFov;
	double aspectRatio;

	Point3d lookFrom;
	Point3d lookAt;
	Vector3d up{ 0, 1, 0 };

public:
	Camera();

	Camera(double vFov, double aspectRatio, Point3d lookFrom, Point3d lookAt);

	void SetLookFrom(const Point3d& lookFrom);

	void SetLookAt(const Point3d& lookAt);

	void SetUp(const Vector3d& up);

	void SetVFov(double vFov);

	void SetAspectRatio(double aspectRatio);

	Ray CreateRay(SizeU size, size_t row, size_t column) const;
};
