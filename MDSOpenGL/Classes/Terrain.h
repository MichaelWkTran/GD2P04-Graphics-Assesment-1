/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Terrain.h																										*/
/*  DATE: Aug 27th, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include <vector>

class CTerrain : public GameObject
{
public:
	CTerrain(const char* const _strHeightmapDirectory, const unsigned int _uiNumRows, const unsigned int _uiNumCols, const float _fHeightScaleFactor = 0.35f);
	CTerrain(const unsigned char* _pTextureData, const unsigned int _uiNumRows, const unsigned int _uiNumCols, const float _fHeightScaleFactor = 0.35f);
};