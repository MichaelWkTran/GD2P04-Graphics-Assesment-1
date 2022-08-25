#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

#pragma region CTextureManager functions

std::map<const char*, CTexture*> CTextureManager::m_mapTextures;

bool CTextureManager::Empty()
{
	return m_mapTextures.empty();
}

unsigned int CTextureManager::Size()
{
	return m_mapTextures.size();
}

unsigned int CTextureManager::MaxSize()
{
	return m_mapTextures.max_size();
}

CTexture* CTextureManager::operator[](unsigned int _uiID)
{
	return At(_uiID);
}

CTexture* CTextureManager::At(unsigned int _uiID)
{
	//If the the requested ID is out of bounds, then no texture is found
	if (_uiID > m_mapTextures.size()) return nullptr;
	
	//Seach through the map to find the texture
	for (auto& pTexture : m_mapTextures)
	{
		if (pTexture.second->GetID() == _uiID) return pTexture.second;
	}
	
	//Return nullptr if no texture is found
	return nullptr;
}

CTexture* CTextureManager::Find(const char* _strName)
{
	return m_mapTextures.at(_strName);
}

CTexture* CTextureManager::Insert(const char* _pName, unsigned int&& _uiSlot, GLenum&& _GLeTarget)
{
	CTexture* pTexture = new CTexture(std::move(_GLeTarget), std::move(_uiSlot));
	if (_pName == "") _pName = std::to_string(pTexture->GetID()).c_str();
	m_mapTextures.insert(std::pair<const char*, CTexture*>(_pName, pTexture));

	return pTexture;
}

CTexture* CTextureManager::Insert(const char* _pName, const char* _pImage, unsigned int&& _uiSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType)
{
	CTexture* pTexture = new CTexture(GL_TEXTURE_2D, std::move(_uiSlot));
	if (_pName == "") _pName = std::to_string(pTexture->GetID()).c_str();
	m_mapTextures.insert(std::pair<const char*, CTexture*>(_pName, pTexture));

	int iImageWidth, iImageHeight, iImageComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load(_pImage, &iImageWidth, &iImageHeight, &iImageComponents, 0);

	#pragma region Texture Bind
	pTexture->Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeight, 0, _GLeFormat, _GLePixelType, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pImageData);

	Unbind();
	#pragma endregion

	return pTexture;
}

void CTextureManager::Erase(unsigned int _uiID)
{
	//If the the requested ID is out of bounds, then no texture is found
	if (_uiID > m_mapTextures.size()) return;

	//Seach through the map to find the texture
	for (auto& pTexture : m_mapTextures)
	{
		if (pTexture.second->GetID() == _uiID)
		{
			const char* pTextureName = pTexture.first;
			
			delete pTexture.second;
			pTexture.second = nullptr;
			m_mapTextures.erase(pTextureName);
			
			return;
		}
	}
}

void CTextureManager::Erase(const char* _strName)
{
	auto Iterator = m_mapTextures.find(_strName);
	if (Iterator == m_mapTextures.end()) return void();

	delete (*Iterator).second;
	(*Iterator).second = nullptr;
	m_mapTextures.erase(Iterator);
}

void CTextureManager::Clear()
{
	for (auto& Texture : m_mapTextures)
	{
		if (Texture.second != nullptr) delete Texture.second;
		Texture.second = nullptr;
	}

	m_mapTextures.clear();
}

void CTextureManager::Bind(unsigned int _uiID)
{
	At(_uiID)->Bind();
}

void CTextureManager::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion

#pragma region CTexture functions

CTexture::CTexture(GLenum&& _GLeTarget, unsigned int&& _uiSlot)
{
	glGenTextures(1, &m_uiID);
	m_GLeTarget = _GLeTarget;
	m_uiUnit = _uiSlot;

	_GLeTarget = 0;
	_uiSlot = 0U;
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_uiID);
}

CTexture::operator int() const
{
	return m_uiID;
}

CTexture::operator int* ()
{
	return nullptr;
}

const unsigned int CTexture::GetID() const
{
	return m_uiID;
}

void CTexture::Uniform(unsigned int _uiShaderID, const char* _strUniformName)
{
	Bind();
	glUniform1i(glGetUniformLocation(_uiShaderID, _strUniformName), m_uiUnit + 1);
}

void CTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE1 + m_uiUnit);
	glBindTexture(m_GLeTarget, m_uiID);
}

void CTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion