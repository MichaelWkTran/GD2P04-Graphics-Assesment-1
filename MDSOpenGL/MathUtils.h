#pragma once
#include <cmath>
#define PI 3.141592653589793238463f

namespace sf
{
	template <typename T>
	class Vector2;
	typedef Vector2<float> Vector2f;

	template <typename T>
	class Vector3;
	typedef Vector3<float> Vector3f;

	const float Magnitude(const Vector2f _v2f);

	const float Magnitude(const Vector3f _v3f);

	const Vector2f Normalise(const Vector2f _v2f);

	const Vector3f Normalise(const Vector3f _v3f);

	const float Distance(const Vector2f _v2fA, const Vector2f _v2fB);

	const float Distance(const Vector3f _v3fA, const Vector3f _v3fB);

	const Vector2f Truncate(const Vector2f _v2f, float _fMaxMagnitude);

	const Vector3f Truncate(const Vector3f _v3f, float _fMaxMagnitude);

	const float Dot(const Vector2f _v2fA, const Vector2f _v2fB);

	const float Dot(const Vector3f _v3fA, const Vector3f _v3fB);

	const Vector3f Cross(const Vector3f _v3fA, const Vector3f _v3fB);
}