#pragma once

#include <cmath>

template<class T>
struct Vector3
{
	T x{};
	T y{};
	T z{};

	Vector3<T> operator-() const;

	Vector3<T>& operator+=(const Vector3<T>& v);

	Vector3<T>& operator-=(const Vector3<T>& v);

	Vector3<T>& operator*=(T t);

	Vector3<T>& operator/=(T t);

	T GetLength() const;

	T GetLengthSquared() const;

	Vector3<T> Normalize() const;

	Vector3<T> Reflect(const Vector3<T>& normal) const;

	Vector3<T> Refract(const Vector3<T>& normal, double coefficient) const;
};

template<class T>
Vector3<T> operator+(Vector3<T> v, const Vector3<T>& u);

template<class T>
Vector3<T> operator-(Vector3<T> v, const Vector3<T>& u);

template<class T>
Vector3<T> operator*(T t, Vector3<T> v);

template<class T>
Vector3<T> operator/(Vector3<T> v, T t);

template<class T>
T DotProduct(const Vector3<T>& v, const Vector3<T>& u);

template<class T>
Vector3<T> CrossProduct(const Vector3<T>& v, const Vector3<T>& u);

using Vector3d = Vector3<double>;
using Point3d = Vector3d;

template<class T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return { -x, -y, -z };
}

template<class T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template<class T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template<class T>
inline Vector3<T>& Vector3<T>::operator*=(T t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

template<class T>
inline Vector3<T>& Vector3<T>::operator/=(T t)
{
	return *this *= 1.0 / t;
}

template<class T>
inline T Vector3<T>::GetLength() const
{
	return sqrt(GetLengthSquared());
}

template<class T>
inline T Vector3<T>::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

template<class T>
inline Vector3<T> Vector3<T>::Normalize() const
{
	return *this / GetLength();
}

template<class T>
inline Vector3<T> Vector3<T>::Reflect(const Vector3<T>& normal) const
{
	return *this - 2 * DotProduct(*this, normal) * normal;
}

template<class T>
inline Vector3<T> Vector3<T>::Refract(const Vector3<T>& normal, double coefficient) const
{
	T cosTheta = std::min(DotProduct(-(*this), normal), 1.0);

	Vector3<T> perpendicular = coefficient * (*this + cosTheta * normal);
	Vector3<T> parallel = -sqrt(abs(1.0 - perpendicular.GetLengthSquared())) * normal;

	return perpendicular + parallel;
}

template<class T>
inline T DotProduct(const Vector3<T>& v, const Vector3<T>& u)
{
	return v.x * u.x + v.y * u.y + v.z * u.z;
}

template<class T>
inline Vector3<T> CrossProduct(const Vector3<T>& v, const Vector3<T>& u)
{
	return Vector3<T>(
		v.y * u.z - v.z * u.y,
		v.z * u.x - v.x * u.z,
		v.x * u.y - v.y * u.x);
}

template<class T>
inline Vector3<T> operator*(T t, Vector3<T> v)
{
	return v *= t;
}

template<class T>
inline Vector3<T> operator+(Vector3<T> v, const Vector3<T>& u)
{
	return v += u;
}

template<class T>
inline Vector3<T> operator-(Vector3<T> v, const Vector3<T>& u)
{
	return v -= u;
}

template<class T>
inline Vector3<T> operator/(Vector3<T> v, T t)
{
	return v /= t;
}
