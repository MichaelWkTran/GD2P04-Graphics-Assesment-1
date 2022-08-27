#include "GenerateTexture.h"
#include <math.h>

float gt::Noise(int _x, int _y, int _iSeed)
{
	double value;
	
	//(n << 13) is the same thing as (n * 2^13)
	//the (x & 7fffffff) operation is equivaLent to (x mod 2147483648) 
	
	// 57 here is the seed vaLue
	int noise = _x + _y * _iSeed; 
	noise = (noise << 13) ^ noise; 

	int t = (noise * (noise * noise * 15731 + 789221) + 1376312589) & 0x7fffffff;
	value = 1.0 - double(t) * 0.931322574615478515625e-9;
	
	return value;
}

float gt::Smooth(float x, float y, int _iSeed)
{
	float fCorners = Noise(x - 1, y - 1, _iSeed) + Noise(x + 1, y - 1, _iSeed) + Noise(x - 1, y + 1, _iSeed) + Noise(x + 1, y + 1, _iSeed); fCorners /= 16;
	float fSides = Noise(x - 1, y, _iSeed) + Noise(x + 1, y, _iSeed) + Noise(x, y - 1, _iSeed) + Noise(x , y + 1, _iSeed); fSides /= 8;
	float fCentre = Noise(x, y, _iSeed) / 4;

	return fCorners + fSides + fCentre;
}

float gt::Lerp(float _a, float _b, float _t)
{
	return _a + ((_b - _a) * _t);
}

float gt::CosineInterpolate(float _a, float _b, float _t)
{
	return Lerp(_a, _b, (1 - cosf(_t * 3.14f)) / 2.0f);
}

