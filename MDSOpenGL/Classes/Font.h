#pragma once
#include <glm/glm.hpp>
#include <FREETYPE/ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <map>

class CTexture;

class CFont
{
protected:
	static const int m_iFontCharacterLimit = 128;
	bool m_bInitialized = false;
	
public:
	struct stFontChar
	{
		CTexture* pTexture;
		glm::ivec2 v2iSize;
		glm::ivec2 v2iBearing;
		unsigned int GLuAdvance;
	};
	
	std::map<char, stFontChar> m_mapCharacter;
	
	CFont(const char* _pFont, glm::ivec2 _v2iPixelSize);
};

