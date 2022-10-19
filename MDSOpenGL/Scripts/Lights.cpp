/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: LightManager.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Lights.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Main.h"
#include "GameManager.h"
#include "GameObject.h"
#include <queue>

std::shared_ptr<CShader> CLight::m_pShadowMapShader = nullptr;
std::set<CLight*> CLight::m_setLightsInWorld;
glm::vec4 CLight::m_v4AmbientColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f);

void CLight::UpdateLightUniforms(CShader& _Shader)
{
	unsigned int uiInfinitePointLightIndex = 0U;
	unsigned int uiPointLightIndex		   = 0U;
	unsigned int uiDirectionalLightIndex   = 0U;
	unsigned int uiSpotLightIndex		   = 0U;

	//Set light type uniforms
	for (auto& pLight : m_setLightsInWorld)
	{
		std::string strUniformObject = "";

		if (auto pObject = dynamic_cast<CInfinitePointLight*>(pLight))
		{
			strUniformObject = std::string("uni_InfinitePointLight") + '['+std::to_string(uiInfinitePointLightIndex)+']';

			//Set the light uniforms
			_Shader.Uniform3f(strUniformObject + ".v3LightPosition", pObject->GetPosition());
			
			//Update light index
			uiInfinitePointLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CPointLight*>(pLight))
		{
			strUniformObject = std::string("uni_PointLight") + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_Shader.Uniform3f(strUniformObject + ".v3LightPosition", pObject->GetPosition());
			_Shader.Uniform1f(strUniformObject + ".fAttenuationExponent", pObject->m_fAttenuationExponent);
			_Shader.Uniform1f(strUniformObject + ".fAttenuationLinear", pObject->m_fAttenuationLinear);
			_Shader.Uniform1f(strUniformObject + ".fAttenuationConstant", pObject->m_fAttenuationConstant);

			//Update light index
			uiPointLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CDirectionalLight*>(pLight))
		{
			strUniformObject = std::string("uni_DirectionalLight")  + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_Shader.Uniform3f(strUniformObject + ".v3LightDirection", pObject->GetLightDirection());

			//Update light index
			uiDirectionalLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CSpotLight*>(pLight))
		{
			strUniformObject = std::string("uni_SpotLight") + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_Shader.Uniform3f(strUniformObject + ".v3LightPosition", pObject->GetPosition());
			_Shader.Uniform3f(strUniformObject + ".v3LightDirection", pObject->GetLightDirection());
			_Shader.Uniform1f(strUniformObject + ".fOuterCone", pObject->m_fOuterCone);
			_Shader.Uniform1f(strUniformObject + ".fInnerCone", pObject->m_fInnerCone);

			//Update light index
			uiSpotLightIndex++;
		}
		
		//Set CLight uniforms
		_Shader.Uniform4f(strUniformObject + ".v4LightColour", pLight->m_v4LightColour);
		_Shader.UniformMatrix4fv(strUniformObject + ".mat4VPMatrix", 1, GL_FALSE, pLight->GetProjection());
		pLight->m_pShadowMapTexture->Uniform(_Shader, strUniformObject + ".samp2DShadowMap", 10); //[Change Unit Later]
	}

	//Set the uniforms for how may lights are in the world for each type
	_Shader.Uniform1i("uni_iInfinitePointLightNum", uiInfinitePointLightIndex);
	_Shader.Uniform1i("uni_iPointLightNum", uiPointLightIndex);
	_Shader.Uniform1i("uni_iDirectionalLightNum", uiDirectionalLightIndex);
	_Shader.Uniform1i("uni_iSpotLightNum", uiSpotLightIndex);

	//Set uni_v4AmbientColour
	_Shader.Uniform4f("uni_v4AmbientColour", m_v4AmbientColour);
}

void CLight::UpdateShadowUniforms()
{
	if (m_setLightsInWorld.size() <= 0U) return;

	//Update the projection matricies for all the lights
	for (auto& pLight : m_setLightsInWorld)
		pLight->UpdateProjectionMatrix();
	
	//Set the shader of the meshes creating shadows
	std::queue<std::shared_ptr<CShader>> queueOriginalShaders;
	for (auto& i : GetGameManager().GetUpdatedObjects())
	{
		//Check whether the object is a game object
		CGameObject* pGameObject = dynamic_cast<CGameObject*>(i);
		if (pGameObject == nullptr) continue;

		//Store original shader and set the mesh shader as the shadow map shader
		queueOriginalShaders.emplace(pGameObject->m_Mesh.m_pShader);
		pGameObject->m_Mesh.m_pShader = m_pShadowMapShader;
	}

	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Draw the light framebuffers
	for (auto& pLight : m_setLightsInWorld)
	{
		//Set light viewport and buffer
		glViewport(0, 0, e_uViewPortW, e_uViewPortH);
		glBindFramebuffer(GL_FRAMEBUFFER, pLight->GetFrameBuffer());
		glClear(GL_DEPTH_BUFFER_BIT);

		//Set shadow map shader projection uniform
		m_pShadowMapShader->UniformMatrix4fv("uni_mat4LightProjection", 1, false, pLight->GetProjection());

		//Draw scene
		for (auto& i : GetGameManager().GetUpdatedObjects())
		{
			//Check whether the object is a game object
			CGameObject* pGameObject = dynamic_cast<CGameObject*>(i);
			if (pGameObject == nullptr) continue;

			//Draw the game object onto the frame buffer
			pGameObject->Draw();
		}
	}

	//Unbind frame buffer and return viewport to original properties
	CTexture::Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0U);
	glViewport(0, 0, e_uViewPortW, e_uViewPortH);

	//Restore the original shader to the mesh
	for (auto& i : GetGameManager().GetUpdatedObjects())
	{
		//Check whether the object is a game object
		CGameObject* pGameObject = dynamic_cast<CGameObject*>(i);
		if (pGameObject == nullptr) continue;

		//Store original shader and set the mesh shader as the shadow map shader
		pGameObject->m_Mesh.m_pShader = queueOriginalShaders.front();
		queueOriginalShaders.pop();
	}
}

CLight::CLight(glm::vec4 _v4LightColour)
{
	m_setLightsInWorld.emplace(this);
	if (m_pShadowMapShader == nullptr) m_pShadowMapShader = std::make_shared<CShader>("ShadowMap.vert", "Empty.frag");

	//Initialise Light
	m_v4LightColour = _v4LightColour;
	m_mat4Projection = glm::mat4(1.0f);
	m_bUpdateProjectionMatrix = true;

	//Depth Texture
	m_pShadowMapTexture = new CTexture();
	m_pShadowMapTexture->Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, e_uViewPortW, e_uViewPortH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float fClampColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fClampColour);

	//Depth Frame Buffer
	glGenFramebuffers(1, &m_uiFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *m_pShadowMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pShadowMapTexture->Unbind();

	//Check Frame Buffer Validation
	GLenum uiStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (uiStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", uiStatus);
	}
}

CLight::~CLight()
{
	delete m_pShadowMapTexture;
	glDeleteFramebuffers(1U, &m_uiFrameBuffer);
}

void CDirectionalLight::UpdateProjectionMatrix()
{
	//Check whether the projection matrix needed to be updated and update it
	if (m_bUpdateProjectionMatrix == false) return;

	glm::mat4 mat4Projection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 mat4View = glm::lookAt(20.0f * -m_v3LightDirection, {}, glm::vec3(0.0f, 1.0f, 0.0f));
	m_mat4Projection = mat4Projection * mat4View;
}
