/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Camera.cpp																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Camera.h"
#include "Main.h"
#include "GameManager.h"
#include "FreePlayerCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


CCamera* CCamera::m_pMainCamera = nullptr;

CCamera::CCamera()
{
	//Set the main camera if not assigned yet
	if (m_pMainCamera == nullptr) m_pMainCamera = this;

	//Set camera viewport
	m_uv2ViewPort.x = e_uViewPortW;
	m_uv2ViewPort.y = e_uViewPortH;
	
	//Set camera view mode
	m_bIsPerspective = true;
	
	//Set view specefic variables
	m_fFieldOfView = 45.0f;
	m_fOrthographicScale = 1;
	
	//Set far and near plane
	m_fNearPlane = 0.1f;
	m_fFarPlane = 100.0f;

	//Set camera matrices
	m_mat4View = glm::mat4(1.0f);
	m_mat4Projection = glm::mat4(1.0f);
	m_mat4Camera = glm::mat4(1.0f);

	m_bUpdateProjectionMatrix = true;
}

CCamera::~CCamera()
{
	//Set m_pMainCamera to null if it is destroyed
	if (m_pMainCamera == this) m_pMainCamera == nullptr;
}

CCamera& CCamera::MainCamera()
{
	//If a main camera does not exist, create a free camera
	if (m_pMainCamera == nullptr)
	{
		m_pMainCamera = new CFreePlayerCamera();
	}

	return *m_pMainCamera;
}

const glm::mat4 CCamera::GetViewMatrix() const
{
	return m_mat4View;
}

const glm::mat4 CCamera::GetProjectionMatrix() const
{
	return m_mat4Projection;
}

const glm::mat4 CCamera::GetCameraMatrix() const
{
	return m_mat4Camera;
}

const glm::uvec2 CCamera::GetViewPort() const
{
	return m_uv2ViewPort;
}

void CCamera::SetViewPort(const glm::uvec2 _uv2ViewPort)
{
	m_uv2ViewPort = _uv2ViewPort;
	m_bUpdateProjectionMatrix = true;
}

/*Gives a boolean that shows the view mode of the camera. If the boolean is true, the camera is in perspective mode, otherwise, it is in orthographic mode*/
const bool CCamera::GetProjection() const
{
	return m_bIsPerspective;
}

/*If the boolean given is true, the camera is set to perspective mode, otherwise, it is set to orthographic mode*/
void CCamera::SetProjection(const bool _bIsPerspective)
{
	m_bIsPerspective = _bIsPerspective;
	m_bUpdateProjectionMatrix = true;
}

const float CCamera::GetFieldOfView(const bool _bInRadians)
{
	if (_bInRadians) return glm::radians(m_fFieldOfView);
	else return m_fFieldOfView;
}

void CCamera::SetFieldOfView(const float _fFOV, const bool _bIsRadians)
{
	m_fFieldOfView = _bIsRadians ? glm::degrees(_fFOV) : _fFOV;
	m_bUpdateProjectionMatrix = true;
}

const float CCamera::GetOrthographicScale() const
{
	return m_fOrthographicScale;
}

void CCamera::SetOrthographicScale(const float _fOrthographicScale)
{
	m_fOrthographicScale = _fOrthographicScale;
	m_bUpdateProjectionMatrix = true;
}

const float CCamera::GetNearPlane() const
{
	return m_fNearPlane;
}

void CCamera::SetNearPlane(const float _fNearPlane)
{
	m_fNearPlane = _fNearPlane;
	m_bUpdateProjectionMatrix = true;
}

const float CCamera::GetFarPlane() const
{
	return m_fFarPlane;
}

void CCamera::SetFarPlane(const float _fFarPlane)
{
	m_fFarPlane = _fFarPlane;
	m_bUpdateProjectionMatrix = true;
}

void CCamera::Update()
{
	if (m_bUpdateProjectionMatrix)
	{
		if (m_bIsPerspective)
		{
			m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView), ((float)m_uv2ViewPort.x) / ((float)m_uv2ViewPort.y), m_fNearPlane, m_fFarPlane);
		}
		else
		{
			float fHalfViewPortW = (((float)m_uv2ViewPort.x) / 2) / m_fOrthographicScale;
			float fHalfViewPortH = (((float)m_uv2ViewPort.y) / 2) / m_fOrthographicScale;

			m_mat4Projection = glm::ortho(fHalfViewPortW, -fHalfViewPortW, fHalfViewPortH, -fHalfViewPortH, m_fNearPlane, m_fFarPlane);
		}

		m_bUpdateProjectionMatrix = false;
	}

	//Update the view and camera matrices
	m_mat4View = glm::lookAt(m_Transform.GetPosition(), m_Transform.GetPosition() + m_Transform.Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
	m_mat4Camera = m_mat4Projection * m_mat4View;
}