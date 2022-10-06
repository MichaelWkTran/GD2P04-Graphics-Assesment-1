#include "GeoSphere.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"


CGeoSphere::CGeoSphere()
{
	gm::GenerateSphere(m_Mesh, 0.5f, 10);
	m_Mesh.m_pShader = CShader::Find("Diffuse");
	m_Mesh.m_pTransform = &m_Transform;
}

void CGeoSphere::Draw()
{
	//Draw Mesh
	GameObject::Draw();

	//Draw Normals
	CShader* pNormalShader = CShader::Find("Normal");
	if (!pNormalShader) pNormalShader = new CShader("Normal", "Star.vert", "Star.geom", "Star.frag");

	pNormalShader->Activate();
	m_Mesh.BindVertexArray();

	glDrawArrays(GL_POINTS, 0, m_Mesh.GetVerticies().size());

	m_Mesh.UnbindVertexArray();
	pNormalShader->Deactivate();
}