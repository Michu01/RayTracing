#pragma once

#include <iostream>
#include <iomanip>
#include <thread>
#include <numeric>
#include <execution>
#include <functional>

#include "Image.h"
#include "Camera.h"
#include "Ray.h"
#include "SleepFor.h"
#include "SphereCollection.h"

class Renderer
{
private:
	size_t samplesPerPixel;
	size_t bounceLimit;
	std::chrono::seconds logDelay;

private:
	void LogProgress(std::atomic_uint& i, glm::uvec2 imageSize, std::stop_token stopToken) const;

	glm::vec3 GetRayColor(const Ray& ray, const SphereCollection& sphereCollection, size_t bounces = 0) const;

public:
	Renderer();

	Renderer(size_t samplesPerPixel, size_t bounceLimit, std::chrono::duration<long long> logDelay);

	void SetSamplesPerPixel(size_t samplesPerPixel);

	size_t GetSamplesPerPixel() const;

	Image Render(glm::uvec2 imageSize, const Camera& camera, const SphereCollection& sphereCollection) const;

	void Render(Image& image, const Camera& camera, const SphereCollection& sphereCollection) const;

	std::function<void(Image&)> GetRenderFunction(glm::uvec2 imageSize, const Camera& camera, const SphereCollection& sphereCollection) const;
};

