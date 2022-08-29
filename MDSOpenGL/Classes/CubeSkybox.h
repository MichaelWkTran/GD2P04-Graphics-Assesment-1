#pragma once
#include <vector>
#include "UpdatedObject.h"
#include "Mesh.h"

class CShader;

class CCubeSkybox : public CUpdatedObject
{
private:
	CMesh<> m_Mesh;

public:
	CCubeSkybox(float _fSize, const char* _pTextureDirectories[6]);
	
	void UpdateShaderUniforms(CShader* _pShader);
	void UpdateShaderUniforms(std::vector<CShader*> _vShaders);
	virtual void Draw() override;
};