#include "GeoSphere.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"
#include "AssesmentGameManager.h"

CShader* CGeoSphere::m_normalShader = nullptr;

CGeoSphere::CGeoSphere()
{
	gm::GenerateSphere(m_mesh, 0.5f, 20);
	m_mesh.m_shader = (*dynamic_cast<CAssesmentGameManager*>(&GetGameManager())->m_shaders.find("Diffuse")).second;
	
	//Create normal shader
	if (m_normalShader == nullptr) m_normalShader = new CShader("Normal.vert", "Normal.geom", "Normal.frag");
}

void CGeoSphere::Draw()
{
	//Draw Mesh
	CGameObject::Draw();

	//Draw Normals
	m_normalShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, GetMainCamera().GetCameraMatrix());
	m_normalShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, m_transform.GetModel());

	m_normalShader->Activate();
	m_mesh.m_vertexArray.Bind();
	
	glDrawArrays(GL_POINTS, 0, m_mesh.GetVerticies().size());
	
	CVertexArray::Unbind();
	m_normalShader->Deactivate();
}