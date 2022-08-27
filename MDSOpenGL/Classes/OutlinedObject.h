#pragma once
#include "GameObject.h" 
#include <glm/glm.hpp>

class COutlinedObject : public CGameObject
{
public:
	float m_fOutLineThickness;
	glm::vec3 m_v3OutlineColour;

	COutlinedObject(float _fOutLineThickness = 1.0f, glm::vec3 _v3OutlineColour = {});
	void Update() override;
	void Draw() override;
};

