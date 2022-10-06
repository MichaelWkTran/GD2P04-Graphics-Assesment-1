#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

class CFrameBuffer
{
	unsigned int m_uiID;

public:
	CFrameBuffer();
	~CFrameBuffer();

	const unsigned int& GetID() { return m_uiID; };
	void Bind();
	static void Unbind();
};

