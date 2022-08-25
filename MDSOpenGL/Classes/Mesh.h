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
	
	static void LinkAttributes(CVertexArray* _pVertexArray, CVertexBuffer<stVertex>* _pVertexBuffer, CElementBuffer* _pElementBuffer)
	{
		if (_pVertexArray == nullptr || _pVertexBuffer == nullptr || _pElementBuffer == nullptr) return;

		_pVertexArray->Bind(); _pVertexBuffer->Bind(); _pElementBuffer->Bind();

		_pVertexArray->LinkAttribute(0 /*m_v3Position*/,	 3, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Position)));
		_pVertexArray->LinkAttribute(1 /*m_v3Normal*/,		 3, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v3Normal)));
		_pVertexArray->LinkAttribute(2 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(offsetof(stVertex, stVertex::v2TextureUV)));

		_pVertexArray->Unbind(); _pVertexBuffer->Unbind(); _pElementBuffer->Unbind();
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