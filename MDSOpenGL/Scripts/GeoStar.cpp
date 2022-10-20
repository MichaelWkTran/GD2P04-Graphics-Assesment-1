#include "GeoStar.h"
#include "Shader.h"
#include "Texture.h"

CShader* CGeoStar::m_pShader = nullptr;
CTexture* CGeoStar::m_pTexture = nullptr;

CGeoStar::CGeoStar()
{
	//Assign the mesh with one vertex
	m_Mesh.SetVerticies(std::vector<stVertex>{ stVertex{} });
	
	//Set mesh shader and vertex
	if (m_pShader == nullptr) m_pShader = new CShader("Star.vert", "Star.geom", "Star.frag");
	m_Mesh.m_pShader = std::shared_ptr<CShader>(m_pShader);
	
	if (m_pTexture == nullptr) m_pTexture = new CTexture("Eye.png");
	m_Mesh.m_mapTextures.emplace(std::make_pair("", m_pTexture));

	m_Mesh.m_pTransform = &m_Transform;

	//Set mesh draw method
	m_Mesh.m_pDrawMethod = [](CMesh<>& _Mesh) { glDrawArrays(GL_POINTS, 0, 1); };
}