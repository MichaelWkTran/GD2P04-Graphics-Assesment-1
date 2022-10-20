#include "TessModel.h"
#include "Shader.h"
#include "GenerateMesh.h"
#include "Texture.h"

CTessModel::CTessModel()
{
	//Create and Assign Texture
	CTexture* pTextures = new CTexture();

	//Load 2D Image
	int iImageWidth, iImageHeight, iChannelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pImageData = stbi_load((std::string(CTexture::m_pDirective) + "Height Maps/coastMountain513.jpg").c_str(),
		&iImageWidth, &iImageHeight, &iChannelNum, 0);

	pTextures->Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeight, 0, GL_R, GL_UNSIGNED_BYTE, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pImageData);

	pTextures->Unbind();
	
	m_Mesh.m_mapTextures.emplace("uni_samp2DHeightMap", pTextures);

	//Set Mesh Verticies
	std::vector<stVertex> vVerticies;
	
	unsigned int uiSegmentWidthHeight = 100U;
	float fPlaneScale = 5.0f;
	for (unsigned int x = 0; x < uiSegmentWidthHeight; x++)
		for (unsigned int y = 0; y < uiSegmentWidthHeight; y++)
		{
			stVertex SegemntVerticies[4]
			{
				stVertex{fPlaneScale * glm::vec3((float)x - 0.5f, 0.0f, (float)y - 0.5f), {}, glm::vec2((float)x, (float)y) + (glm::vec2(0.0f, 0.0f) / (float)uiSegmentWidthHeight)},
				stVertex{fPlaneScale * glm::vec3((float)x + 0.5f, 0.0f, (float)y - 0.5f), {}, glm::vec2((float)x, (float)y) + (glm::vec2(0.0f, 1.0f) / (float)uiSegmentWidthHeight)},
				stVertex{fPlaneScale * glm::vec3((float)x - 0.5f, 0.0f, (float)y + 0.5f), {}, glm::vec2((float)x, (float)y) + (glm::vec2(1.0f, 0.0f) / (float)uiSegmentWidthHeight)},
				stVertex{fPlaneScale * glm::vec3((float)x + 0.5f, 0.0f, (float)y + 0.5f), {}, glm::vec2((float)x, (float)y) + (glm::vec2(1.0f, 1.0f) / (float)uiSegmentWidthHeight)}
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
	m_Mesh.BindVertexArray(); m_Mesh.BindVertexArray(); m_Mesh.BindElementBuffer();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	stVertex::LinkAttributes();
	CVertexArray::Unbind(); CVertexBuffer<stVertex>::Unbind(); CElementBuffer::Unbind();
}

void CTessModel::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	CGameObject::Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
