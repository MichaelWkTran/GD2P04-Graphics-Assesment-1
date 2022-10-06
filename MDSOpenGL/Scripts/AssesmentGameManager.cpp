/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.cpp																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

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
	CLightManager::m_vDirectionalLight.push_back(stDirectionalLight{ });
	CLightManager::UpdateShaderUniforms(pDiffuse);
	
	//Setup Camera
	GetMainCamera().SetFarPlane(4000.0f);

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
	//new CGeoStar();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Update()
//	 Purpose: To perform operations every frame such as enabling or disabling scissor tests

void CAssesmentGameManager::Update()
{
	//Update Game Manager
	CGameManager::Update();
}