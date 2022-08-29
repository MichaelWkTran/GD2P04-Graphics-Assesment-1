#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include <vector>

class CTerrain : public CGameObject
{
public:
	CTerrain(const char* const _strHeightmapDirectory, const unsigned int _uiNumRows, const unsigned int _uiNumCols, const float _fHeightScaleFactor = 0.35f);
	CTerrain(const unsigned char* _pTextureData, const unsigned int _uiNumRows, const unsigned int _uiNumCols, const float _fHeightScaleFactor = 0.35f);
};