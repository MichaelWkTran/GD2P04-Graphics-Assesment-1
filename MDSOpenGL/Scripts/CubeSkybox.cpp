/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: CubeSkybox.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "CubeSkybox.h"
#include "GenerateMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

CCubeSkybox::CCubeSkybox(const float _fSize, const char* const _pTextureDirectories[6])
{
	//Set Shader
	m_Mesh.m_pShader = CShader::Find("CubeSkybox");
	if (m_Mesh.m_pShader == nullptr) m_Mesh.m_pShader = new CShader("CubeSkybox", "CubeMap.vert", "CubeMap.frag");

	//Generate Mesh
	gm::GenerateFlatCube(m_Mesh, glm::vec3(1.0f, 1.0f, 1.0f) * _fSize);

	//Invert Faces
	std::vector<unsigned int> vIndicies = m_Mesh.GetIndicies();
	for (int i = 0; i < (int)vIndicies.size(); i += 3)
	{
		int iTemp = vIndicies[i + 1];
		vIndicies[i + 1] = vIndicies[i + 2];
		vIndicies[i + 2] = iTemp;
	}
	m_Mesh.SetIndicies(vIndicies);

	//Set up texture
	CTexture* pTexture = new CTexture("CubeSkybox", 0, GL_TEXTURE_CUBE_MAP);
	m_Mesh.m_mapTextures.insert(std::make_pair("uni_sampCube", new CTexture("CubeSkybox", 0, GL_TEXTURE_CUBE_MAP)));
	pTexture->Bind();

	int iImageWidth, iImageHeight, iImageComponents;
	stbi_set_flip_vertically_on_load(false);
	for (int i = 0; i < 6; i++)
	{
		unsigned char* pImageData = stbi_load(_pTextureDirectories[i], &iImageWidth, &iImageHeight, &iImageComponents, 0);
		unsigned int uiLoadedComponents = (iImageComponents == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D
		(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, uiLoadedComponents, iImageWidth, iImageHeight, 0,
			uiLoadedComponents, GL_UNSIGNED_BYTE, pImageData
		);
		
		stbi_image_free(pImageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	CTexture::Unbind();
}

void CCubeSkybox::UpdateShaderUniforms(CShader* _pShader)
{
	//_pShader->Activate();
	m_Mesh.m_mapTextures[0]->Uniform(*_pShader, "uni_sampSkybox");
	CTexture::Unbind();
	//_pShader->Deactivate();
}

void CCubeSkybox::UpdateShaderUniforms(std::vector<CShader*> _vShaders)
{
	for (auto pShader : _vShaders) UpdateShaderUniforms(pShader);
}

void CCubeSkybox::Draw()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	m_Mesh.Draw(GetMainCamera());
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}