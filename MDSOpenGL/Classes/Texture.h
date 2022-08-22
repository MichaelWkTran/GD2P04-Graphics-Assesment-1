#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <map>
#include <string>

class CShader;
class CTexture;

class CTextureManager
{
private:
	static std::map<const char* /*Texture name*/, CTexture*> m_mapTextures;

public:
	static bool Empty();
	static unsigned int Size();
	static unsigned int MaxSize();

	CTexture* operator[](unsigned int _uiID);
	static CTexture* At(unsigned int _uiID);
	static CTexture* Find(const char* _strName);

	static CTexture* Insert(const char* _pName, const char* _pType, unsigned int&& _uiSlot, GLenum&& _GLeTarget = GL_TEXTURE_2D);
	static CTexture* Insert(const char* _pName, const char* _pImage, const char* _pType, unsigned int&& _uiSlot, GLenum&& _GLeFormat, GLenum&& _GLePixelType);
	static void Erase(unsigned int _uiID);
	static void Erase(const char* _strName);
	static void Clear();

	static void Bind(unsigned int _uiID);
	static void Unbind();
};

class CTexture
{
	friend CTextureManager;

private:
	unsigned int m_uiID;
	
	CTexture(const char* _pType, GLenum&& _GLeTarget, unsigned int&& _uiSlot);
	~CTexture();

public:
	const char* m_pType;
	GLenum m_GLeTarget;
	unsigned int m_uiUnit;
	
	CTexture(CTexture const&) = delete;
	CTexture& operator=(const CTexture&) = delete;

	operator int() const;
	explicit operator int* ();
	const unsigned int GetID() const;
	void Uniform(unsigned int _uiShaderID, std::string _strUniformName);
	void Bind() const;
	static void Unbind();
};