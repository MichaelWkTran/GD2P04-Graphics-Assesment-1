/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: GameObject.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject()
{
	m_Mesh.m_pTransform = &m_Transform;
	m_bVisible = true;
}

const bool GameObject::TagExists(const char* _pTagName)
{
	if (m_setTags.find(_pTagName) != m_setTags.end()) return true;
	else return false;
}

void GameObject::Draw()
{
	m_Mesh.Draw(GetMainCamera());
}