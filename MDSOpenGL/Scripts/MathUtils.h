// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
//
// File Name: MathUtils.h
// Description: Math utils initialization file
// Authors: Michael Wai Kit Tran

#pragma once
#include <cmath>

namespace glm
{
	const float Lerp(const float _a, const float _b, const float _t);
	const float CosineInterpolate(const float _a, const float _b, const float _t);
}