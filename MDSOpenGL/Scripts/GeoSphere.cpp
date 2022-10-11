#include "GeoSphere.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"

CShader* CGeoSphere::m_pNormalShader = nullptr;

CGeoSphere::CGeoSphere()
{
	gm::GenerateSphere(m_Mesh, 0.5f, 20);
	m_Mesh.m_pShader = CShader::Find("Diffuse");
	m_Mesh.m_pTransform = &m_Transform;

	//Create normal shader
	if (m_pNormalShader == nullptr) m_pNormalShader = new CShader("Normal", "Normal.vert", "Normal.geom", "Normal.frag");
}

void CGeoSphere::Draw()
{
	//Draw Mesh
	GameObject::Draw();

	//Draw Normals
	m_pNormalShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, GetMainCamera().GetCameraMatrix());

	m_pNormalShader->Activate();
	m_Mesh.BindVertexArray();

	glDrawArrays(GL_POINTS, 0, m_Mesh.GetVerticies().size());

	m_Mesh.UnbindVertexArray();
	m_pNormalShader->Deactivate();
}