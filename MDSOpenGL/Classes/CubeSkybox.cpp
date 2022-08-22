#include "CubeSkybox.h"
#include "../GenerateMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

CCubeSkybox::CCubeSkybox(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6])
{
	CreateSkybox(_pShader, _fSize, _pTextureDirectories);
}

void CCubeSkybox::CreateSkybox(CShader* _pShader, float _fSize, const char* _pTextureDirectories[6])
{
	m_Mesh.m_pShader = _pShader;
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
	m_Mesh.m_vTextures.push_back(CTextureManager::Insert("", 0, GL_TEXTURE_CUBE_MAP));
	CTexture* pTexture = m_Mesh.m_vTextures.back();

	pTexture->Bind();

	int iImageWidth, iImageHeight, iImageComponents;
	stbi_set_flip_vertically_on_load(false);
	for (int i = 0; i < 6; i++)
	{
		unsigned char* pImageData = stbi_load(_pTextureDirectories[i], &iImageWidth, &iImageHeight, &iImageComponents, 0);

		unsigned int GLuLoadedComponents = (iImageComponents == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D
		(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GLuLoadedComponents, iImageWidth, iImageHeight, 0,
			GLuLoadedComponents, GL_UNSIGNED_BYTE, pImageData
		);
		glGenerateMipmap(pTexture->m_GLeTarget);
		stbi_image_free(pImageData);
	}

	glTexParameteri(pTexture->m_GLeTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(pTexture->m_GLeTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(pTexture->m_GLeTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(pTexture->m_GLeTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(pTexture->m_GLeTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	CTextureManager::Unbind();
}

/*static*/ void CCubeSkybox::UpdateShaderUniforms(CShader* _pShader)
{
	_pShader->Activate();
	m_Mesh.m_vTextures.back()->Uniform(*_pShader, "Skybox");
	CTextureManager::Unbind();
	_pShader->Deactivate();
}

/*static*/ void CCubeSkybox::UpdateShaderUniforms(std::vector<CShader*> _vShaders)
{
	for (auto pShader : _vShaders) UpdateShaderUniforms(pShader);
}

void CCubeSkybox::Draw()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_Mesh.m_pShader->Activate();
	m_Mesh.m_vTextures.back()->Uniform(*m_Mesh.m_pShader, "");
	m_Mesh.Draw(GetMainCamera());
}