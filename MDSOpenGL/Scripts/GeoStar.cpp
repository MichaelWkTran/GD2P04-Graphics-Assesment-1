#include "GeoStar.h"
#include "Shader.h"
#include "Texture.h"

CGeoStar::CGeoStar()
{
	//Assign the mesh with one vertex
	m_Mesh.SetVerticies(std::vector<stVertex>{ stVertex{} });
	
	//Set mesh shader and vertex
	m_Mesh.m_pShader = CShader::Find("Star");
	if (m_Mesh.m_pShader == nullptr ) m_Mesh.m_pShader = new CShader("Star", "Star.vert", "Star.geom", "Star.frag");

	CTexture* pTexture = CTexture::Find("Star");
	m_Mesh.m_mapTextures.emplace(std::make_pair("", (pTexture == nullptr) ? new CTexture("Star", "Eye.png", GL_RGB, GL_UNSIGNED_BYTE) : pTexture));

	m_Mesh.m_pTransform = &m_Transform;

	//Set mesh draw method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh) { glDrawArrays(GL_POINTS, 0, 1); };
}