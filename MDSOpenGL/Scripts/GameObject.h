/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: GameObject.h																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include "UpdatedObject.h"
#include "Transform.h"
#include "Mesh.h"
#include <set>

class CGameObject : public CUpdatedObject
{
public:
	//Refrencing variables
	std::set<const char*> m_tags;

	//Transformation variables
	CTransform m_transform;
	
	//Rendering variables
	CMesh<> m_mesh;
	bool m_visible;

	//Methods
	CGameObject();

	const bool TagExists(const char* _tagName);

	virtual void Draw() override;
};