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

