#include "AssesmentGameManager.h"
#include "FreePlayerCamera.h"
#include "TinyObjectLoader.h"
#include "OutlinedObject.h"
#include "3DTextureObject.h"
#include "Terrain.h"
#include "LightManager.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "../Main.h"

CAssesmentGameManager::CAssesmentGameManager()
{
	m_bScissorTestEnabled = false;
	m_bStencilTestEnabled = false;
	e_setKeyCallbackFunctions.emplace
	(
		[](GLFWwindow* _pWindow, int _iKey, int _iScanCode, int _iAction, int _iMods)
		{
			CAssesmentGameManager* pGameManager = ((CAssesmentGameManager*)&GetGameManager());
			if (_iAction != GLFW_PRESS) return;

			switch (_iKey)
			{
			case GLFW_KEY_X:
				pGameManager->m_bScissorTestEnabled = !pGameManager->m_bScissorTestEnabled;
				break;
			case GLFW_KEY_Z:
				pGameManager->m_bStencilTestEnabled = !pGameManager->m_bStencilTestEnabled;
				break;
			}
		}
	);

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
	};

	CLightManager::m_vDirectionalLight.push_back(stDirectionalLight{ });

	CLightManager::UpdateShaderUniforms(pDiffuse);

	//Draw Superman
	{
		COutlinedObject* pSuperman = new COutlinedObject();
		pSuperman->m_Mesh.m_pShader = pDiffuse;
		GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
		pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
		{
			glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
		};
		pSuperman->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", new CTexture("Grass", "Resources/Textures/Grass.png", 0, GL_RGBA, GL_UNSIGNED_BYTE));

		pSuperman->m_v3OutlineColour = glm::vec3(1.0f, 0.0f, 0.0f);
		pSuperman->m_fOutLineThickness = 0.1f;
	}

	//Draw Terrain
	//{
	//	CTerrain* pSuperman = new CTerrain("Resources/Height Maps/coastMountain513.raw", 513U, 513U);
	//	pSuperman->m_Mesh.m_pShader = pDiffuse;
	//	pSuperman->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", new CTexture("Grass", "Resources/Textures/Grass.png", 1, GL_RGBA, GL_UNSIGNED_BYTE));
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