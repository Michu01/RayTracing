#include "Random.h"

Random Random::Instance;

float Random::Float(std::uniform_real_distribution<float> distribution)
{
    return distribution(generator);
}

glm::vec3 Random::UnitVec3()
{
    float theta = Float(std::uniform_real_distribution(0.0f, std::numbers::pi_v<float>));
    float phi = Float(std::uniform_real_distribution(0.0f, 2.0f * std::numbers::pi_v<float>));
    glm::vec3 vec{};
    vec.x = sin(theta) * cos(phi);
    vec.y = sin(theta) * sin(phi);
    vec.z = cos(theta);
    return vec;
}

glm::vec3 Random::Color()
{
    std::uniform_real_distribution distribution(0.0, 1.0);

    return { distribution(generator), distribution(generator), distribution(generator) };
}
