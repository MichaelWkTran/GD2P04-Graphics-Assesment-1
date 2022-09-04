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

class GameObject : public CUpdatedObject
{
public:
	//Refrencing variables
	std::set<const char*> m_setTags;

	//Transformation variables
	CTransform m_Transform;
	
	//Rendering variables
	CMesh<> m_Mesh;
	bool m_bVisible;

	//Methods
	GameObject();

	const bool TagExists(const char* _pTagName);

	virtual void Draw() override;
};