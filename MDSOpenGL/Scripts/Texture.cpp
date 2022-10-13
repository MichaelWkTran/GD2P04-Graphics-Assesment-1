/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Texture.cpp																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

const char* CTexture::m_pDirective = "Resources/Textures/";

CTexture::CTexture(const char* _pName, GLenum&& _GLeTarget)
{
	std::string strName = std::to_string(m_uiID);
	glGenTextures(1, &m_uiID);
	m_GLeTarget = _GLeTarget;
}

CTexture::CTexture(const char* _pName, std::string _pImage, GLenum&& _GLeFormat, GLenum&& _GLePixelType)
{
	std::string strName = std::to_string(m_uiID);
	glGenTextures(1, &m_uiID);
	m_GLeTarget = GL_TEXTURE_2D;

	//Load 2D Image
	int iImageWidth, iImageHeight, iChannelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load((std::string(m_pDirective) + _pImage).c_str(), &iImageWidth, &iImageHeight, &iChannelNum, 0);

	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeight, 0, _GLeFormat, _GLePixelType, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pImageData);

	Unbind();
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

void CTexture::Uniform(unsigned int _uiShaderID, const char* _pUniformName, unsigned int&& _uiSlot)
{
	glUseProgram(_uiShaderID);
	glActiveTexture(GL_TEXTURE0 + _uiSlot);
	Bind();
	glUniform1i(glGetUniformLocation(_uiShaderID, _pUniformName), _uiSlot);
	glUseProgram(0);

	_uiSlot = 0U;
}

void CTexture::Bind() const
{
	glBindTexture(m_GLeTarget, m_uiID);
}

void CTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}