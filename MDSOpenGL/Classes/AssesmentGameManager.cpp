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

CAssesmentGameManager::CAssesmentGameManager()
{
	CShader* pUnlit = new CShader("Unlit", "Unlit.vert", "Unlit.frag", "", [](CShader& _Shader){ _Shader.Uniform3f("uni_v3Colour", 1.0f, 1.0f, 1.0f); });
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
	
	CLightManager::m_vDirectionalLight.push_back( stDirectionalLight { } );

	CLightManager::UpdateShaderUniforms(pDiffuse);

	//Draw Superman
	//{
	//	COutlinedObject* pSuperman = CreateObject<COutlinedObject>();
	//	pSuperman->m_Mesh.m_pShader = pDiffuse;
	//	GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
	//	pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
	//	{
	//		glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
	//	};
	//
	//	pSuperman->m_v3OutlineColour = glm::vec3(1.0f, 0.0f, 0.0f);
	//	pSuperman->m_fOutLineThickness = 0.1f;
	//}
	
	//Draw Rings
	{
		C3DTextureObject* pSuperman = CreateObject<C3DTextureObject>();

	{
		//Light Uniforms
		_Shader.Uniform1f("uni_fSpecularStrength", 0.3f);
		_Shader.Uniform1f("uni_fShininess", 16.0f);

		_Shader.Uniform1f("uni_fRimExponent", 16.0f);
		_Shader.Uniform1f("uni_fShininess", 16.0f);
		_Shader.Uniform4f("uni_v4RimColour", 1.0f, 1.0f, 1.0f, 0.0f);

		_Shader.Uniform1f("uni_fReflectionStrength", 0.0f);
	};
	
	CLightManager::UpdateDiffuseShader("Diffuse.frag");
	CLightManager::m_vDirectionalLight.push_back( stDirectionalLight { } );

	CLightManager::UpdateShaderUniforms(pDiffuse);

	//Draw Terrain
	{
		CTerrain* pSuperman = CreateObject<CTerrain>();
		pSuperman->Initalise("Resources/Height Maps/coastMountain513.raw", 513U, 513U);
		pSuperman->m_Mesh.m_pShader = pDiffuse;
		pSuperman->m_Mesh.m_mapTextures.emplace("uni_samp2DDiffuse0", CTextureManager::Insert("Grass", "Resources/Textures/Grass.png", 0, GL_RGBA, GL_UNSIGNED_BYTE));
	}
	
	//Draw Rings
	//{
	//	C3DTextureObject* pSuperman = CreateObject<C3DTextureObject>();
	//	GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
	//	pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
	//	{
	//		glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
	//	};
	//}
}