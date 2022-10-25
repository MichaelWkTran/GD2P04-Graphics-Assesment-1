#include "GeoStar.h"
#include "Shader.h"
#include "Texture.h"
#include "Lights.h"

CGeoStar::CGeoStar()
{
	//Assign the mesh with one vertex
	m_mesh.SetVerticies(std::vector<stVertex>{ stVertex{} });
	
	//Set shaders
	m_mesh.m_shader = new CShader("Star.vert", "Star.geom", "Star.frag");
	m_mesh.m_shadowShader = new CShader("StarShadowMap.vert", "StarShadowMap.geom", "Empty.frag");

	//Setup texture
	m_mesh.m_textures.emplace(std::make_pair("", new CTexture("Eye.png")));

	//Set mesh draw method
	m_mesh.m_drawMethod = [](CMesh<>& _Mesh) { glDrawArrays(GL_POINTS, 0, 1); };
}

void CGeoStar::Draw()
{
	CLight::UpdateLightUniforms(*m_mesh.m_shader);
	CGameObject::Draw();
}
