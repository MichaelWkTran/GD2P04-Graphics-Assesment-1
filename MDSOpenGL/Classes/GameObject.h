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