/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: GenerateTexture.cpp																							*/
/*  DATE: Aug 27th, 2022																								*/
/************************************************************************************************************************/

#include "GenerateTexture.h"
#include <math.h>

float Smooth(float, float, int);
float Lerp(float, float, float);
float CosineInterpolate(float, float, float);
float SmoothInterpolate(float, float, int);

#pragma region gt namespace methods

float gt::Noise(float _x, float _y, int _iSeed)
{
	//(n << 13) is the same thing as (n * 2^13)
	//the (x & 7fffffff) operation is equivaLent to (x mod 2147483648) 
	
	int iNoise = _x + _y * _iSeed; 
	iNoise = (iNoise << 13) ^ iNoise; 

	int t = (iNoise * (iNoise * iNoise * 15731 + 789221) + 1376312589) & 0x7fffffff;
	double dValue = 1.0 - double(t) * 0.931322574615478515625e-9;
	
	return (float)dValue;
}

float gt::PerlinNoise(float _x, float _y, unsigned int _uiOctaves, float _fZoom, float _fPersistance, int _iSeed)
{
	float fTotal = 0.0f;

	for (unsigned int uiIterations = 0; uiIterations < _uiOctaves - 1U; uiIterations++)
	{
		float fFrequency = powf(2.0f, (float)uiIterations) / _fZoom;
		float fAmplitude = powf(_fPersistance, (float)uiIterations);

		fTotal += SmoothInterpolate(_x * fFrequency, _y * fFrequency, _iSeed) * fAmplitude;
	}

	return fTotal;
}

#pragma endregion

float Smooth(float _x, float _y, int _iSeed)
{
	float fCorners = gt::Noise(_x-1.0f, _y-1.0f, _iSeed) + gt::Noise(_x+1.0f, _y-1.0f, _iSeed) + gt::Noise(_x-1.0f, _y+1.0f, _iSeed) + gt::Noise(_x+1.0f, _y+1.0f, _iSeed); fCorners /= 16.0f;
	float fSides = gt::Noise(_x-1.0f, _y, _iSeed) + gt::Noise(_x+1.0f, _y,	_iSeed) + gt::Noise(_x, _y-1.0f, _iSeed) + gt::Noise(_x , _y+1.0f, _iSeed); fSides /= 8.0f;
	float fCentre = gt::Noise(_x, _y, _iSeed) / 4.0f;

	return fCorners + fSides + fCentre;
}

float Lerp(float _a, float _b, float _t)
{
	return _a + ((_b - _a) * _t);
}

float CosineInterpolate(float _a, float _b, float _t)
{
	return Lerp(_a, _b, (1.0f - cosf(_t * 3.14f)) / 2.0f);
}

float SmoothInterpolate(float _x, float _y, int _iSeed)
{
	float fFractX = _x - int(_x);
	float fFractY = _y - int(_y);
	
	//Smooths
	float v1 = Smooth(int(_x),		  int(_y),		  _iSeed);
	float v2 = Smooth(int(_x) + 1.0f, int(_y),		  _iSeed);
	
	float v3 = Smooth(int(_x),		  int(_y) + 1.0f, _iSeed);
	float v4 = Smooth(int(_x) + 1.0f, int(_y) + 1.0f, _iSeed);
	
	//Interpolates
	float i1 = Lerp(v1, v2, fFractX);
	float i2 = Lerp(v3, v4, fFractX);
	
	return Lerp(i1, i2, fFractY);
}