#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

const char* CTexture::m_strDirective = "Resources/Textures/";
std::map<const char*, CTexture*> CTexture::m_mapTextures;

CTexture::CTexture(const char* _pName, unsigned int&& _uiSlot, GLenum&& _GLeTarget)
{
	if (_pName == "") _pName = std::to_string(m_uiID).c_str();
	m_mapTextures.emplace(std::make_pair(m_strName = _pName, this));
	glGenTextures(1, &m_uiID);
	m_GLeTarget = _GLeTarget;
	m_uiUnit = _uiSlot;
	
	_uiSlot = 0U;
	_GLeTarget = 0U;
}

CTexture::CTexture(const char* _pName, std::string _pImage, unsigned int&& _uiSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType)
{
	//Set up CTexture
	if (_pName == "") _pName = std::to_string(m_uiID).c_str();
	m_mapTextures.emplace(std::make_pair(m_strName = _pName, this));
	glGenTextures(1, &m_uiID);
	m_GLeTarget = GL_TEXTURE_2D;
	m_uiUnit = _uiSlot;	

	//Load 2D Image
	int iImageWidth, iImageHeight, iChannelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load((std::string(m_strDirective) + _pImage).c_str(), &iImageWidth, &iImageHeight, &iChannelNum, 0);

	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeight, 0, _GLeFormat, _GLePixelType, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pImageData);

	Unbind();
	
	_uiSlot = 0U;
	_GLeFormat = 0U;
	_GLePixelType = 0U;
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

void CTexture::Uniform(unsigned int _uiShaderID, const char* _pUniformName)
{
	glUseProgram(_uiShaderID);
	Bind();
	glUniform1i(glGetUniformLocation(_uiShaderID, _pUniformName), m_uiUnit);
	glUseProgram(0);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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

CTexture* CTexture::Find(const char* _pName)
{
	if (m_mapTextures.find(_pName) == m_mapTextures.end()) return nullptr;
	return m_mapTextures.at(_pName);
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