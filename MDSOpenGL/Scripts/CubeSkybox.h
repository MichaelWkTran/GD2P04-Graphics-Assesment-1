/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: CubeSkybox.h																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "UpdatedObject.h"
#include "Mesh.h"
#include <vector>

class CShader;

class CCubeSkybox : public CUpdatedObject
{
private:
	CMesh<> m_Mesh;

public:
	CCubeSkybox(const float _fSize, const char* const _pTextureDirectories[6]);
	
	void UpdateShaderUniforms(CShader* _pShader);
	void UpdateShaderUniforms(std::vector<CShader*> _vShaders);
	virtual void Draw() override;
};