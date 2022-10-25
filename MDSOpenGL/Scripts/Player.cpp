#include "Player.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "Shader.h"
#include "AssesmentGameManager.h"
#include "Main.h"

CPlayer::CPlayer()
{
	gm::GenerateCube(m_mesh);
	m_mesh.m_shader = (*dynamic_cast<CAssesmentGameManager*>(&GetGameManager())->m_shaders.find("Diffuse")).second;
}

void CPlayer::Update()
{
	float fSpeed = 10.0f;
	
	//Handles key inputs
	glm::vec3 v3Velocity(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(e_mainWindow, GLFW_KEY_W) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * m_transform.Forward();
	if (glfwGetKey(e_mainWindow, GLFW_KEY_S) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * -m_transform.Forward();
	if (glfwGetKey(e_mainWindow, GLFW_KEY_A) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * -m_transform.Right();
	if (glfwGetKey(e_mainWindow, GLFW_KEY_D) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * m_transform.Right();
	if (glfwGetKey(e_mainWindow, GLFW_KEY_Q) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * m_transform.Up();
	if (glfwGetKey(e_mainWindow, GLFW_KEY_E) == GLFW_PRESS) v3Velocity += e_deltatime * fSpeed * -m_transform.Up();

	m_transform.Move(v3Velocity);
}
