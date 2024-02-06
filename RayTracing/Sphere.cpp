#include "Sphere.h"
#include "Ray.h"

Sphere::Sphere(const glm::vec3& center, float radius, const Material& material) : 
	center{ center }, radius{ radius }, material{ material }
{
}

std::optional<HitResult> Sphere::Hit(const Ray& ray, Range range) const
{
	glm::vec3 oc = ray.origin - center;
	float a = pow(glm::length(ray.direction), 2.0f);
	float halfB = glm::dot(oc, ray.direction);
	float c = pow(glm::length(oc), 2.0f) - radius * radius;
	float discriminant = halfB * halfB - a * c;

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

	glm::vec3 p = ray.At(root);
	glm::vec3 normal = (p - center) / radius;
	bool isFrontFace = glm::dot(ray.direction, normal) < 0.0;

	HitResult result;
	result.t = root;
	result.p = p;
	result.isFrontFace = isFrontFace;
	result.normal = isFrontFace ? normal : -normal;
	result.material = &material;

	return result;
}
