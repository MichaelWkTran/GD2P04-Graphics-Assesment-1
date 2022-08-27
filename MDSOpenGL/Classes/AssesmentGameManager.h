#pragma once
#include "GameManager.h"

class CAssesmentGameManager : public CGameManager
{
public:
	bool m_bScissorTestEnabled;
	bool m_bStencilTestEnabled;

	CAssesmentGameManager();

	void Update() override;
};

