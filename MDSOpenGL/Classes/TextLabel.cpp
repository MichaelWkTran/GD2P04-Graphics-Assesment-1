#include "TextLabel.h"
#include "../Main.h"
#include <iostream>
#include "Font.h"
#include "Texture.h"
#include "Shader.h"

void CTextLabel::UpdateSize()
{
	m_v2Size = glm::vec2();
	
	if (m_strText.empty()) return void();
	
	if (m_uHAlign != 0 || m_uVAlign < 2)
	{
		for (std::string::const_iterator TextCharacter = m_strText.begin(); TextCharacter != m_strText.end(); TextCharacter++)
		{
			auto FontCharacter = m_pFont->m_mapCharacter[*TextCharacter];
			m_v2Size.x += FontCharacter.GLuAdvance;
			m_v2Size.y += (float)FontCharacter.v2iBearing.y;
		}

		m_v2Size.y /= (float)m_strText.size();

		m_v2Size.x *= m_v2Scale.x;
		m_v2Size.y *= m_v2Scale.y;
	}
}

CTextLabel::CTextLabel
(
	std::string _strText,
	CFont* _pFont,
	CShader* _pShader,
	glm::vec2 _v2Position
)
{
	m_strText = _strText;
	m_pFont = _pFont;
	m_pShader = _pShader;
	m_v2Position = _v2Position;
	m_uHAlign = 0;
	m_uVAlign = 0;
	m_v2Scale = glm::vec2(1.0f, 1.0f);
	m_v3Colour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_mat4Projection = glm::ortho(0.0f, (float)e_uViewPortW, 0.0f, (float)e_uViewPortH, 0.0f, 100.0f);
	
	//Update m_v2Size
	UpdateSize();

	//Configure the VAO and VBO for texture quads
	m_ElementBuffer.SetIndicies
	(
		std::vector<unsigned int>
		{
			0, 3, 1,
			0, 2, 3
		}
	);

	m_VertexArray.Bind(); m_VertexBuffer.Bind(); m_ElementBuffer.Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
	
	m_VertexArray.LinkAttribute(0, 4, GL_FLOAT, 4 * sizeof(float), (void*)0);
	
	m_VertexBuffer.Unbind(); m_VertexArray.Unbind(); m_ElementBuffer.Unbind();
}

const glm::mat4& CTextLabel::GetProjectionMatrix()
{
	return m_mat4Projection;
}

void CTextLabel::Draw()
{
	if (m_pShader == nullptr) return;

	//Bind VAO and Shader
	m_pShader->Activate(); m_VertexArray.Bind();
	
	//Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3fv(glGetUniformLocation(m_pShader->GetID(), "uni_v3Color"), 1, glm::value_ptr(m_v3Colour));
	m_pShader->UniformMatrix4fv("uni_mat4Projection", 1, GL_FALSE, m_mat4Projection);

	// Update text width and height
	UpdateSize();
	
	// Store Character Origin that can change as we write each character. Keep the starting text position intact.
	glm::vec2 v2CharacterOrigin = m_v2Position; 
	switch (m_uHAlign)
	{
	case 0: break;
	case 1:
	{
		v2CharacterOrigin.x += -m_v2Size.x / 2.0f;
		break;
	}
	default:
	{
		v2CharacterOrigin.x += -m_v2Size.x;
	}
	}
	
	switch (m_uVAlign)
	{
	case 0:
	{
		v2CharacterOrigin.y += -m_v2Size.y;
		break;
	}
	case 1:
	{
		v2CharacterOrigin.y += -m_v2Size.y/2.0f; 
		break;
	}
	}

	// Iterate through the text characters and draw them
	for (std::string::const_iterator TextCharacter = m_strText.begin(); TextCharacter != m_strText.end(); TextCharacter++)
	{
		auto FontCharacter = m_pFont->m_mapCharacter[*TextCharacter];
		
		float fPosX = v2CharacterOrigin.x + FontCharacter.v2iBearing.x * m_v2Scale.x;
		float fPosY = v2CharacterOrigin.y - (FontCharacter.v2iSize.y - FontCharacter.v2iBearing.y) * m_v2Scale.y;
		
		float fWidth = FontCharacter.v2iSize.x * m_v2Scale.x;
		float fHeight = FontCharacter.v2iSize.y * m_v2Scale.y;

		// Update VB0 for each character
		float GLuVertices[4][4] =
		{
			//Coordinates					 /**/ Texture Cordinates
			fPosX,          fPosY + fHeight, /**/ 0.0f, 0.0f,
			fPosX + fWidth, fPosY + fHeight, /**/ 1.0f, 0.0f,
			fPosX,          fPosY,           /**/ 0.0f, 1.0f,
			fPosX + fWidth, fPosY,           /**/ 1.0f, 1.0f
		};
		
		// Reload the vertex array to the VB0
		m_VertexBuffer.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLuVertices), GLuVertices);

		// Render the glyph texture over the quad
		FontCharacter.pTexture->Bind();
		glUniform1i(glGetUniformLocation(m_pShader->GetID(), "uni_samp2DTextTexture"), 0);
		glDrawElements(GL_TRIANGLES, m_ElementBuffer.GetIndicies().size(), GL_UNSIGNED_INT, 0);

		// Move the position of the origin point for the next glyph by the advance
		v2CharacterOrigin.x += FontCharacter.GLuAdvance * m_v2Scale.x;
	}
	
	m_VertexBuffer.Unbind(); m_VertexArray.Unbind(); CTextureManager::Unbind(); m_pShader->Deactivate();
	glDisable(GL_BLEND);
}