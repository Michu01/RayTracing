#include "MetalMaterial.h"
#include "HitResult.h"

MetalMaterial::MetalMaterial() :
    color{}, fuzz{}
{
}

MetalMaterial::MetalMaterial(const glm::vec3& color, float fuzz) :
    color{ color }, fuzz{ fuzz }
{
}

std::optional<std::pair<glm::vec3, Ray>> MetalMaterial::Scatter(const Ray& inputRay, const HitResult& hitResult) const
{
    glm::vec3 reflected = glm::reflect(inputRay.direction, hitResult.normal);
    Ray scatteredRay(hitResult.p, reflected + fuzz * Random::Instance.UnitVec3());

    if (glm::dot(scatteredRay.direction, hitResult.normal) <= 0.0)
    {
        return {};
    }

    return std::pair{ color, scatteredRay };
}
