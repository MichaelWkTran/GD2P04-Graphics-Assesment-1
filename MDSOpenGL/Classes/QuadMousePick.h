#pragma once
#include "GameObject.h"

class CQuadMousePick : CGameObject
{
private:
	glm::vec2 m_v2Size;

public:
	bool m_bIsIntersecting;

	CQuadMousePick(glm::vec2 _v2Size);

	void Update() override;
};