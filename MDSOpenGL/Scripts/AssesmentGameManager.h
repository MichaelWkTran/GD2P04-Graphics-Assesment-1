/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.h																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

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

