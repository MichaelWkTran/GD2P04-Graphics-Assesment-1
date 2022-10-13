/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: LightManager.cpp																								*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "LightManager.h"
#include <fstream>
#include "Shader.h"
#include "Texture.h"
#include "Main.h"

std::vector<CLight*> CLight::m_vLightsInWorld;
glm::vec4 CLight::m_v4AmbientColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f);

CLight::CLight(glm::vec4 _v4LightColour)
{
	m_vLightsInWorld.emplace_back(this);

	//Initialise Light
	m_uiFrameBuffer = 0U;
	m_v4LightColour = _v4LightColour;
	m_mat4Projection = glm::mat4(1.0f);
	m_bUpdateProjectionMatrix = false;

	//Depth Texture
	CTexture* m_pDepthMap = new CTexture("DepthMap");
	m_pDepthMap->Bind();
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, e_uViewPortW, e_uViewPortH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	//Create Depth Frame Buffer
	unsigned int m_uiFrameBuffer;
	glGenFramebuffers(1, &m_uiFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBuffer);

	//Attach Depth Texture as FBO's Depth Buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *m_pDepthMap, 0);

	//Disable Writes to Color Buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Unbind Buffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Check Frame Buffer Validation
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", Status);
	}
}

CLight::~CLight()
{

}

void CLight::UpdateShaderUniforms(CShader* _pShader)
{
	unsigned int uiInfinitePointLightIndex = 0U;
	unsigned int uiPointLightIndex		   = 0U;
	unsigned int uiDirectionalLightIndex   = 0U;
	unsigned int uiSpotLightIndex		   = 0U;

	//Set light type uniforms
	for (auto& pLight : m_vLightsInWorld)
	{
		if (auto pObject = dynamic_cast<CInfinitePointLight*>(pLight))
		{
			std::string pUniformObject = std::string("uni_InfinitePointLight") + '['+std::to_string(uiInfinitePointLightIndex)+']';

			//Set the light uniforms
			_pShader->Uniform3f(pUniformObject + ".v3LightPosition", pObject->GetPosition());
			_pShader->Uniform4f(pUniformObject + ".v4LightColour", pObject->m_v4LightColour);
			_pShader->UniformMatrix4fv(pUniformObject + ".mat4VPMatrix", 1, GL_FALSE, pObject->GetProjection());

			//Update light index
			uiInfinitePointLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CPointLight*>(pLight))
		{
			std::string pUniformObject = std::string("uni_PointLight") + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_pShader->Uniform3f(pUniformObject + ".v3LightPosition", pObject->GetPosition());
			_pShader->Uniform4f(pUniformObject + ".v4LightColour", pObject->m_v4LightColour);
			_pShader->Uniform1f(pUniformObject + ".fAttenuationExponent", pObject->m_fAttenuationExponent);
			_pShader->Uniform1f(pUniformObject + ".fAttenuationLinear", pObject->m_fAttenuationLinear);
			_pShader->Uniform1f(pUniformObject + ".fAttenuationConstant", pObject->m_fAttenuationConstant);
			_pShader->UniformMatrix4fv(pUniformObject + ".mat4VPMatrix", 1, GL_FALSE, pObject->GetProjection());

			//Update light index
			uiPointLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CDirectionalLight*>(pLight))
		{
			std::string pUniformObject = std::string("uni_DirectionalLight")  + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_pShader->Uniform3f(pUniformObject + ".v3LightDirection", pObject->GetLightDirection());
			_pShader->Uniform4f(pUniformObject + ".v4LightColour", pObject->m_v4LightColour);
			_pShader->UniformMatrix4fv(pUniformObject + ".mat4VPMatrix", 1, GL_FALSE, pObject->GetProjection());

			//Update light index
			uiDirectionalLightIndex++;
		}
		else if (auto pObject = dynamic_cast<CSpotLight*>(pLight))
		{
			std::string pUniformObject = std::string("uni_SpotLight") + '[' + std::to_string(uiInfinitePointLightIndex) + ']';

			//Set the light uniforms
			_pShader->Uniform3f(pUniformObject + ".v3LightPosition", pObject->GetPosition());
			_pShader->Uniform3f(pUniformObject + ".v3LightDirection", pObject->GetLightDirection());
			_pShader->Uniform4f(pUniformObject + ".v4LightColour", pObject->m_v4LightColour);
			_pShader->Uniform1f(pUniformObject + ".fOuterCone", pObject->m_fOuterCone);
			_pShader->Uniform1f(pUniformObject + ".fInnerCone", pObject->m_fInnerCone);
			_pShader->UniformMatrix4fv(pUniformObject + ".mat4VPMatrix", 1, GL_FALSE, pObject->GetProjection());

			//Update light index
			uiSpotLightIndex++;
		}
	}

	//Set the uniforms for how may lights are in the world for each type
	_pShader->Uniform1i("uni_iInfinitePointLightNum", uiInfinitePointLightIndex);
	_pShader->Uniform1i("uni_iPointLightNum", uiPointLightIndex);
	_pShader->Uniform1i("uni_iDirectionalLightNum", uiDirectionalLightIndex);
	_pShader->Uniform1i("uni_iSpotLightNum", uiSpotLightIndex);

	//Set uni_v4AmbientColour
	_pShader->Uniform4f("uni_v4AmbientColour", m_v4AmbientColour);
}