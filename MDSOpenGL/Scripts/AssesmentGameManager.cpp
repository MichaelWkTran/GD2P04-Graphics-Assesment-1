/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.cpp																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

float fTime = 0;

#include "AssesmentGameManager.h"
#include "Camera.h" //For accessing the camera
#include "TinyObjectLoader.h" //For loading models
#include "LightManager.h" //For adding lights to the scene
#include "Shader.h" //For creating shaders
#include "Texture.h" //For creating textures
#include "CubeSkybox.h" //For creating a skybox
#include "Main.h" //For accessing viewport and keyboard input
#include <iostream> //For printing messages to console
#include "GeoStar.h"
#include "GeoSphere.h"

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CAssesmentGameManager()
//	 Purpose: To initalise variables and to setup the demo scene

CAssesmentGameManager::CAssesmentGameManager()
{
	//Setup frame and render buffer
	{
		//Create frame buffer
		glGenFramebuffers(1, &m_uiFrameBuffer);

		//Bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBuffer);

		//Create and attach texture to frame buffer
		CTexture* pRenderTexture = new CTexture("FrameBuffer");
		m_RenderQuad.m_mapTextures.emplace("renderTexture", pRenderTexture);
		pRenderTexture->Bind();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, e_uViewPortW, e_uViewPortH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pRenderTexture, 0);

		pRenderTexture->Unbind();

		//Create Render buffer
		glGenRenderbuffers(1, &m_uiRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_uiRenderBuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, e_uViewPortW, e_uViewPortH);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiRenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR: Framebuffer is is not complete";

		//Unbind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Setup render quad mesh 
		m_RenderQuad.SetVerticies({
		stVertex{glm::vec3(-1.0f, -1.0f, 0.0f), {}, glm::vec2(0.0f, 0.0f)},
		stVertex{glm::vec3( 1.0f, -1.0f, 0.0f), {}, glm::vec2(1.0f, 0.0f)},
		stVertex{glm::vec3(-1.0f,  1.0f, 0.0f), {}, glm::vec2(0.0f, 1.0f)},
		stVertex{glm::vec3( 1.0f,  1.0f, 0.0f), {}, glm::vec2(1.0f, 1.0f)}
		});

		m_RenderQuad.SetIndicies({0, 1, 2, 1, 3, 2});

		//Setup default frame buffer effect
		m_FrameBufferEffect = FrameBufferEffect::None;

		//Create Shaders
		new CShader("FrameBuffer",		   "FrameBuffer.vert", "FrameBuffer.frag");
		new CShader("Rain",				   "FrameBuffer.vert", "Rain.frag");
		new CShader("ChromaticAberration", "FrameBuffer.vert", "ChromaticAberration.frag");
		new CShader("CRT",				   "FrameBuffer.vert", "CRT.frag");

		//-------------------------------------------------------------------------------
		m_RenderQuad.m_mapTextures.emplace("noise", new CTexture("Noise", "Noise.png", GL_RGB, GL_UNSIGNED_BYTE));
	}

	//Create Shaders
	CShader* pDiffuse = new CShader("Diffuse", "Diffuse.vert", "Diffuse.frag");
	pDiffuse->m_pDefaultUniform = [](CShader& _Shader)
	{
		//Light Uniforms
		_Shader.Uniform1f("uni_fSpecularStrength", 0.3f);
		_Shader.Uniform1f("uni_fShininess", 16.0f);

		_Shader.Uniform1f("uni_fRimExponent", 16.0f);
		_Shader.Uniform1f("uni_fShininess", 16.0f);
		_Shader.Uniform4f("uni_v4RimColour", 1.0f, 1.0f, 1.0f, 0.0f);

		_Shader.Uniform1f("uni_fReflectionStrength", 0.0f);
	};

	//Setup Lighting
	new CDirectionalLight;
	CLight::UpdateShaderUniforms(pDiffuse);
	
	//Setup Camera
	GetMainCamera().SetFarPlane(4000.0f);
	GetMainCamera().m_Transform.Move(glm::vec3(0, 0, 5));

	//Draw Skybox
	{
		const char* pCubeMapDirectories[6]
		{
			"Cubemaps/MountainOutpost/Right.jpg",
			"Cubemaps/MountainOutpost/Left.jpg",
			"Cubemaps/MountainOutpost/Up.jpg",
			"Cubemaps/MountainOutpost/Down.jpg",
			"Cubemaps/MountainOutpost/Back.jpg",
			"Cubemaps/MountainOutpost/Front.jpg"
		};
	
		new CCubeSkybox(2000.0f, pCubeMapDirectories);
	}

	new CGeoSphere();
	new CGeoStar();
}

CAssesmentGameManager::~CAssesmentGameManager()
{
	glDeleteFramebuffers(1, &m_uiFrameBuffer);
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Update()
//	 Purpose: To perform operations every frame such as enabling or disabling scissor tests

void CAssesmentGameManager::Update()
{
	//Draw the scene to the frame buffer
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBuffer);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CGameManager::Update();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//Draw the renderTexture from the frame buffer to the screen
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Set the render buffer shader
	switch (m_FrameBufferEffect)
	{
	case FrameBufferEffect::None:
		m_RenderQuad.m_pShader = CShader::Find("FrameBuffer");
		break;
	case FrameBufferEffect::Rain:
		m_RenderQuad.m_pShader = CShader::Find("Rain");
		fTime += e_fDeltatime;
		m_RenderQuad.m_pShader->Uniform1f("iTime", fTime);
		break;
	case FrameBufferEffect::ChromaticAberration:
		m_RenderQuad.m_pShader = CShader::Find("ChromaticAberration");
		fTime += e_fDeltatime;
		m_RenderQuad.m_pShader->Uniform1f("iTime", fTime);
		break;
	case FrameBufferEffect::CRT:
		m_RenderQuad.m_pShader = CShader::Find("CRT");
		fTime += e_fDeltatime;
		m_RenderQuad.m_pShader->Uniform1f("iTime", fTime);
		break;
	}
	
	//Draw render quad
	m_RenderQuad.Draw(GetMainCamera());
}