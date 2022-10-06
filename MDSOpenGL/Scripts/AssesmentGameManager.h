/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.h																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "GameManager.h"

enum class FrameBufferEffect
{
	None,
	Rain,
	ChromaticAberration,
	CRT
};

class CAssesmentGameManager : public CGameManager
{
public:
	CAssesmentGameManager();
	void Update() override;
};

