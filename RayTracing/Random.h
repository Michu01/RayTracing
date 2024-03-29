#pragma once

#include <random>
#include <numbers>

#include <glm/glm.hpp>

class Random
{
private:
	std::mt19937 generator{ std::random_device{}() };

public:
	static thread_local Random Instance;

public:
	float Float(std::uniform_real_distribution<float> distribution);

	glm::vec3 UnitVec3();

	glm::vec3 Color();
};

