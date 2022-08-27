#include <math.h>

namespace gt
{
	float Noise(int _x, int _y, int _iSeed = 0);
	float PerlinNoise(int _x, int _y, int _iOctaves = 4, float _fZoom = 64.0f, float _fPersistance = 0.5f, int _iSeed = 0);
}