#include "LambertianMaterial.h"
#include "HitResult.h"

LambertianMaterial::LambertianMaterial(const glm::vec3& color, float reflectance) :
    color{ color }, reflectance{ reflectance }
{
}

std::optional<std::pair<glm::vec3, Ray>> LambertianMaterial::Scatter(const Ray&, const HitResult& hitResult) const
{
    glm::vec3 direction = hitResult.normal + Random::Instance.UnitVec3();

    if (glm::length(direction) < 1e-8)
    {
        return {};
    }

    Ray outputRay(hitResult.p, direction);
    glm::vec3 outputColor = reflectance * color;

    return std::pair{ outputColor, outputRay };
}
