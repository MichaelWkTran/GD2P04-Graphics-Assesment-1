#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

std::map<const char*, CTexture*> CTexture::m_mapTextures;

CTexture::CTexture(const char* _pName, unsigned int&& _uiSlot, GLenum&& _GLeTarget)
{
	if (_pName == "") _pName = std::to_string(m_uiID).c_str();
	m_mapTextures.emplace(std::make_pair(m_strName = _pName, this));
	glGenTextures(1, &m_uiID);
	m_GLeTarget = _GLeTarget;
	m_uiUnit = _uiSlot;
}

CTexture::CTexture(const char* _pName, const char* _pImage, unsigned int&& _uiSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType)
{
	//Set up CTexture
	if (_pName == "") _pName = std::to_string(m_uiID).c_str();
	m_mapTextures.emplace(std::make_pair(m_strName = _pName, this));
	glGenTextures(1, &m_uiID);
	m_GLeTarget = GL_TEXTURE_2D;
	m_uiUnit = _uiSlot;	

	//Load 2D Image
	int iImageWidth, iImageHeight, iColChanNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load(_pImage, &iImageWidth, &iImageHeight, &iColChanNum, 0);

	Bind();

	glTexParameteri(m_GLeTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_GLeTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(m_GLeTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_GLeTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(m_GLeTarget, 0, GL_RGBA, iImageWidth, iImageHeight, 0, _GLeFormat, _GLePixelType, pImageData);
	glGenerateMipmap(m_GLeTarget);
	stbi_image_free(pImageData);

	Unbind();
}

CTexture::~CTexture()
{
	m_mapTextures.at(m_strName) = nullptr;
	m_mapTextures.erase(m_strName);
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
	glUniform1i(glGetUniformLocation(_uiShaderID, _strUniformName), m_uiUnit);
}

void CTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_uiUnit);
	glBindTexture(m_GLeTarget, m_uiID);
}

void CTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
}

bool CTexture::Empty()
{
	return m_mapTextures.empty();
}

unsigned int CTexture::Size()
{
	return m_mapTextures.size();
}

unsigned int CTexture::MaxSize()
{
	return m_mapTextures.max_size();
}

CTexture* CTexture::At(unsigned int _uiID)
{
	//Seach through the map to find the texture
	for (auto& pTexture : m_mapTextures)
	{
		if (pTexture.second->GetID() == _uiID) return pTexture.second;
	}

	//Return nullptr if no texture is found
	return nullptr;
}

CTexture* CTexture::Find(const char* _strName)
{
	if (m_mapTextures.find(_strName) == m_mapTextures.end()) return nullptr;
	return m_mapTextures.at(_strName);
}

void CTexture::Erase(unsigned int _uiID)
{
	CTexture* pTexture = At(_uiID);
	if (pTexture == nullptr) return;

	delete pTexture;
}

void CTexture::Erase(std::string _strName)
{
	auto Iterator = m_mapTextures.find(_strName.c_str());
	if (Iterator == m_mapTextures.end()) return void();

	delete (*Iterator).second;
}

void CTexture::Clear()
{
	for (auto& pTexture : m_mapTextures)
	{
		if (pTexture.second != nullptr) delete pTexture.second;
		pTexture.second = nullptr;
	}

	m_mapTextures.clear();
}