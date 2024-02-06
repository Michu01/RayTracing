#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h"
#include "Random.h"

class Camera
{
private:
	float vFov;
	float aspectRatio;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up{ 0, 1, 0 };

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
	
	void Rotate(const glm::vec3& origin, const glm::vec3& offset);

	Ray CreateRay(glm::uvec2 size, size_t row, size_t column) const;
};
