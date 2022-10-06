#include "FrameBuffer.h"
#include "Texture.h"
#include <glm/glm.hpp>

CFrameBuffer::CFrameBuffer()
{
	glGenFramebuffers(1, &m_uiID);
	Bind();
	
	{
		CTexture* pRenderTexture = new CTexture("FrameBuffer" + m_uiID, 0);
		pRenderTexture->Bind();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pRenderTexture->GetID(), 0);

		pRenderTexture->Unbind();
	}
	


	Unbind();
}

CFrameBuffer::~CFrameBuffer()
{
	glDeleteFramebuffers(1, &m_uiID);
}

void CFrameBuffer::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_uiID);
}

void CFrameBuffer::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
