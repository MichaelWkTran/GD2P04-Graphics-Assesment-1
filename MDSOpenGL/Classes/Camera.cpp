#include "Camera.h"
#include "../Main.h"
#include "GameManager.h"
#include "FreePlayerCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


CCamera* CCamera::m_pMainCamera = nullptr;

CCamera::CCamera()
{
	if (m_pMainCamera == nullptr) m_pMainCamera = this;

	m_uv2ViewPort.x = e_uViewPortW;
	m_uv2ViewPort.y = e_uViewPortH;
	
	m_bIsPerspective = true;
	
	m_fFOV = 45.0f;
	m_fOrthographicScale = 1;
	
	m_fNearPlane = 0.1f;
	m_fFarPlane = 100.0f;

	m_mat4View = glm::mat4(1.0f);
	m_mat4Projection = glm::mat4(1.0f);
	m_mat4Camera = glm::mat4(1.0f);

	m_bUpdateProjectionMatrix = true;
}

CCamera& CCamera::MainCamera()
{
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

const bool CCamera::GetProjection() const
{
	return m_bIsPerspective;
}

void CCamera::SetProjection(const bool _bIsPerspective)
{
	m_bIsPerspective = _bIsPerspective;
	m_bUpdateProjectionMatrix = true;
}

const float CCamera::GetFOV(const bool _InRadians/* = false*/)
{
	if (_InRadians) return glm::radians(m_fFOV);
	else return m_fFOV;
}

void CCamera::SetFOV(const float _FOV, const bool _IsRadians/* = false*/)
{
	m_fFOV = _IsRadians ? glm::degrees(_FOV) : _FOV;
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
			m_mat4Projection = glm::perspective(glm::radians(m_fFOV), ((float)m_uv2ViewPort.x) / ((float)m_uv2ViewPort.y), m_fNearPlane, m_fFarPlane);
		}
		else
		{
			float fHalfViewPortW = (((float)m_uv2ViewPort.x) / 2) / m_fOrthographicScale;
			float fHalfViewPortH = (((float)m_uv2ViewPort.y) / 2) / m_fOrthographicScale;

			m_mat4Projection = glm::ortho(fHalfViewPortW, -fHalfViewPortW, fHalfViewPortH, -fHalfViewPortH, m_fNearPlane, m_fFarPlane);
		}

		m_bUpdateProjectionMatrix = false;
	}

	m_mat4View = glm::lookAt(m_Transform.GetPosition(), m_Transform.GetPosition() + m_Transform.Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
	m_mat4Camera = m_mat4Projection * m_mat4View;
}