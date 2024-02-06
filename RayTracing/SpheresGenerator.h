#pragma once

#include "SphereCollection.h"
#include "Random.h"

class SpheresGenerator
{
public:
	struct Parameters
	{
		size_t maxCount = 200;

		glm::vec3 center{ 0, 0, 0 };
		float radius = 10.0f;

		float lambertian = 0.4f;
		float metal = 0.3f;
		float dielectric = 0.3f;

		float minRadius = 0.1f;
		float maxRadius = 1.0f;
	};

public:
	SphereCollection Generate(const Parameters& parameters);
};

