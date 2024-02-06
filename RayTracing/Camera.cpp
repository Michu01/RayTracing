#include "Camera.h"

Camera::Camera(double vFov, double aspectRatio, Point3d lookFrom, Point3d lookAt) :
	vFov{ vFov }, lookFrom{ lookFrom }, lookAt{ lookAt }, aspectRatio{ aspectRatio }
{
}

Camera::Camera() :
	vFov{}, aspectRatio{}
{
}

Ray Camera::CreateRay(SizeU size, size_t row, size_t column) const
{
	Vector3d direction = lookFrom - lookAt;

	Vector3d w = direction.Normalize();
	Vector3d u = CrossProduct(up, w).Normalize();
	Vector3d v = CrossProduct(w, u);

	double focalLength = direction.GetLength();

	double theta = DegreesToRadians(vFov);
	double viewportHeight = 2 * tan(0.5 * theta) * focalLength;
	double viewportWidth = viewportHeight * aspectRatio;

	Vector3d viewportU = viewportWidth * u;
	Vector3d viewportV = viewportHeight * -v;

	Vector3d pixelDeltaU = viewportU / (double)size.width;
	Vector3d pixelDeltaV = viewportV / (double)size.height;

	Vector3d viewportUpperLeft = lookFrom - focalLength * w - 0.5 * (viewportU + viewportV);

	Vector3d pixelStart = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	std::uniform_real_distribution<double> distribution(-0.5, 0.5);

	double dx = Random::Instance.RandomDouble(distribution);
	double dy = Random::Instance.RandomDouble(distribution);

	Vector3d pixelCoord = pixelStart + ((double)row * pixelDeltaV) + ((double)column * pixelDeltaU);
	Vector3d sampleCoord = pixelCoord + dx * pixelDeltaU + dy * pixelDeltaV;

	Ray ray(lookFrom, sampleCoord - lookFrom);

	return ray;
}

void Camera::SetUp(const Vector3d& up)
{
	this->up = up;
}

void Camera::SetLookFrom(const Point3d& lookFrom)
{
	this->lookFrom = lookFrom;
}

void Camera::SetLookAt(const Point3d& lookAt)
{
	this->lookAt = lookAt;
}

void Camera::SetVFov(double vFov)
{
	this->vFov = vFov;
}

void Camera::SetAspectRatio(double aspectRatio)
{
	this->aspectRatio = aspectRatio;
}
