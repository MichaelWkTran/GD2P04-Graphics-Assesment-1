#pragma once
#include "UpdatedObject.h"
#include "Transform.h"
#include "Mesh.h"
#include <set>

class CGameObject : public CUpdatedObject
{
public:
	//Refrencing variables
	std::set<const char*> m_strTags;

	//Transformation variables
	CTransform m_Transform;
	
	//Rendering variables
	CMesh<> m_Mesh;
	bool m_bVisible;

	//Methods
	CGameObject();

	const bool TagExists(const char* _strTagName);

	virtual void Draw() override;
};