/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: AssesmentGameManager.cpp																						*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "AssesmentGameManager.h"
#include "Camera.h" //For accessing the camera
#include "TinyObjectLoader.h" //For loading models
#include "OutlinedObject.h" //For creating objects using a stencil outline
#include "3DTextureObject.h" //For creating objects using a 3d Texture
#include "Terrain.h" //For creating terrain
#include "LightManager.h" //For adding lights to the scene
#include "Shader.h" //FOr creating shaders
#include "Texture.h" //For creating textures
#include "CubeSkybox.h" //For creating a skybox
#include "Main.h" //For accessing viewport and keyboard input
#include "GenerateTexture.h" //For generating a perlin texture
#include <time.h> //For generating a random seed
#include <fstream> //For writing height map data to raw files
#include <iostream> //For printing messages to console
#include <stb/stb_image_write.h>

int iHeightMapSeed = 0;
const glm::uvec2 v2uTerrainSize(513U, 513U);
float *pHeightImageData = nullptr;

//------------------------------------------------------------------------------------------------------------------------
// Procedure: CAssesmentGameManager()
//	 Purpose: To initalise variables and to setup the demo scene

CAssesmentGameManager::CAssesmentGameManager()
{
	m_bScissorTestEnabled = false;
	m_bStencilTestEnabled = false;
	
	//Set Keyboard Input
	e_setKeyCallbackFunctions.emplace
	(
		[](GLFWwindow* _pWindow, int _iKey, int _iScanCode, int _iAction, int _iMods)
		{
			CAssesmentGameManager* pGameManager = ((CAssesmentGameManager*)&GetGameManager());
			if (_iAction != GLFW_PRESS) return;

			switch (_iKey)
			{
			//Enable/Disable Scissor Test
			case GLFW_KEY_X:
				pGameManager->m_bScissorTestEnabled = !pGameManager->m_bScissorTestEnabled;
				break;
				//Enable/Disable Stencil Test
			case GLFW_KEY_Z:
				pGameManager->m_bStencilTestEnabled = !pGameManager->m_bStencilTestEnabled;
				break;
				//Save Height and Cloud Map
			case GLFW_KEY_C:
				//Save the Height Map as a raw file
				if (pHeightImageData == nullptr) break;
				
				stbi_write_jpg("Resources/Height Maps/GeneratedHeightMap.jpg", v2uTerrainSize.x, v2uTerrainSize.y, 1, (char*)pHeightImageData, 100);
				std::cout << "\nTerrain Height Map is Saved\n";

				break;
			}
		}
	);

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

	//Setup Terrain Shader and textures
	new CShader
	(
		"Terrain", "Terrain.vert", "Terrain.frag", "", 
		[](CShader& _Shader)
		{
			_Shader.Uniform2f("uni_v2Tiling", 200.0f, 200.0f);

			_Shader.Uniform1f("uni_fSpecularStrength", 0.3f);
			_Shader.Uniform1f("uni_fShininess", 16.0f);

			_Shader.Uniform1f("uni_fRimExponent", 16.0f);
			_Shader.Uniform1f("uni_fShininess", 16.0f);
			_Shader.Uniform4f("uni_v4RimColour", 1.0f, 1.0f, 1.0f, 0.0f);
		}
	);
	CLightManager::UpdateShaderUniforms(CShader::Find("Terrain"));

	new CTexture("Snow", "Snow.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	new CTexture("Stone", "Stone.png", 1, GL_RGBA, GL_UNSIGNED_BYTE);
	new CTexture("Grass", "Grass.png", 2, GL_RGBA, GL_UNSIGNED_BYTE);
	new CTexture("Sand", "Sand.png", 3, GL_RGBA, GL_UNSIGNED_BYTE);

	//Create RAW Terrain
	{
		CTerrain* pTerrain = new CTerrain("Resources/Height Maps/coastMountain513.jpg");
		pTerrain->m_Mesh.m_pShader = CShader::Find("Terrain");
		pTerrain->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
		pTerrain->m_Transform.SetPosition(glm::vec3(3.0f * (float)v2uTerrainSize.x / 2.0f, 0.0f, 0.0f));
	
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSnow",  CTexture::Find("Snow"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseStone", CTexture::Find("Stone"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseGrass", CTexture::Find("Grass"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSand",  CTexture::Find("Sand"));
	}

	//Create Perlin Terrain
	{
		pHeightImageData = new float[v2uTerrainSize.x * v2uTerrainSize.y];
		srand((int)time(0));
		iHeightMapSeed = rand();

		//Set Perlin Noise Image Data
		for (int x = 0; x < (int)v2uTerrainSize.x; x++)
			for (int y = 0; y < (int)v2uTerrainSize.y; y++)
			{
				pHeightImageData[x + (y * v2uTerrainSize.x)] = 225.0f * gt::PerlinNoise((float)x, (float)y, 6, 64.0f, 0.5f, iHeightMapSeed);
			}

		//Create Terrain
		CTerrain* pTerrain = new CTerrain(pHeightImageData, v2uTerrainSize.x, v2uTerrainSize.y, 0.03f);
		pTerrain->m_Mesh.m_pShader = CShader::Find("Terrain");
		pTerrain->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
		pTerrain->m_Transform.SetPosition(glm::vec3(-(float)v2uTerrainSize.x / 2.0f, 0.0f, 0.0f));

		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSnow", CTexture::Find("Snow"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseStone", CTexture::Find("Stone"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseGrass", CTexture::Find("Grass"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSand", CTexture::Find("Sand"));
	}

	//Create Saved Terrain
	{
		//Create Terrain
		CTerrain* pTerrain = new CTerrain("Resources/Height Maps/GeneratedHeightMap.jpg", 0.03f);
		pTerrain->m_Mesh.m_pShader = CShader::Find("Terrain");
		pTerrain->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
		pTerrain->m_Transform.SetPosition(glm::vec3((float)v2uTerrainSize.x / 2.0f, 0.0f, 0.0f));

		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSnow",  CTexture::Find("Snow"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseStone", CTexture::Find("Stone"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseGrass", CTexture::Find("Grass"));
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuseSand",  CTexture::Find("Sand"));
	}

	//Draw Outlined Object
	{
		COutlinedObject* pOutlinedObject = new COutlinedObject();
		pOutlinedObject->m_Mesh.m_pShader = pDiffuse;
		GetObjModelData(pOutlinedObject->m_Mesh, "Resources/Models/OutlineMesh.obj");
		pOutlinedObject->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
		{
			glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
		};
		
		pOutlinedObject->m_v3OutlineColour = glm::vec3(1.0f, 0.0f, 0.0f);
		pOutlinedObject->m_fOutLineThickness = 0.1f;

		pOutlinedObject->m_Transform.SetPosition(glm::vec3(5.0f, 5.0f, 0.0f));
		pOutlinedObject->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.2f);
	}

	//Draw 3D Texture Object
	{
		C3DTextureObject* p3DTextureObject = new C3DTextureObject;
		GetObjModelData(p3DTextureObject->m_Mesh, "Resources/Models/Suzanne.obj");
		p3DTextureObject->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
		{
			glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
		};

		p3DTextureObject->m_Transform.SetPosition(glm::vec3(-2.5f, 3.0f, 0.0f));
		p3DTextureObject->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.35f);
	}
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Update()
//	 Purpose: To perform operations every frame such as enabling or disabling scissor tests

void CAssesmentGameManager::Update()
{
	//Enable Scissor Test
	if (m_bScissorTestEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 100, e_uViewPortW, e_uViewPortH - 200);
	}

	//Update Game Manager
	CGameManager::Update();

	//Return to default
	glDisable(GL_SCISSOR_TEST);
}