#include "GeoStar.h"
#include "Shader.h"
#include "Texture.h"
#include "Lights.h"

CGeoStar::CGeoStar()
{
	//Assign the mesh with one vertex
	m_Mesh.SetVerticies(std::vector<stVertex>{ stVertex{} });
	
	//Set shaders
	m_Mesh.m_pShader = std::make_shared<CShader>("Star.vert", "Star.geom", "Star.frag");
	m_Mesh.m_pShadowShader = std::make_shared<CShader>("StarShadowMap.vert", "StarShadowMap.geom", "Empty.frag");

	//Setup texture
	m_Mesh.m_mapTextures.emplace(std::make_pair("", std::make_shared<CTexture>("Eye.png")));

	//Set mesh draw method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh) { glDrawArrays(GL_POINTS, 0, 1); };
}

void CGeoStar::Draw()
{
	CLight::UpdateLightUniforms(*m_Mesh.m_pShader.get());
	CGameObject::Draw();
}
