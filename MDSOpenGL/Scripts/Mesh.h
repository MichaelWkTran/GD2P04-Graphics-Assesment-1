/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Mesh.h																										*/
/*  DATE: Aug 23rd, 2022																								*
/************************************************************************************************************************/

#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include <map>
#include <set>
#include <memory>
#include "Transform.h"

class CTexture;
class CShader;
class CCamera;

struct stVertex
{
	glm::vec3 v3Position;
	glm::vec3 v3Normal;
	glm::vec2 v2TextureUV;
	
	static void LinkAttributes()
	{
		glEnableVertexAttribArray(0 /*m_v3Position*/);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Position)));

		glEnableVertexAttribArray(1 /*m_v3Normal*/);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Normal)));

		glEnableVertexAttribArray(2 /*m_v2TextureCoord*/);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v2TextureUV)));
	}
};

class CBaseMesh
{
protected:
	bool m_bUpdateVertexArray;
	static std::set<CBaseMesh*> Meshes;

	CBaseMesh() { Meshes.emplace(this); m_bUpdateVertexArray = false; };
	~CBaseMesh() { Meshes.erase(this); };

public:
	static std::set<CBaseMesh*>::iterator GetMeshesBegin() { return Meshes.begin(); }
	static std::set<CBaseMesh*>::iterator GetMeshesEnd() { return Meshes.end(); }
	
	CVertexArray m_VertexArray;
	CElementBuffer m_ElementBuffer;
	CTransform m_Transform;
	std::map<const char* /*Uniform Name*/, std::shared_ptr<CTexture>> m_mapTextures;
	std::shared_ptr<CShader> m_pShader;
	std::shared_ptr<CShader> m_pShadowShader;
	
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);
	virtual void Draw(const CCamera& _Camera) = 0;
};

template <class T = stVertex>
class CMesh : public CBaseMesh
{
public:
	CVertexBuffer<T> m_VertexBuffer;
	void (*m_pDrawMethod)(CMesh<T>& _Mesh);

	CMesh();
	CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::map<const char*, std::shared_ptr<CTexture>>& _mapTextures, std::shared_ptr<CShader> _pShader = nullptr);

	void UpdateVertexArray();
	const std::vector<T> GetVerticies() const;
	void SetVerticies(const std::vector<T> _vVerticies);
	
	virtual void Draw(const CCamera& _Camera) override;
};