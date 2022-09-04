#include <math.h>

namespace gt
{
	class CImageData
	{
		//https://www.youtube.com/watch?v=Y_fJXlKsHpk
		int iWidth, iHeight, iChannels;
		unsigned int uiSize;
		unsigned char* pData;
	
		CImageData(int width, int height, int channels, bool zeroed = true);
		~CImageData();
		void SaveImage(const char* _pFileName);
	};

	float Noise(int _x, int _y, int _iSeed = 0);
	float PerlinNoise(int _x, int _y, int _iOctaves = 4, float _fZoom = 64.0f, float _fPersistance = 0.5f, int _iSeed = 0);
}