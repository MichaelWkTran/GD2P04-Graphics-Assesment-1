#include "CTessModel.h"

CTessModel::CTessModel()
{
	m_bUpdateVertexArray = false;

	//Set Mesh Verticies
	std::vector<stVertex> vVerticies =
	{
		stVertex {glm::vec3(-1.0f,-1.0f, 0.0f)},
		stVertex {glm::vec3( 1.0f,-1.0f, 0.0f)},
		stVertex {glm::vec3( 1.0f, 1.0f, 0.0f)},
		stVertex {glm::vec3(-1.0f, 1.0f, 0.0f)},
	};
	m_Mesh.SetVerticies(vVerticies);

	//Set Mesh Draw Method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh)
	{
		glDrawArrays(GL_PATCHES, 0, _Mesh.GetVerticies().size());
	};

	m_Mesh.BindVertexArray(); m_Mesh.BindVertexArray(); m_Mesh.BindElementBuffer();
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	stVertex::LinkAttributes();
	CVertexArray::Unbind(); CVertexBuffer<stVertex>::Unbind(); CElementBuffer::Unbind();
}

void CTessModel::Draw()
{
	
}


