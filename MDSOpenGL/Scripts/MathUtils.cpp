// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
//
// File Name: MathUtils.cpp
// Description: Math utils declaration file
// Authors: Michael Wai Kit Tran

#include "MathUtils.h"
#include <glm/gtc/constants.hpp>

using namespace std;

const float glm::Lerp(const float _a, const float _b, const float _t)
{
	return _a + ((_b - _a) * _t);
}

const float glm::CosineInterpolate(const float _a, const float _b, const float _t)
{
	return Lerp(_a, _b, (1.0f - cosf(_t * pi<float>())) / 2.0f);
}