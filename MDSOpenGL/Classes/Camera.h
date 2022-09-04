#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "UpdatedObject.h"
#include "Transform.h"

/*The camera object allows the user to view the game world.*/
class CCamera : public CUpdatedObject
{
protected:
	glm::uvec2 m_uv2ViewPort;
	bool m_bIsPerspective;
	float m_fFieldOfView; //Value is in degrees
	float m_fOrthographicScale;
	float m_fNearPlane, m_fFarPlane;

	bool m_bUpdateProjectionMatrix;
	glm::mat4 m_mat4View, m_mat4Projection, m_mat4Camera;

public:
	static CCamera* m_pMainCamera;
	CTransform m_Transform;

	CCamera();
	~CCamera();
	static CCamera& MainCamera();

	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;
	const glm::mat4 GetCameraMatrix() const;
	const glm::uvec2 GetViewPort() const; void SetViewPort(const glm::uvec2 _uv2ViewPort);
	const bool GetProjection() const; void SetProjection(const bool _bIsPerspective);
	const float GetFieldOfView(bool _bInRadians = false); void SetFieldOfView(const float _fFOV, const bool _bIsRadians = false);
	const float GetOrthographicScale() const; void SetOrthographicScale(const float _fOrthographicScale);
	const float GetNearPlane() const; void SetNearPlane(const float _fNearPlane);
	const float GetFarPlane() const; void SetFarPlane(const float _fFarPlane);

	virtual void Update() override;
};

#define GetMainCamera CCamera::MainCamera