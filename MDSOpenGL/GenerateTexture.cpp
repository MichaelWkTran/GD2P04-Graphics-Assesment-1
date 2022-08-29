#include "GenerateTexture.h"
#include <math.h>
#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

float Smooth(float, float, int);
float Lerp(float, float, float);
float CosineInterpolate(float, float, float);
float SmoothInterpolate(float, float, int);

#pragma region gt namespace methods


#pragma region CImageData methods

gt::CImageData::CImageData(int _iWidth, int _iHeight, int _iChannels, bool _bZeroed)
{
	unsigned int uiDataSize = _iWidth * _iHeight * _iChannels;
	if (_bZeroed) pData = (unsigned char*)calloc(uiDataSize, sizeof(unsigned char));
	else pData = (unsigned char*)malloc(uiDataSize * sizeof(unsigned char));

	if (pData == nullptr)
	{
		std::cout << "ERROR: Can not allocate data for the image";
		return;
	}

	iWidth = _iWidth;
	_iHeight = _iHeight;
	uiSize = uiDataSize;
	iChannels = _iChannels;
}

gt::CImageData::~CImageData()
{
	free(pData);
}

bool StrEndsWith(const std::string _String, const std::string _strSuffix) {
	if (_String.length() < _strSuffix.length()) { return false; }

	return _String.compare(_String.length() - _strSuffix.length(), _strSuffix.length(), _strSuffix) == 0;
}

void gt::CImageData::SaveImage(const char* _strFileName)
{
	if (StrEndsWith(_strFileName, ".jpg") || StrEndsWith(_strFileName, ".JPG") || StrEndsWith(_strFileName, ".jpeg") || StrEndsWith(_strFileName, ".JPEG"))
	{
		stbi_write_jpg(_strFileName, iWidth, iHeight, iChannels, pData, 100);
	}
	else if (StrEndsWith(_strFileName, ".png") || StrEndsWith(_strFileName, ".PNG"))
	{
		stbi_write_png(_strFileName, iWidth, iHeight, iChannels, pData, iWidth * iChannels);
	}
	else if (StrEndsWith(_strFileName, ".bmp") || StrEndsWith(_strFileName, ".BMP"))
	{
		stbi_write_bmp(_strFileName, iWidth, iHeight, iChannels, pData);
	}
	else if (StrEndsWith(_strFileName, ".raw") || StrEndsWith(_strFileName, ".RAW"))
	{

	}
	else
	{

	}
}

#pragma endregion

float gt::Noise(int _x, int _y, int _iSeed)
{
	double value;
	
	//(n << 13) is the same thing as (n * 2^13)
	//the (x & 7fffffff) operation is equivaLent to (x mod 2147483648) 
	
	int noise = _x + _y * _iSeed; 
	noise = (noise << 13) ^ noise; 

	int t = (noise * (noise * noise * 15731 + 789221) + 1376312589) & 0x7fffffff;
	value = 1.0 - double(t) * 0.931322574615478515625e-9;
	
	return value;
}

float gt::PerlinNoise(int _x, int _y, int _iOctaves, float _fZoom, float _fPersistance, int _iSeed)
{
	float fTotal = 0.0f;

	for (int i = 0; i < _iOctaves - 1; i++)
	{
		float fFrequency = powf(2, i) / _fZoom;
		float fAmplitude = powf(_fPersistance, i);

		fTotal += SmoothInterpolate(_x * fFrequency, _y * fFrequency, _iSeed) * fAmplitude;
	}

	return fTotal;
}

#pragma endregion

float Smooth(float x, float y, int _iSeed)
{
	float fCorners = gt::Noise(x - 1, y - 1, _iSeed) + gt::Noise(x + 1, y - 1, _iSeed) + gt::Noise(x - 1, y + 1, _iSeed) + gt::Noise(x + 1, y + 1, _iSeed); fCorners /= 16;
	float fSides = gt::Noise(x - 1, y, _iSeed) + gt::Noise(x + 1, y, _iSeed) + gt::Noise(x, y - 1, _iSeed) + gt::Noise(x , y + 1, _iSeed); fSides /= 8;
	float fCentre = gt::Noise(x, y, _iSeed) / 4;

	return fCorners + fSides + fCentre;
}

float Lerp(float _a, float _b, float _t)
{
	return _a + ((_b - _a) * _t);
}

float CosineInterpolate(float _a, float _b, float _t)
{
	return Lerp(_a, _b, (1 - cosf(_t * 3.14f)) / 2.0f);
}

float SmoothInterpolate(float x, float y, int _iSeed)
{
	float fractX = x - int(x);
	float fractY = y - int(y);
	
	//Smooths
	float v1 = Smooth(int(x), int(y), _iSeed);
	float v2 = Smooth(int(x) + 1, int(y), _iSeed);
	
	float v3 = Smooth(int(x), int(y) + 1, _iSeed);
	float v4 = Smooth(int(x) + 1, int(y) + 1, _iSeed);
	
	//Interpolates
	float i1 = Lerp(v1, v2, fractX);
	float i2 = Lerp(v3, v4, fractX);
	
	return Lerp(i1, i2, fractY);
}