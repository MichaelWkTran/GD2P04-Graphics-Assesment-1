#pragma once
#include "GameObject.h"

class CGeoSphere : public GameObject
{
public:
	static CShader* m_pNormalShader;

	CGeoSphere();

	virtual void Draw() override;
};

