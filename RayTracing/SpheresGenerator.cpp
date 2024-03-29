#include "SpheresGenerator.h"

SphereCollection SpheresGenerator::Generate(const Parameters& parameters)
{
    std::vector<Sphere> spheres;

    auto randGen = Random::Instance;

    auto isCollision = [&spheres](const Sphere& s1)
        {
            return std::any_of(spheres.cbegin(), spheres.cend(), [&s1](const Sphere& s2)
                {
                    return glm::length(s1.center - s2.center) < s1.radius + s2.radius;
                });
        };

    auto getMaterial = [&randGen, &parameters]() -> std::optional<Material>
        {
            float choice = randGen.Float(std::uniform_real_distribution<float>(0.0f, 1.0f));

            if (choice < parameters.lambertian)
            {
                return LambertianMaterial(randGen.Color(), 1.0);
            }

            choice -= parameters.lambertian;

            if (choice < parameters.dielectric)
            {
                return DielectricMaterial(1.5);
            }

            choice -= parameters.metal;

            if (choice < parameters.metal)
            {
                return MetalMaterial(randGen.Color(), randGen.Float(std::uniform_real_distribution<float>(0.0, 1.0)));
            }

            return {};
        };

    while (spheres.size() != parameters.maxCount)
    {
        Sphere sphere;
        sphere.radius = randGen.Float(std::uniform_real_distribution<float>(parameters.minRadius, parameters.maxRadius));
        sphere.center = parameters.center + parameters.radius * randGen.UnitVec3();
        sphere.center.y = parameters.center.y + sphere.radius;

        if (isCollision(sphere))
        {
            continue;
        }

        auto material = getMaterial();

        if (!material)
        {
            continue;
        }

        sphere.material = material.value();
       
        spheres.push_back(sphere);
    }

    Sphere sphere(parameters.center - glm::vec3(0, 10000, 0), 10000, LambertianMaterial(randGen.Color(), 1.0));
    spheres.push_back(sphere);

    return SphereCollection(spheres);
}
