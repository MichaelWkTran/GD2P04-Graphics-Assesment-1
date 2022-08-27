#include "OutlinedObject.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

COutlinedObject::COutlinedObject()
{
	m_fOutLineThickness = 1.0f;
	m_v3OutlineColour = glm::vec3(0.0f, 0.0f, 0.0f);
}

void COutlinedObject::Draw()
{
	//Enable stencil and set stencil operation
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//1st pass - Set current stencil value
	glStencilFunc(GL_ALWAYS, 1.0f, 0xFF);
	glStencilMask(0xFF);

	//Draw regular sized mesh
	m_Mesh.Draw(GetMainCamera());

	//2nd pass
	glStencilFunc(GL_NOTEQUAL, 1.0f, 0xFF);
	glStencilMask(0x00);

	//Set Outline Uniforms
	CShader* m_pUnlitShader = CShader::Find("Unlit");
	glm::mat4 mat4Model = glm::scale(m_Transform.GetModel(), (1.0f + m_fOutLineThickness) * glm::vec3(1.0f, 1.0f, 1.0f));
	m_pUnlitShader->UniformMatrix4fv("uni_mat4Model", 1, GL_FALSE, mat4Model);
	m_pUnlitShader->UniformMatrix4fv("uni_mat4CameraMatrix", 1, GL_FALSE, GetMainCamera().GetCameraMatrix());
	m_pUnlitShader->Uniform3f("uni_v3Colour", m_v3OutlineColour);

	//Draw outline
	m_Mesh.BindVertexArray();
	m_Mesh.m_pDrawMethod(m_Mesh);
	m_Mesh.UnbindVertexArray();

	//Disable stencil test
	glStencilMask(0x00);
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
}