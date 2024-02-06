#include "Sphere.h"
#include "Ray.h"

Sphere::Sphere(const Point3d& center, double radius, const Material& material) : 
	center{ center }, radius{ radius }, material{ material }
{
}

std::optional<HitResult> Sphere::Hit(const Ray& ray, Range range) const
{
	Vector3d oc = ray.origin - center;
	double a = ray.direction.GetLengthSquared();
	double halfB = DotProduct(oc, ray.direction);
	double c = oc.GetLengthSquared() - radius * radius;
	double discriminant = halfB * halfB - a * c;

	if (discriminant < 0)
	{
		return std::nullopt;
	}

	auto root = (-halfB - sqrt(discriminant)) / a;

	if (!range.Contains(root))
	{
		root = (-halfB + sqrt(discriminant)) / a;

		if (!range.Contains(root))
		{
			return std::nullopt;
		}
	}

	Point3d p = ray.At(root);
	Vector3d normal = (p - center) / radius;
	bool isFrontFace = DotProduct(ray.direction, normal) < 0.0;

	HitResult result;
	result.t = root;
	result.p = p;
	result.isFrontFace = isFrontFace;
	result.normal = isFrontFace ? normal : -normal;
	result.material = &material;

	return result;
}
