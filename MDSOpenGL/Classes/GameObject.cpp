#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject()
{
	m_Mesh.m_pTransform = &m_Transform;
	m_bVisible = true;
}

const bool GameObject::TagExists(const char* _strTagName)
{
	if (m_strTags.find(_strTagName) != m_strTags.end()) return true;
	else return false;
}

void GameObject::Draw()
{
	m_Mesh.Draw(GetMainCamera());
}