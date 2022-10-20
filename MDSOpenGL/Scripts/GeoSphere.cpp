#include "GeoSphere.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"
#include "AssesmentGameManager.h"

CShader* CGeoSphere::m_pNormalShader = nullptr;

CGeoSphere::CGeoSphere()
{
	gm::GenerateSphere(m_Mesh, 0.5f, 20);
	m_Mesh.m_pShader = (*dynamic_cast<CAssesmentGameManager*>(&GetGameManager())->m_mapShaders.find("Diffuse")).second;
	
	//Create normal shader
	if (m_pNormalShader == nullptr) m_pNormalShader = new CShader("Normal.vert", "Normal.geom", "Normal.frag");
}

void CGeoSphere::Draw()
{
	//Draw Mesh
	CGameObject::Draw();

	//Draw Normals
	m_pNormalShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, GetMainCamera().GetCameraMatrix());
	m_pNormalShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, m_Transform.GetModel());

	m_pNormalShader->Activate();
	m_Mesh.m_VertexArray.Bind();
	
	glDrawArrays(GL_POINTS, 0, m_Mesh.GetVerticies().size());
	
	CVertexArray::Unbind();
	m_pNormalShader->Deactivate();
}