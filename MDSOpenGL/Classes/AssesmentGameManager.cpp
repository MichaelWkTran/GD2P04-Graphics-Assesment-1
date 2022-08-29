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
#include "../Main.h" //For accessing viewport and keyboard input
#include "../GenerateTexture.h" //For generating a perlin texture
#include <time.h> //For generating a random seed
#include <fstream> //For writing height map data to raw files

int iHeightMapSeed = 0;
const glm::uvec2 v2uTerrainSize(513U, 513U);
unsigned char* pHeightImageData = nullptr;

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
				
				std::ofstream binaryFile("file.raw", std::ios::out | std::ios::trunc);
				binaryFile.write(reinterpret_cast<char*>(&pHeightImageData), v2uTerrainSize.x * v2uTerrainSize.y);
				binaryFile.close();
				break;
			}
		}
	);

	//Create Shaders
	CShader* pUnlit = new CShader("Unlit", "Unlit.vert", "Unlit.frag", "", [](CShader& _Shader) { _Shader.Uniform3f("uni_v3Colour", 1.0f, 1.0f, 1.0f); });
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
		
		_Shader.Uniform2f("uni_v2Tiling", 100.0f, 100.0f);
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

	//Draw RAW Terrain
	{
		CTerrain* pTerrain = new CTerrain("Resources/Height Maps/coastMountain513.raw", v2uTerrainSize.x, v2uTerrainSize.y);
		pTerrain->m_Mesh.m_pShader = pDiffuse;
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", new CTexture("Grass", "Grass.png", 0, GL_RGBA, GL_UNSIGNED_BYTE));
		pTerrain->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
		pTerrain->m_Transform.SetPosition(glm::vec3((float)v2uTerrainSize.x / 2.0f, 0.0f, 0.0f));
	}

	//Draw Perlin Terrain
	{
		pHeightImageData = new unsigned char[v2uTerrainSize.x * v2uTerrainSize.y];
		iHeightMapSeed = time(0);

		//Set Perlin Noise Image Data
		for (int x = 0; x < v2uTerrainSize.x; x++) for (int y = 0; y < v2uTerrainSize.y; y++)
		{
			pHeightImageData[x + (y * v2uTerrainSize.x)] = (unsigned char)(255.0f * gt::PerlinNoise(x, y, 4, 128.0f, 2.0f, iHeightMapSeed));
		}

		//Create Terrain
		CTerrain* pTerrain = new CTerrain(pHeightImageData, v2uTerrainSize.x, v2uTerrainSize.y, 0.03f);
		pTerrain->m_Mesh.m_pShader = pDiffuse;
		pTerrain->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", new CTexture("Sand", "Sand.png", 0, GL_RGBA, GL_UNSIGNED_BYTE));
		pTerrain->m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
		pTerrain->m_Transform.SetPosition(glm::vec3(-(float)v2uTerrainSize.x / 2.0f, 0.0f, 0.0f));

		//Free Image Data
		//delete[] pHeightImageData;
	}

	//Draw Superman
	//{
	//	COutlinedObject* pSuperman = new COutlinedObject();
	//	pSuperman->m_Mesh.m_pShader = pDiffuse;
	//	GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
	//	pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
	//	{
	//		glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
	//	};
	//	pSuperman->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", new CTexture("Grass", "Grass.png", 0, GL_RGBA, GL_UNSIGNED_BYTE));
	//
	//	pSuperman->m_v3OutlineColour = glm::vec3(1.0f, 0.0f, 0.0f);
	//	pSuperman->m_fOutLineThickness = 0.1f;
	//}

	//Draw Rings
	//{
	//	C3DTextureObject* pSuperman = new C3DTextureObject;
	//	GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
	//	pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
	//	{
	//		glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
	//	};
	//}
}

void CAssesmentGameManager::Update()
{
	if (m_bScissorTestEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 100, e_uViewPortW, e_uViewPortH - 200);
	}

	CGameManager::Update();

	glDisable(GL_SCISSOR_TEST);
}