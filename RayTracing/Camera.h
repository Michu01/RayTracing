#pragma once

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h"
#include "Random.h"

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up{ 0, 1, 0 };

	float yaw = 90.0f;
	float pitch = 0.0f;

public:
	float vFov;
	float aspectRatio;

public:
	Camera();

	Camera(float vFov, float aspectRatio, const glm::vec3& position, const glm::vec3& direction);

	void SetLookFrom(const glm::vec3& lookFrom);

	void SetLookAt(const glm::vec3& lookAt);

	void SetUp(const glm::vec3& up);

	void SetVFov(float vFov);

	void SetAspectRatio(float aspectRatio);

	void Move(const glm::vec3& offset);

	void Zoom(float factor);
	
	void Rotate(const glm::vec3& offset);

	Ray CreateRay(glm::uvec2 size, size_t row, size_t column) const;

	std::function<Ray(size_t, size_t)> GetCreateRayFunction(glm::uvec2 size) const;
};
