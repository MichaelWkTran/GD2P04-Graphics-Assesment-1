#include "FreePlayerCamera.h"
#include "../Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

const glm::vec3 v3Up = glm::vec3(0.0f, 1.0f, 0.0f);

CFreePlayerCamera::CFreePlayerCamera()
{
	m_pWindow = e_pMainWindow;
	m_bFirstClick = false;
}

void CFreePlayerCamera::Update()
{
	float fSpeed = 10.0f;
	float fSensitivity = 30.0f;
	
	//Handles key inputs
	glm::vec3 v3Velocity(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)            v3Velocity += fSpeed * m_Transform.Forward();
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)            v3Velocity += fSpeed * -m_Transform.Forward();
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)            v3Velocity += fSpeed * -glm::normalize(glm::cross(m_Transform.Forward(), v3Up));
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)            v3Velocity += fSpeed * glm::normalize(glm::cross(m_Transform.Forward(), v3Up));
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)        v3Velocity += fSpeed * v3Up;
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) v3Velocity += fSpeed * -v3Up;

	m_Transform.SetPosition(m_Transform.GetPosition() + (v3Velocity * e_fDeltatime));

	//Handles mouse inputs
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//Hides mouse cursor
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		//Prevents camera from jumping on the first click
		if (m_bFirstClick)
		{
			e_v2MousePosition = glm::vec2(e_uViewPortW / 2, e_uViewPortH / 2);
			glfwSetCursorPos(m_pWindow, e_v2MousePosition.x, e_v2MousePosition.y);
			m_bFirstClick = false;
		}

		//Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		//and then "transforms" them into degrees 
		float fRotX = fSensitivity * (e_v2MousePosition.x - (e_uViewPortW / 2)) * e_fDeltatime;
		float fRotY = fSensitivity * (e_v2MousePosition.y - (e_uViewPortH / 2)) * e_fDeltatime;
		
		//Calculates upcoming vertical change in the Orientation
		glm::vec3 v3Orientation = glm::rotate(m_Transform.Forward(), glm::radians(-fRotY), glm::normalize(glm::cross(m_Transform.Forward(), v3Up)));;

		//Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(v3Orientation, v3Up) - glm::radians(90.0f)) > glm::radians(85.0f))
		{
			v3Orientation = m_Transform.Forward();
		}

		//Rotates the Orientation left and right
		v3Orientation = glm::rotate(v3Orientation, glm::radians(-fRotX), v3Up);

		//Set New Orientation
		m_Transform.LookAt(m_Transform.GetPosition() + v3Orientation);

		//Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(m_pWindow, (e_uViewPortW / 2), (e_uViewPortH / 2));
		e_v2MousePosition = glm::vec2(e_uViewPortW / 2, e_uViewPortH / 2);
	}
	else if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		//Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		//Makes sure the next time the camera looks around it doesn't jump
		m_bFirstClick = true;
	}

	CCamera::Update();
}
