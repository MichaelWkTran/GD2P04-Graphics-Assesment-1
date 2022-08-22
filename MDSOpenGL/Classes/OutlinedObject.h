#pragma once
#include "GameObject.h" 
#include <glm/glm.hpp>

class COutlinedObject : public CGameObject
{
public:
	float m_fOutLineThickness;
	glm::vec3 m_v3OutlineColour;

	COutlinedObject();
	virtual void Draw() override;
};

