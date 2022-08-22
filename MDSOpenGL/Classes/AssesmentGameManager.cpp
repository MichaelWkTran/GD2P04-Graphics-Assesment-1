#include "AssesmentGameManager.h"
#include "FreePlayerCamera.h"
#include "TinyObjectLoader.h"
#include "OutlinedObject.h"
#include "GameObject.h"
#include "Shader.h"

CAssesmentGameManager::CAssesmentGameManager()
{
	CShader* pUnlit = CShaderManager::Insert("Unlit", "Resources/Shaders/Unlit.vert", "Resources/Shaders/Unlit.frag");
	
	//Draw Superman
	{
		COutlinedObject* pSuperman = CreateObject<COutlinedObject>();
		pSuperman->m_Mesh.m_pShader = pUnlit;
		GetObjModelData(pSuperman->m_Mesh, "Resources/Models/Suzanne.obj");
		pSuperman->m_Mesh.m_pDrawMethod = [](CMesh<stVertex>& _Mesh)
		{
			glDrawArrays(GL_TRIANGLES, _Mesh.GetIndicies().size(), GL_UNSIGNED_INT);
		};

		pSuperman->m_v3OutlineColour = glm::vec3(1.0f, 0.0f, 0.0f);
		pSuperman->m_fOutLineThickness = 0.1f;
	}
	
	//Draw Rings


}