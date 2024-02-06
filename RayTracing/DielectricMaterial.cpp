#include "DielectricMaterial.h"
#include "HitResult.h"

double DielectricMaterial::GetReflectance(double cos, double ref)
{
    double r = (1 - ref) / (1 + ref);
    double rs = r * r;
    return rs + (1 - rs) * pow(1 - cos, 5);
}

DielectricMaterial::DielectricMaterial(double refraction) :
    refraction{ refraction }
{
}

std::optional<std::pair<Color, Ray>> DielectricMaterial::Scatter(const Ray& inputRay, const HitResult& hitResult) const
{
    Color color(1.0, 1.0, 1.0);

    double refractionRatio = hitResult.isFrontFace ? 1.0 / refraction : refraction;

    double cosTheta = std::min(DotProduct(-inputRay.direction, hitResult.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool canRefract = refractionRatio * sinTheta > 1.0;

    double random = Random::Instance.RandomDouble(std::uniform_real_distribution{ 0.0, 1.0 });
    double reflectance = GetReflectance(cosTheta, refractionRatio);

    Vector3d direction = canRefract && random < reflectance ?
        inputRay.direction.Refract(hitResult.normal, refractionRatio) :
        inputRay.direction.Reflect(hitResult.normal);

    Ray scatteredRay(hitResult.p, direction);

    return std::pair{ color, scatteredRay };
}
