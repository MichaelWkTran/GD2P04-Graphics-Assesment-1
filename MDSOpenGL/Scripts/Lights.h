/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: LightManager.h																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <set>
#include <memory>

class CShader;
class CTexture;

class CLight
{
private:
	static std::set<CLight*> m_setLightsInWorld;

protected:
	unsigned int m_uiFrameBuffer;
	glm::mat4 m_mat4Projection;
	bool m_bUpdateProjectionMatrix;

	virtual void UpdateProjectionMatrix() = 0;

public:
	static std::shared_ptr<CShader> m_pShadowMapShader;
	CTexture* m_pShadowMapTexture;
	static glm::vec4 m_v4AmbientColour;
	static void UpdateLightUniforms(CShader& _Shader);
	static void UpdateShadowUniforms();

	glm::vec4 m_v4LightColour;
	
	//Methods
	CLight(glm::vec4 _v4LightColour = glm::vec4(1.0f));
	virtual ~CLight();

	const glm::mat4 GetProjection()
	{
		return m_mat4Projection;
	}
	void SetProjection(const glm::mat4 _mat4Projection)
	{
		m_mat4Projection = _mat4Projection;
		m_bUpdateProjectionMatrix = true;
	}
	unsigned int GetFrameBuffer()
	{
		return m_uiFrameBuffer;
	}
};

class CInfinitePointLight : public CLight
{
private:
	glm::vec3 m_v3Position;

protected:
	virtual void UpdateProjectionMatrix() override
	{
		
	}

public:
	CInfinitePointLight(glm::vec3 _v3Position = {}, glm::vec4 _v4LightColour = glm::vec4(1.0f)) : CLight(_v4LightColour)
	{
		m_v3Position = _v3Position;
	};

	const glm::vec3 GetPosition()
	{
		return m_v3Position;
	}
	const void SetPosition(glm::vec3 _v3Position)
	{
		m_v3Position = _v3Position;
		m_bUpdateProjectionMatrix = true;
	}
};

class CPointLight : public CLight
{
private:
	glm::vec3 m_v3Position;

protected:
	virtual void UpdateProjectionMatrix() override
	{

	}

public:
	float m_fAttenuationExponent;
	float m_fAttenuationLinear;
	float m_fAttenuationConstant;

	//Methods
	CPointLight
	(
		glm::vec3 _v3Position = {}, glm::vec4 _v4LightColour = glm::vec4(1.0f),
		float _fAttenuationExponent = 3.0f,
		float _fAttenuationLinear = 0.7f,
		float _fAttenuationConstant = 1.0f
	) : CLight(_v4LightColour)
	{
		m_v3Position = _v3Position;
		m_fAttenuationExponent = 3.0f;
		m_fAttenuationLinear = 0.7f;
		m_fAttenuationConstant = 1.0f;
	}

	const glm::vec3 GetPosition()
	{
		return m_v3Position;
	}
	const void SetPosition(glm::vec3 _v3Position)
	{
		m_v3Position = _v3Position;
		m_bUpdateProjectionMatrix = true;
	}
};

class CDirectionalLight : public CLight
{
private:
	glm::vec3 m_v3LightDirection;
	
protected:
	virtual void UpdateProjectionMatrix() override;

public:
	CDirectionalLight(glm::vec4 _v4LightColour = glm::vec4(1.0f), glm::vec3 _v3LightDirection = glm::vec3(-1.0f,-1.0f,1.0f))
		: CLight(_v4LightColour)
	{
		m_v3LightDirection = _v3LightDirection;
	}

	const glm::vec3 GetLightDirection()
	{
		return m_v3LightDirection;
	}
	void SetLightDirection(const glm::vec3 _v3LightDirection)
	{
		m_v3LightDirection = _v3LightDirection;
		m_bUpdateProjectionMatrix = true;
	}
};

class CSpotLight : public CLight
{
private:
	glm::vec3 m_v3Position;
	glm::vec3 m_v3LightDirection;

protected:
	virtual void UpdateProjectionMatrix() override
	{

	}

public:
	float m_fOuterCone;
	float m_fInnerCone;

	//Methods
	CSpotLight
	(
		glm::vec3 _v3Position = {},
		glm::vec4 _v4LightColour = glm::vec4(1.0f),
		glm::vec3 _v3LightDirection = glm::vec3(0.0f, -1.0f, 0.0f),
		float _fOuterCone = 0.9f, float _fInnerCone = 0.95f
	) : CLight(_v4LightColour)
	{
		m_v3Position = _v3Position;
		m_v3LightDirection = _v3LightDirection;
		m_fOuterCone = _fOuterCone;
		m_fInnerCone = _fInnerCone;
	}

	const glm::vec3 GetPosition()
	{
		return m_v3Position;
	}
	const void SetPosition(glm::vec3 _v3Position)
	{
		m_v3Position = _v3Position;
		m_bUpdateProjectionMatrix = true;
	}
	const glm::vec3 GetLightDirection()
	{
		return m_v3LightDirection;
	}
	void SetLightDirection(const glm::vec3 _v3LightDirection)
	{
		m_v3LightDirection = _v3LightDirection;
		m_bUpdateProjectionMatrix = true;
	}
};