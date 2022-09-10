/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Mesh.h																										*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CMesh()
//	 Purpose: Initalises variables of the mesh

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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CMesh()
//	 Purpose: Initalises variables of the mesh._vVertices is the number of vertices of the mesh.
//			  _vIndices is the number of indices in the element buffer.
//			  _mapTextures is the textures stored in the mesh.
//			  _pShader is the shader used by the mesh. 

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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: BindVertexArray()
//	 Purpose: Binds the vertex array of the mesh

template<class T>
inline void CMesh<T>::BindVertexArray()
{
	m_VertexArray.Bind();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: UnbindVertexArray()
//	 Purpose: Unbinds the mesh vertex array

template<class T>
inline void CMesh<T>::UnbindVertexArray()
{
	m_VertexArray.Unbind();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetVerticies()
//	 Purpose: Gets the vertices of the mesh.
//	 Returns: The vertices of the mesh.

template<class T>
inline const std::vector<T> CMesh<T>::GetVerticies() const
{
	return m_VertexBuffer.GetVertices();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetVerticies()
//	 Purpose: Sets the vertices of the mesh via _vVertices.

template<class T>
inline void CMesh<T>::SetVerticies(const std::vector<T> _vVerticies)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_bUpdateVertexArray = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetIndicies()
//	 Purpose: Gets the indices stored in the mesh.
//	 Returns: The indices stored in the mesh.

template<class T>
inline const std::vector<unsigned int> CMesh<T>::GetIndicies() const
{
	return m_ElementBuffer.GetIndicies();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetIndicies()
//	 Purpose: Sets the indices stored in the mesh via _vIndices.

template<class T>
inline void CMesh<T>::SetIndicies(const std::vector<unsigned int> _vIndicies)
{
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_bUpdateVertexArray = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Draw()
//	 Purpose: Draws the mesh using the camera matrix from _Camera

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
	if (m_pTransform != nullptr) m_pShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, m_pTransform->GetModel());
	else m_pShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, glm::mat4(1));

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

	//Draw Mesh
	m_pShader->Activate();
	m_VertexArray.Bind();
	m_pDrawMethod(*this);
	m_VertexArray.Unbind();
	CTexture::Unbind();
	m_pShader->Deactivate();
}

template class CMesh<stVertex>;