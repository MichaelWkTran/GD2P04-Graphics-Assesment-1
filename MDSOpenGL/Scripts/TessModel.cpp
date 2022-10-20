#include "TessModel.h"
#include "Shader.h"
#include "GenerateMesh.h"
#include "Texture.h"
#include "Lights.h"

CTessModel::CTessModel()
{
	//Set Texture
	m_Mesh.m_mapTextures.emplace("uni_samp2DHeightMap", std::make_shared<CTexture>("Height Maps/coastMountain513.jpg"));

	//Set Mesh Verticies
	std::vector<stVertex> vVerticies;
	
	unsigned int uiSegmentWidthHeight = 100U;
	float fPlaneScale = 5.0f;
	for (unsigned int x = 0; x < uiSegmentWidthHeight; x++)
		for (unsigned int y = 0; y < uiSegmentWidthHeight; y++)
		{
			stVertex SegemntVerticies[4]
			{
				stVertex{fPlaneScale * glm::vec3(x-0.5f,0.0f,y-0.5f), {}, glm::vec2(x+0.0f, y+0.0f) / ((float)uiSegmentWidthHeight+1)},
				stVertex{fPlaneScale * glm::vec3(x+0.5f,0.0f,y-0.5f), {}, glm::vec2(x+1.0f, y+0.0f) / ((float)uiSegmentWidthHeight+1)},
				stVertex{fPlaneScale * glm::vec3(x-0.5f,0.0f,y+0.5f), {}, glm::vec2(x+0.0f, y+1.0f) / ((float)uiSegmentWidthHeight+1)},
				stVertex{fPlaneScale * glm::vec3(x+0.5f,0.0f,y+0.5f), {}, glm::vec2(x+1.0f, y+1.0f) / ((float)uiSegmentWidthHeight+1)}
			};

			vVerticies.push_back(SegemntVerticies[0]);
			vVerticies.push_back(SegemntVerticies[2]);
			vVerticies.push_back(SegemntVerticies[1]);
			vVerticies.push_back(SegemntVerticies[1]);
			vVerticies.push_back(SegemntVerticies[2]);
			vVerticies.push_back(SegemntVerticies[3]);
		}

	for (unsigned int i = 0; i < vVerticies.size(); i++)
	{
		vVerticies[i].v3Position -= (float)uiSegmentWidthHeight * fPlaneScale * 0.5f * glm::vec3(1.0f, 0.0f, 1.0f);
	}

	m_Mesh.SetVerticies(vVerticies);

	//Set Mesh Draw Method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh)
	{
		glDrawArrays(GL_PATCHES, 0, _Mesh.GetVerticies().size());
	};

	//Set Mesh Shader
	m_Mesh.m_pShader = std::make_shared<CShader>("Tessellation.vert", "Tessellation.tesc", "Tessellation.tese", "Tessellation.frag");

	//Set Mesh Attributes
	m_Mesh.m_VertexArray.Bind(); m_Mesh.m_VertexArray.Bind(); m_Mesh.m_ElementBuffer.Bind();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	stVertex::LinkAttributes();
	CVertexArray::Unbind(); CVertexBuffer<stVertex>::Unbind(); CElementBuffer::Unbind();
}

void CTessModel::Draw()
{
	CLight::UpdateLightUniforms(*m_Mesh.m_pShader.get());
	CGameObject::Draw();
}
