#include "LambertianMaterial.h"
#include "HitResult.h"

LambertianMaterial::LambertianMaterial(Color color, double reflectance) :
    color{ color }, reflectance{ reflectance }
{
}

std::optional<std::pair<Color, Ray>> LambertianMaterial::Scatter(const Ray&, const HitResult& hitResult) const
{
    Vector3d direction = hitResult.normal + Random::Instance.RandomUnitVector3d();

    if (direction.GetLengthSquared() < 1e-8)
    {
        return {};
    }

    Ray outputRay(hitResult.p, direction);
    Color outputColor = reflectance * color;

    return std::pair{ outputColor, outputRay };
}
