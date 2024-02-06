#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "Sphere.h"
#include "Range.h"
#include "HitResult.h"
#include "Ray.h"

class SphereCollection
{
private:
	std::vector<Sphere> spheres;

public:
	SphereCollection();

	SphereCollection(std::initializer_list<Sphere> spheres);

	template<class Container>
	SphereCollection(const Container& container);

	void Add(Sphere&& sphere);

	std::optional<HitResult> Hit(const Ray& ray, Range range) const;
};

template<class Container>
inline SphereCollection::SphereCollection(const Container& container) :
	spheres(container.cbegin(), container.cend())
{
}
