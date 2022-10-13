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
	static std::map<const char* /*Texture name*/, CTexture*> m_mapTextures;
	unsigned int m_uiID;

public:
	static const char* m_pDirective;
	const char* m_pName;
	GLenum m_GLeTarget;
	
	CTexture(const char* _pName, GLenum&& _GLeTarget = GL_TEXTURE_2D);
	CTexture(const char* _pName, std::string _pImage, GLenum&& _GLeFormat, GLenum&& _GLePixelType);
	CTexture(CTexture const&) = delete;
	CTexture& operator=(const CTexture&) = delete;
	~CTexture();

	operator int() const;
	explicit operator int* ();
	const unsigned int GetID() const;
	void Uniform(unsigned int _uiShaderID, const char* _pUniformName, unsigned int&& _uiSlot = 0);
	void Bind() const;
	static void Unbind();

	static bool Empty();
	static unsigned int Size();
	static unsigned int MaxSize();
	static CTexture* At(unsigned int _uiID);
	static CTexture* Find(const char* _pName);
	static void Erase(unsigned int _uiID);
	static void Erase(std::string _strName);
	static void Clear();
};