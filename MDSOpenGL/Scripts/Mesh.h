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
#include <memory>

class CTransform;
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
	std::map<const char* /*Uniform Name*/, std::shared_ptr<CTexture>> m_mapTextures;
	std::shared_ptr<CShader> m_pShader;
	void (*m_pDrawMethod)(CMesh<T>& _Mesh);

	CMesh();
	CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::map<const char*, std::shared_ptr<CTexture>>& _mapTextures, std::shared_ptr<CShader> _pShader = nullptr);

	void BindVertexArray();
	void BindElementBuffer();
	void BindVertexBuffer();
	const std::vector<T> GetVerticies() const;
	void SetVerticies(const std::vector<T> _vVerticies);
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);
	
	virtual void Draw(const CCamera& _Camera);
};