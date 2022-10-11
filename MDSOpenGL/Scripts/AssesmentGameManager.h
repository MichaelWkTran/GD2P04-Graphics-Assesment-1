/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.h																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "GameManager.h"
#include "Mesh.h"

class CTexture;

enum class FrameBufferEffect
{
	None,
	Rain,
	ChromaticAberration,
	CRT
};

class CAssesmentGameManager : public CGameManager
{
private:
	unsigned int m_uiFrameBuffer;
	unsigned int m_uiRenderBuffer;
	CMesh<> m_RenderQuad;

public:
	FrameBufferEffect m_FrameBufferEffect;

	CAssesmentGameManager();
	~CAssesmentGameManager();
	void Update() override;
};

