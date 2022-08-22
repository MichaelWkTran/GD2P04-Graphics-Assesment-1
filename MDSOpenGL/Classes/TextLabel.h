#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include "UpdatedObject.h"
#include "Mesh.h"

class CFont;

class CTextLabel : CUpdatedObject
{
protected:
	glm::mat4 m_mat4Projection;
	
	CVertexArray m_VertexArray;
	CVertexBuffer<stVertex> m_VertexBuffer;
	CElementBuffer m_ElementBuffer;

	void UpdateSize();

public:
	std::string m_strText;
	CFont* m_pFont;
	glm::vec2 m_v2Position;
	unsigned int m_uHAlign; //0 = Left, 1 = Centre, 2 = Right
	unsigned int m_uVAlign; //0 = Top, 1 = Middle, 2 = Bottom
	glm::vec2 m_v2Size;
	glm::vec2 m_v2Scale;
	glm::vec3 m_v3Colour;
	CShader* m_pShader;
	
	CTextLabel(std::string _strText, CFont* _pFont, CShader* _pShader, glm::vec2 _v2Position);
	~CTextLabel() {}

	const glm::mat4& GetProjectionMatrix();
	virtual void Draw() override;
};