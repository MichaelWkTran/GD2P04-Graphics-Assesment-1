#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<class T>
inline CMesh<T>::CMesh()
{
	m_pTransform = nullptr;
	m_pShader = nullptr;
	m_pDrawMethod = [](CMesh<T>& _Mesh)
	{
		glDrawElements(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT, 0);
	};
	m_bUpdateVertexArray = false;
}

template<class T>
inline CMesh<T>::CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::map<const char*, CTexture*>& _mapTextures, CShader* _pShader)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_mapTextures = _mapTextures;
	m_pShader = _pShader;
	m_pDrawMethod = [](CMesh<T>& _Mesh)
	{
		glDrawElements(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT, 0);
	};
	m_bUpdateVertexArray = true;
}

template<class T>
inline void CMesh<T>::BindVertexArray()
{
	m_VertexArray.Bind();
}

template<class T>
inline void CMesh<T>::UnbindVertexArray()
{
	m_VertexArray.Unbind();
}

template<class T>
inline const std::vector<T> CMesh<T>::GetVerticies() const
{
	return m_VertexBuffer.GetVertices();
}

template<class T>
inline void CMesh<T>::SetVerticies(const std::vector<T> _vVerticies)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_bUpdateVertexArray = true;
}

template<class T>
inline const std::vector<unsigned int> CMesh<T>::GetIndicies() const
{
	return m_ElementBuffer.GetIndicies();
}

template<class T>
inline void CMesh<T>::SetIndicies(const std::vector<unsigned int> _vIndicies)
{
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_bUpdateVertexArray = true;
}

template<class T>
inline void CMesh<T>::Draw(const CCamera& _Camera)
{
	//Dont draw if no shader is assigned or m_VertexBuffer have no verticies
	if (m_pShader == nullptr || m_VertexBuffer.GetVertices().empty()) return void();

	//Update vertex array if its vertex data has changed
	if (m_bUpdateVertexArray)
	{
		m_bUpdateVertexArray = false;
		T::LinkAttributes(&m_VertexArray, &m_VertexBuffer, &m_ElementBuffer);
	}

	//Set GameObject Uniform
	if (m_pTransform != nullptr)
	{
		m_pShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, m_pTransform->GetModel());
	}
	else
	{
		m_pShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, glm::mat4(1));
	}

	//Set Mesh Uniforms
	m_pShader->ResetUniforms();
	m_pShader->Uniform3f("uni_v3CameraPosition", _Camera.m_Transform.GetPosition());
	m_pShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, _Camera.GetCameraMatrix());

	//Set Texture Uniforms
	for (auto& pTexture : m_mapTextures)
	{
		if (pTexture.second == nullptr) continue;
		pTexture.second->Uniform(*m_pShader, pTexture.first);
	}
	CTexture::Unbind();

	//Draw Mesh
	m_VertexArray.Bind();
	m_pDrawMethod(*this);
	m_VertexArray.Unbind();
}

template class CMesh<stVertex>;