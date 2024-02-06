#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Random.h"

class Camera
{
private:
	float vFov;
	float aspectRatio;

	glm::vec3 lookFrom;
	glm::vec3 lookAt;
	glm::vec3 up{ 0, 1, 0 };

public:
	Camera();

	Camera(float vFov, float aspectRatio, const glm::vec3& lookFrom, const glm::vec3& lookAt);

	void SetLookFrom(const glm::vec3& lookFrom);

	void SetLookAt(const glm::vec3& lookAt);

	void SetUp(const glm::vec3& up);

	void SetVFov(float vFov);

	void SetAspectRatio(float aspectRatio);

	Ray CreateRay(glm::uvec2 size, size_t row, size_t column) const;
};
