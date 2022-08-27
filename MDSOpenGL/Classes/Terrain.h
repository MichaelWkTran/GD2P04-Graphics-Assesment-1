#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include <vector>

class CTerrain : public CGameObject
{
public:
	CTerrain() {};
	CTerrain(const char* _strHeightmapDirectory, unsigned int _uiNumRows, unsigned int _uiNumCols, float _fHeightScaleFactor = 0.35f);

	void Initalise(const char* _strHeightmapDirectory, unsigned int _uiNumRows, unsigned int _uiNumCols, float _fHeightScaleFactor = 0.35f);
};

