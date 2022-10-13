/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Texture.h																										*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <map>
#include <string>

class CShader;

class CTexture
{
private:
	unsigned int m_uiID;

public:
	static const char* m_pDirective;
	GLenum m_GLeTarget;
	
	CTexture(const char* _pName, GLenum&& _GLeTarget = GL_TEXTURE_2D);
	CTexture(const char* _pName, std::string _pImage, GLenum&& _GLeFormat, GLenum&& _GLePixelType);
	//CTexture(CTexture const&) = delete;
	CTexture& operator=(const CTexture&) = delete;
	~CTexture();

	operator int() const;
	explicit operator int* ();
	const unsigned int GetID() const;
	void Uniform(unsigned int _uiShaderID, const char* _pUniformName, unsigned int&& _uiSlot = 0);
	void Bind() const;
	static void Unbind();
};