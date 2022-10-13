#include "GeoStar.h"
#include "Shader.h"
#include "Texture.h"

CTexture* CGeoStar::m_pTexture = nullptr;

CGeoStar::CGeoStar()
{
	//Assign the mesh with one vertex
	m_Mesh.SetVerticies(std::vector<stVertex>{ stVertex{} });
	
	//Set mesh shader and vertex
	m_Mesh.m_pShader = CShader::Find("Star");
	if (m_Mesh.m_pShader == nullptr ) m_Mesh.m_pShader = new CShader("Star", "Star.vert", "Star.geom", "Star.frag");
	
	if (m_pTexture == nullptr) m_pTexture = new CTexture("Star", "Eye.png", GL_RGB, GL_UNSIGNED_BYTE);
	m_Mesh.m_mapTextures.emplace(std::make_pair("", m_pTexture));

	m_Mesh.m_pTransform = &m_Transform;

	//Set mesh draw method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh) { glDrawArrays(GL_POINTS, 0, 1); };
}