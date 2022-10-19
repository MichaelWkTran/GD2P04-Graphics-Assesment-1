#include "TessModel.h"
#include "Shader.h"

CTessModel::CTessModel()
{
	//Set Mesh Verticies
	std::vector<stVertex> vVerticies =
	{
		stVertex {glm::vec3(-1.0f,-1.0f, 0.0f)},
		stVertex {glm::vec3( 1.0f,-1.0f, 0.0f)},
		stVertex {glm::vec3( 1.0f, 1.0f, 0.0f)}
	};
	m_Mesh.SetVerticies(vVerticies);

	//Set Mesh Draw Method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh)
	{
		glDrawArrays(GL_PATCHES, 0, _Mesh.GetVerticies().size());
	};

	//Set Mesh Shader
	m_Mesh.m_pShader = std::make_shared<CShader>("Tessellation.vert", "Tessellation.tesc", "Tessellation.tese", "Tessellation.frag");

	//Set Mesh Attributes
	m_Mesh.BindVertexArray(); m_Mesh.BindVertexArray(); m_Mesh.BindElementBuffer();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	stVertex::LinkAttributes();
	CVertexArray::Unbind(); CVertexBuffer<stVertex>::Unbind(); CElementBuffer::Unbind();
}

void CTessModel::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	CGameObject::Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
