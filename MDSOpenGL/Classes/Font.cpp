/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Font.cpp																										*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Font.h"
#include <iostream>
#include "Texture.h"

CFont::CFont(const char* _pFont, glm::ivec2 _v2iPixelSize)
{
	m_bInitialized = false;
	FT_Library FontLibrary;
	FT_Face FontFace;

	//Initializing the Library object
	if (FT_Init_FreeType(&FontLibrary) != 0)
	{
		std::cout << "FreeType Error: Could not init FreeType Library";
		return;
	}
	//Loading the font as face
	if (FT_New_Face(FontLibrary, _pFont, 0, &FontFace) != 0)
	{
		std::cout << "FreeType Error: Failed to load font";
		return;
	}

	FT_Set_Pixel_Sizes(FontFace, _v2iPixelSize.x, _v2iPixelSize.y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load the characters of the font up to the font character limit
	for (GLubyte Glyph = 0; Glyph < m_iFontCharacterLimit; Glyph++)
	{
		//Load the character glyph into face
		if (FT_Load_Char(FontFace, Glyph, FT_LOAD_RENDER))
		{
			std::cout << "FreeType Error: Failed to load Glyph: " << (unsigned char)Glyph << std::endl;
			continue;
		}

		auto GenerateTexture = [&]() -> CTexture*
		{
			CTexture* pTexture = new CTexture("", 0);
			pTexture->Bind();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, FontFace->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			return pTexture;
		};

		stFontChar FontCharacter =
		{
			GenerateTexture(),
			glm::ivec2(FontFace->glyph->bitmap.width,
			FontFace->glyph->bitmap.rows),
			glm::ivec2(FontFace->glyph->bitmap_left,
			FontFace->glyph->bitmap_top),
			(unsigned int)FontFace->glyph->advance.x / 64
		};

		m_mapCharacter.insert(std::pair<GLchar, stFontChar>(Glyph, FontCharacter));
	}
	CTexture::Unbind();

	//Destroy FontLibrary and FontFace
	FT_Done_Face(FontFace);
	FT_Done_FreeType(FontLibrary);

	//Set Initialized
	m_bInitialized = true;
};
