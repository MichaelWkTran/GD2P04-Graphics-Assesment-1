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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CCamera()
//	 Purpose: Initalises variables of the created CCamera

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

//------------------------------------------------------------------------------------------------------------------------
// Procedure: ~CCamera()
//	 Purpose: Ensures that m_pMainCamera never contains garbage data if it is destroyed

CCamera::~CCamera()
{
	//Set m_pMainCamera to null if it is destroyed
	if (m_pMainCamera == this) m_pMainCamera == nullptr;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: MainCamera()
//	 Purpose: Gets the main camera if requested. If a main camera does not exist, create a free camera as the main camera
//	 Returns: The current main camera

CCamera& CCamera::MainCamera()
{
	//If a main camera does not exist, create a free camera
	if (m_pMainCamera == nullptr)
	{
		m_pMainCamera = new CFreePlayerCamera();
	}

	return *m_pMainCamera;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetViewMatrix()
//	 Purpose: Gets the view matrix of the camera
//	 Returns: The view matrix of the camera

const glm::mat4 CCamera::GetViewMatrix() const
{
	return m_mat4View;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetProjectionMatrix()
//	 Purpose: Gets the projection matrix of the camera
//	 Returns: The projection matrix of the camera

const glm::mat4 CCamera::GetProjectionMatrix() const
{
	return m_mat4Projection;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetCameraMatrix()
//	 Purpose: Gets the camera matrix of the camera
//	 Returns: The camera matrix of the camera

const glm::mat4 CCamera::GetCameraMatrix() const
{
	return m_mat4Camera;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetViewPort()
//	 Purpose: Gets the viewport size of the camera
//	 Returns: The viewport size of the camera

const glm::uvec2 CCamera::GetViewPort() const
{
	return m_uv2ViewPort;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetViewPort()
//	 Purpose: Sets the viewport size of the camera where the passed in _uv2ViewPort is the new viewport

void CCamera::SetViewPort(const glm::uvec2 _uv2ViewPort)
{
	m_uv2ViewPort = _uv2ViewPort;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetProjection()
//	 Purpose: Gives a boolean that shows the view mode of the camera. If the boolean is true, the camera is in perspective mode, otherwise, it is in orthographic mode
//	 Returns: True if the camera is in perspective mode, otherwise false if it is in orthographic mode

const bool CCamera::GetProjection() const
{
	return m_bIsPerspective;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetProjection()
//	 Purpose: Sets the view mode of the camera. If the boolean given is true, the camera is set to perspective mode, otherwise, it is set to orthographic mode

void CCamera::SetProjection(const bool _bIsPerspective)
{
	m_bIsPerspective = _bIsPerspective;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetFieldOfView()
//	 Purpose: Gets the field of view of the camera. The function will return the field of view in radians if _bInRadians is true,
//			  otherwise it will return the field of view in degrees.
//	 Returns: The field of view in radians if _bInRadians is true, otherwise it will return the field of view in degrees.

const float CCamera::GetFieldOfView(const bool _bInRadians)
{
	if (_bInRadians) return glm::radians(m_fFieldOfView);
	else return m_fFieldOfView;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetFieldOfView()
//	 Purpose: Sets the new field of view of the camera, _fFieldOfView. If the field of view is in radians, then _bIsRadians must be set to true.

void CCamera::SetFieldOfView(const float _fFieldOfView, const bool _bIsRadians)
{
	m_fFieldOfView = _bIsRadians ? glm::degrees(_fFieldOfView) : _fFieldOfView;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetOrthographicScale()
//	 Purpose: Gets the orthographic scale of the camera.
//	 Returns: The orthographic scale of the camera

const float CCamera::GetOrthographicScale() const
{
	return m_fOrthographicScale;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetOrthographicScale()
//	 Purpose: Sets the new orthographic scale, _fOrthographicScale, of the camera.

void CCamera::SetOrthographicScale(const float _fOrthographicScale)
{
	m_fOrthographicScale = _fOrthographicScale;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetNearPlane()
//	 Purpose: Gets the near plane distance of the camera
//	 Returns: The near plane distance of the camera

const float CCamera::GetNearPlane() const
{
	return m_fNearPlane;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetNearPlane()
//	 Purpose: Sets the new near plane distance, _fNearPlane, of the camera

void CCamera::SetNearPlane(const float _fNearPlane)
{
	m_fNearPlane = _fNearPlane;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: GetFarPlane()
//	 Purpose: Gets the far plane distance of the camera
//	 Returns: The far plane distance of the camera

const float CCamera::GetFarPlane() const
{
	return m_fFarPlane;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: SetFarPlane()
//	 Purpose: Sets the new far plane distance of the camera, _fFarPlane.

void CCamera::SetFarPlane(const float _fFarPlane)
{
	m_fFarPlane = _fFarPlane;
	m_bUpdateProjectionMatrix = true;
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: void CCamera::Update()
//	 Purpose: Updates the projection, view and camera matricies

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