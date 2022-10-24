/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: GameObject.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "GameObject.h"
#include "Camera.h"

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GameObject()
//	 Purpose: To initalise variables of the game object

CGameObject::CGameObject()
{
	m_visible = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: TagExists()
//	 Purpose: Check whether the tag, _pTagName, exists in m_setTags 
//	 Returns: Whether the tag, _pTagName, exists in in m_setTags

const bool CGameObject::TagExists(const char* _tagName)
{
	if (m_tags.find(_tagName) != m_tags.end()) return true;
	else return false;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Draw()
//	 Purpose: To draw the mesh stored in the game object

void CGameObject::Draw()
{
	m_mesh.m_transform = m_transform;
	m_mesh.Draw(GetMainCamera());
}