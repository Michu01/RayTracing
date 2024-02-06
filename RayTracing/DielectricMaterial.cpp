#include "DielectricMaterial.h"
#include "HitResult.h"

float DielectricMaterial::GetReflectance(float cos, float ref)
{
    float r = (1 - ref) / (1 + ref);
    float rs = r * r;
    return rs + (1 - rs) * pow(1 - cos, 5.0f);
}

DielectricMaterial::DielectricMaterial() :
    refraction{}
{
}

DielectricMaterial::DielectricMaterial(float refraction) :
    refraction{ refraction }
{
}

std::optional<std::pair<glm::vec3, Ray>> DielectricMaterial::Scatter(const Ray& inputRay, const HitResult& hitResult) const
{
    glm::vec3 color(1.0, 1.0, 1.0);

    float refractionRatio = hitResult.isFrontFace ? 1.0f / refraction : refraction;

    float cosTheta = std::min(glm::dot(-inputRay.direction, hitResult.normal), 1.0f);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool canRefract = refractionRatio * sinTheta > 1.0;

    float random = Random::Instance.Float(std::uniform_real_distribution{ 0.0f, 1.0f });
    float reflectance = GetReflectance(cosTheta, refractionRatio);

    glm::vec3 direction = canRefract && random < reflectance ?
        glm::refract(inputRay.direction, hitResult.normal, refractionRatio) :
        glm::reflect(inputRay.direction, hitResult.normal);

    Ray scatteredRay(hitResult.p, direction);

    return std::pair{ color, scatteredRay };
}
