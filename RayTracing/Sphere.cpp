#include "Sphere.h"
#include "Ray.h"

Sphere::Sphere() :
	center{}, radius{}
{
}

Sphere::Sphere(const glm::vec3& center, float radius, const Material& material) :
	center{ center }, radius{ radius }, material{ material }
{
}

std::optional<HitResult> Sphere::Hit(const Ray& ray, Range range) const
{
	glm::vec3 oc = ray.origin - center;
	float a = glm::dot(ray.direction, ray.direction);
	float halfB = glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = halfB * halfB - a * c;

	if (discriminant < 0)
	{
		return std::nullopt;
	}

	float root = (-halfB - sqrt(discriminant)) / a;

	if (!range.Contains(root))
	{
		return std::nullopt;
	}

	glm::vec3 p = ray.At(root);
	glm::vec3 normal = (p - center) / radius;
	bool isFrontFace = glm::dot(ray.direction, normal) <= 0.0;

	HitResult result;
	result.t = root;
	result.p = p;
	result.isFrontFace = isFrontFace;
	result.normal = isFrontFace ? normal : -normal;
	result.material = &material;

	return result;
}
