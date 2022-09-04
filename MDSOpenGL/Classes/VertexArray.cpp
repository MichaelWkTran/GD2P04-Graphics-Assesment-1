/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: VertexArray.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "VertexArray.h"

CVertexArray::CVertexArray()
{
    glGenVertexArrays(1, &m_uiID);
}

CVertexArray::~CVertexArray()
{
    glDeleteVertexArrays(1, &m_uiID);
}

const unsigned int& CVertexArray::GetID()
{
    return m_uiID;
}

void CVertexArray::LinkAttribute
(
    unsigned int _uiLayout,
    unsigned int _uiNumComponents,
    GLenum _GLeType,
    GLsizeiptr _Stride,
    void* _pOffset
)
{
    glVertexAttribPointer(_uiLayout, _uiNumComponents, _GLeType, GL_FALSE, _Stride, _pOffset);
    glEnableVertexAttribArray(_uiLayout);
}

void CVertexArray::Bind()
{
    glBindVertexArray(m_uiID);
}

void CVertexArray::Unbind()
{
    glBindVertexArray(0);
}