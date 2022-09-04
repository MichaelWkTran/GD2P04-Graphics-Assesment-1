/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: FreePlayerCamera.h																							*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "Camera.h"

class CFreePlayerCamera : public CCamera
{
public:
	GLFWwindow* m_pWindow;
	bool m_bFirstClick;

	CFreePlayerCamera();
	~CFreePlayerCamera() {};
	
	virtual void Update() override;
};

