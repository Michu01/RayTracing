#include "Camera.h"

Camera::Camera(float vFov, float aspectRatio, const glm::vec3& lookFrom, const glm::vec3& lookAt) :
	vFov{ vFov }, lookFrom{ lookFrom }, lookAt{ lookAt }, aspectRatio{ aspectRatio }
{
}

Camera::Camera() :
	vFov{}, aspectRatio{}, lookFrom{}, lookAt{}
{
}

Ray Camera::CreateRay(glm::uvec2 size, size_t row, size_t column) const
{
	glm::vec3 direction = lookFrom - lookAt;

	glm::vec3 w = glm::normalize(direction);
	glm::vec3 u = glm::normalize(glm::cross(up, w));;
	glm::vec3 v = glm::cross(w, u);

	float focalLength = glm::length(direction);

	float theta = glm::radians(vFov);
	float viewportHeight = 2.0f * tan(0.5f * theta) * focalLength;
	float viewportWidth = viewportHeight * aspectRatio;

	glm::vec3 viewportU = viewportWidth * u;
	glm::vec3 viewportV = viewportHeight * -v;

	glm::vec3 pixelDeltaU = viewportU / (float)size.x;
	glm::vec3 pixelDeltaV = viewportV / (float)size.y;

	glm::vec3 viewportUpperLeft = lookFrom - focalLength * w - 0.5f * (viewportU + viewportV);

	glm::vec3 pixelStart = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

	std::uniform_real_distribution<float> distribution(-0.5f, 0.5f);

	float dx = Random::Instance.Float(distribution);
	float dy = Random::Instance.Float(distribution);

	glm::vec3 pixelCoord = pixelStart + ((float)row * pixelDeltaV) + ((float)column * pixelDeltaU);
	glm::vec3 sampleCoord = pixelCoord + dx * pixelDeltaU + dy * pixelDeltaV;

	Ray ray(lookFrom, sampleCoord - lookFrom);

	return ray;
}

void Camera::SetUp(const glm::vec3& up)
{
	this->up = up;
}

void Camera::SetLookFrom(const glm::vec3& lookFrom)
{
	this->lookFrom = lookFrom;
}

void Camera::SetLookAt(const glm::vec3& lookAt)
{
	this->lookAt = lookAt;
}

void Camera::SetVFov(float vFov)
{
	this->vFov = vFov;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}
