/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: VertexArray.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "VertexArray.h"

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Generate a vertex array.

CVertexArray::CVertexArray()
{
    glGenVertexArrays(1, &m_uiID);
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Delete the vertex array.

CVertexArray::~CVertexArray()
{
    glDeleteVertexArrays(1, &m_uiID);
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Get the vertex array ID.
//	 Returns: THe vertex array ID.

const unsigned int& CVertexArray::GetID()
{
    return m_uiID;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Assigns attributes to verticies.

void CVertexArray::LinkAttribute
(
    unsigned int _uiLayout,
    unsigned int _uiNumComponents,
    GLenum _GLeType,
    GLsizeiptr _Stride,
    void* _pOffset
)
{
    glEnableVertexAttribArray(_uiLayout);
    glVertexAttribPointer(_uiLayout, _uiNumComponents, _GLeType, GL_FALSE, _Stride, _pOffset);
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Bind vertex array

void CVertexArray::Bind()
{
    glBindVertexArray(m_uiID);
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetModel()
//	 Purpose: Unbinds vertex array

void CVertexArray::Unbind()
{
    glBindVertexArray(0);
}