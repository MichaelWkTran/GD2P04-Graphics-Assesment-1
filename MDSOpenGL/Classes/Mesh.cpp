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
inline CMesh<T>::CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _vTextures, CShader* _pShader)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_vTextures = _vTextures;
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
	m_pShader->Uniform3f("uni_v3CameraPosition", _Camera.m_Transform.GetPosition());
	m_pShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, _Camera.GetCameraMatrix());

	//Set Texture Uniforms
	unsigned int uNumDiffuse = 0, uNumSpecular = 0, uNumReflect = 0;
	for (auto& pTexture : m_vTextures)
	{
		//Get the name of the uniform in the format: uni_samp + "texture target" + "texture type" + "add array index"
		std::string strUniformName = pTexture->m_pType;

		//Add array number
		if (pTexture->m_pType == "Diffuse") strUniformName += std::to_string(uNumDiffuse++);
		else if (pTexture->m_pType == "Specular") strUniformName += std::to_string(uNumSpecular++);
		else if (pTexture->m_pType == "Reflect") strUniformName += std::to_string(uNumReflect++);
		else continue;

		//Set Uniform
		pTexture->Uniform(*m_pShader, strUniformName);
	}
	CTextureManager::Unbind();

	//Draw Mesh
	m_VertexArray.Bind();
	m_pDrawMethod(*this);
	m_VertexArray.Unbind();
}

template class CMesh<stVertex>;