#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Point3d& origin, const Vector3d& direction) :
	origin{ origin }, direction{ direction }
{
}

Point3d Ray::At(double t) const
{
	return origin + t * direction;
}