#include "SphereCollection.h"

SphereCollection::SphereCollection()
{
}

SphereCollection::SphereCollection(std::initializer_list<Sphere> objects) :
    spheres{ objects }
{
}

void SphereCollection::Add(Sphere&& sphere)
{
    spheres.push_back(sphere);
}

std::optional<HitResult> SphereCollection::Hit(const Ray& ray, Range range) const
{
    std::optional<HitResult> result;
    float tClosest = range.max;

    for (const Sphere& sphere : spheres)
    {
        auto temp = sphere.Hit(ray, Range(range.min, tClosest));

        if (temp)
        {
            result = temp;
            tClosest = result.value().t;
        }
    }

    return result;
}
