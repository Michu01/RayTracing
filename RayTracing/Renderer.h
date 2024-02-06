#pragma once

#include <iostream>
#include <iomanip>
#include <thread>
#include <numeric>
#include <execution>
#include <functional>

#include "Image.h"
#include "Vector3.h"
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
	void LogProgress(std::atomic_uint& i, const SizeU& imageSize, std::stop_token stopToken) const;

	Color GetRayColor(const Ray& ray, const SphereCollection& sphereCollection, size_t bounces = 0) const;

public:
	Renderer();

	Renderer(size_t samplesPerPixel, size_t bounceLimit, std::chrono::duration<long long> logDelay);

	void SetSamplesPerPixel(size_t samplesPerPixel);

	size_t GetSamplesPerPixel() const;

	Image Render(const SizeU& imageSize, const Camera& camera, const SphereCollection& sphereCollection) const;

	void Render(Image& image, const Camera& camera, const SphereCollection& sphereCollection) const;

	std::function<void(Image&)> GetRenderFunction(const SizeU& imageSize, const Camera& camera, const SphereCollection& sphereCollection) const;
};

