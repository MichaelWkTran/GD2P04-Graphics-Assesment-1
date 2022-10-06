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

class CTransform;
class CTexture;
class CShader;
class CCamera;

struct stVertex
{
	glm::vec3 v3Position;
	glm::vec3 v3Normal;
	glm::vec2 v2TextureUV;
	
	static void LinkAttributes(CVertexArray& _VertexArray, CVertexBuffer<stVertex>& _VertexBuffer, CElementBuffer* _pElementBuffer = nullptr)
	{
		_VertexArray.Bind(); _VertexBuffer.Bind(); if (_pElementBuffer) _pElementBuffer->Bind();

		_VertexArray.LinkAttribute(0 /*m_v3Position*/,	   3, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Position)));
		_VertexArray.LinkAttribute(1 /*m_v3Normal*/,	   3, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Normal)));
		_VertexArray.LinkAttribute(2 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v2TextureUV)));

		_VertexArray.Unbind(); _VertexBuffer.Unbind(); if (_pElementBuffer) _pElementBuffer->Unbind();
	}
};

template <class T = stVertex>
class CMesh
{
protected:
	CVertexArray m_VertexArray;
	CElementBuffer m_ElementBuffer;
	CVertexBuffer<T> m_VertexBuffer;
	bool m_bUpdateVertexArray;

public:
	CTransform* m_pTransform;
	std::map<const char* /*Uniform Name*/, CTexture*> m_mapTextures;
	CShader* m_pShader;
	void (*m_pDrawMethod)(CMesh<T>& _Mesh);

	CMesh();
	CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::map<const char*, CTexture*>& _mapTextures, CShader* _pShader);

	void BindVertexArray();
	void UnbindVertexArray();
	const std::vector<T> GetVerticies() const;
	void SetVerticies(const std::vector<T> _vVerticies);
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);
	
	virtual void Draw(const CCamera& _Camera);
};