#pragma once
#include "GameObject.h"

class CShader;
class CTexture;

class CGeoStar : public CGameObject
{
public:
	static CShader* m_pShader;
	static CTexture* m_pTexture;

	CGeoStar();
};