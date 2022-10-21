#include "Player.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"
#include "AssesmentGameManager.h"
#include "Main.h"

CPlayer::CPlayer()
{
	gm::GenerateCube(m_Mesh);
	m_Mesh.m_pShader = (*dynamic_cast<CAssesmentGameManager*>(&GetGameManager())->m_mapShaders.find("Diffuse")).second;
}

void CPlayer::Update()
{
	float fSpeed = 10.0f;
	
	//Handles key inputs
	glm::vec3 v3Velocity(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(e_pMainWindow, GLFW_KEY_W) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * m_Transform.Forward();
	if (glfwGetKey(e_pMainWindow, GLFW_KEY_S) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * -m_Transform.Forward();
	if (glfwGetKey(e_pMainWindow, GLFW_KEY_A) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * -m_Transform.Right();
	if (glfwGetKey(e_pMainWindow, GLFW_KEY_D) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * m_Transform.Right();
	if (glfwGetKey(e_pMainWindow, GLFW_KEY_Q) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * m_Transform.Up();
	if (glfwGetKey(e_pMainWindow, GLFW_KEY_E) == GLFW_PRESS) v3Velocity += e_fDeltatime * fSpeed * -m_Transform.Up();

	m_Transform.Move(v3Velocity);
}
