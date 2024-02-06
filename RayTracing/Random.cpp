#include "Random.h"

Random Random::Instance;

double Random::RandomDouble(std::uniform_real_distribution<double> distribution)
{
    return distribution(generator);
}

Vector3d Random::RandomUnitVector3d()
{
    double theta = RandomDouble(std::uniform_real_distribution(0.0, std::numbers::pi));
    double phi = RandomDouble(std::uniform_real_distribution(0.0, 2.0 * std::numbers::pi));
    Vector3d vec;
    vec.x = sin(theta) * cos(phi);
    vec.y = sin(theta) * sin(phi);
    vec.z = cos(theta);
    return vec;
}

Vector3d Random::RandomUnitVector3dInHemisphere(const Vector3d& normal)
{
    Vector3d vec = RandomUnitVector3d();

    return DotProduct(vec, normal) > 0.0 ? vec : -vec;
}

Color Random::RandomColor()
{
    std::uniform_real_distribution distribution(0.0, 1.0);

    return { distribution(generator), distribution(generator), distribution(generator) };
}
