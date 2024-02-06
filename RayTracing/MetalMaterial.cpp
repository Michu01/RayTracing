#include "MetalMaterial.h"
#include "HitResult.h"

MetalMaterial::MetalMaterial(const Color& color, double fuzz) :
    color{ color }, fuzz{ fuzz }
{
}

std::optional<std::pair<Color, Ray>> MetalMaterial::Scatter(const Ray& inputRay, const HitResult& hitResult) const
{
    Vector3d reflected = inputRay.direction.Reflect(hitResult.normal);
    Ray scatteredRay(hitResult.p, reflected + fuzz * Random::Instance.RandomUnitVector3d());

    if (DotProduct(scatteredRay.direction, hitResult.normal) <= 0.0)
    {
        return {};
    }

    return std::pair{ color, scatteredRay };
}
