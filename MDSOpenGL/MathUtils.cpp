#include "MathUtils.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

using namespace std;

const float sf::Magnitude(const sf::Vector2f _v2f)
{
	return sqrtf((_v2f.x * _v2f.x) + (_v2f.y * _v2f.y));
}

const float sf::Magnitude(const sf::Vector3f _v3f)
{
	return sqrtf((_v3f.x * _v3f.x) + (_v3f.y * _v3f.y) + (_v3f.z * _v3f.z));
}

const sf::Vector2f sf::Normalise(const sf::Vector2f _v2f)
{
	float&& fMagnitude = Magnitude(_v2f);

	if (fMagnitude != 0) return _v2f / fMagnitude;
	else return sf::Vector2f();
}

const sf::Vector3f sf::Normalise(const sf::Vector3f _v3f)
{
	float&& fMagnitude = Magnitude(_v3f);

	if (fMagnitude != 0) return _v3f / fMagnitude;
	else return sf::Vector3f();
}

const float sf::Distance(const sf::Vector2f _v2fA, const sf::Vector2f _v2fB)
{
	return Magnitude(_v2fB - _v2fA);
}

const float sf::Distance(const sf::Vector3f _v3fA, const sf::Vector3f _v3fB)
{
	return Magnitude(_v3fB - _v3fA);
}

const sf::Vector2f sf::Truncate(const sf::Vector2f _v2f, float _fMaxMagnitude)
{
	if (Magnitude(_v2f) > _fMaxMagnitude) return Normalise(_v2f) * _fMaxMagnitude;
	else return _v2f;
}

const sf::Vector3f sf::Truncate(const sf::Vector3f _v3f, float _fMaxMagnitude)
{
	if (Magnitude(_v3f) > _fMaxMagnitude) return Normalise(_v3f) * _fMaxMagnitude;
	else return _v3f;
}

const float sf::Dot(const sf::Vector2f _v2fA, const sf::Vector2f _v2fB)
{
	return (_v2fA.x * _v2fA.x) + (_v2fA.y * _v2fA.y);
}

const float sf::Dot(const sf::Vector3f _v3fA, const sf::Vector3f _v3fB)
{
	return (_v3fA.x * _v3fA.x) + (_v3fA.y * _v3fA.y) + (_v3fA.z * _v3fA.z);
}

const sf::Vector3f sf::Cross(const sf::Vector3f _v3fA, const sf::Vector3f _v3fB)
{
	return sf::Vector3f
	(
		(_v3fA.y * _v3fB.z) - (_v3fA.z * _v3fB.y),
		(_v3fA.z * _v3fB.x) - (_v3fA.x * _v3fB.z),
		(_v3fA.x * _v3fB.y) - (_v3fA.y * _v3fB.x)
	);
}