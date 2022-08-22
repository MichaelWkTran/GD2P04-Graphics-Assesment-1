#include "GameObject.h"
#include "Camera.h"

CGameObject::CGameObject()
{
	m_Mesh.m_pTransform = &m_Transform;
	m_bVisible = true;
}

const bool CGameObject::TagExists(const char* _strTagName)
{
	if (m_strTags.find(_strTagName) != m_strTags.end()) return true;
	else return false;
}

void CGameObject::Draw()
{
	m_Mesh.Draw(GetMainCamera());
}