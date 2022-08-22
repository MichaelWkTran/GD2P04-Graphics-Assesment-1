#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

template <class T>
class CVertexBuffer
{
protected:
	unsigned int m_uiID;
	std::vector <T> m_vVertices;

public:
	CVertexBuffer();
	CVertexBuffer(const std::vector<T> _vVerticies);
	CVertexBuffer(CVertexBuffer const&) = delete;
	~CVertexBuffer();

	const unsigned int& GetID();
	const std::vector<T> GetVertices() const;
	void SetVertices(const std::vector<T> _vVertices);
	void Bind();
	void Unbind();
};

template<class T>
inline CVertexBuffer<T>::CVertexBuffer()
{
	glGenBuffers(1, &m_uiID);
}

template<class T>
inline CVertexBuffer<T>::CVertexBuffer(const std::vector<T> _vVerticies)
{
	glGenBuffers(1, &m_uiID);
	SetVertices(_vVerticies);
}

template<class T>
inline CVertexBuffer<T>::~CVertexBuffer()
{
	glDeleteBuffers(1, &m_uiID);
}

template<class T>
inline const unsigned int& CVertexBuffer<T>::GetID()
{
	return m_uiID;
}

template<class T>
inline const std::vector<T> CVertexBuffer<T>::GetVertices() const
{
	return m_vVertices;
}

template<class T>
inline void CVertexBuffer<T>::SetVertices(const std::vector<T> _vVertices)
{
	m_vVertices = _vVertices;

	Bind();
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(T), m_vVertices.data(), GL_STATIC_DRAW);
	Unbind();
}

template<class T>
inline void CVertexBuffer<T>::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiID);
}

template<class T>
inline void CVertexBuffer<T>::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
