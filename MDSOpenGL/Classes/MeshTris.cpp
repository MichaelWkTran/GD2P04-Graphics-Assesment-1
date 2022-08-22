//#include "MeshTris.h"
//
//void CMeshTris::UpdateVertexArray()
//{
//	if (!m_bUpdateVertexArray) return void();
//	m_bUpdateVertexArray = false;
//
//	std::vector<stVertex> m_vVerticies;
//	std::vector<GLuint> m_vIndicies;
//
//	for (unsigned int uTriIndex = 0; uTriIndex < m_vTriangles.size(); uTriIndex++)
//	{
//		for (unsigned int uVertIndex = 0; uVertIndex < 3U; uVertIndex++)
//		{
//			stTriangle& Triangle = m_vTriangles[uTriIndex];
//			stVertex Vertex
//			{
//				m_vVertexPositions[Triangle.m_Indicies[uVertIndex]],
//				Triangle.m_v3Normals[uVertIndex],
//				Triangle.m_v3TextureUV[uVertIndex],
//				Triangle.m_v3Colours[uVertIndex]
//			};
//
//			m_vVerticies.push_back(Vertex);
//			m_vIndicies.push_back(m_vVerticies.size() - 1U);
//		}
//	}
//
//	m_VertexBuffer.SetVertices(m_vVerticies);
//	m_ElementBuffer.SetIndicies(m_vIndicies);
//
//	m_VertexArray.Bind(); m_VertexBuffer.Bind(); m_ElementBuffer.Bind();
//
//	m_VertexArray.LinkAttribute(0 /*m_v3CurrentPosition*/, 3, GL_FLOAT, sizeof(stVertex), (void*)0);
//	m_VertexArray.LinkAttribute(1 /*m_v3Normal*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(3 * sizeof(float)));
//	m_VertexArray.LinkAttribute(2 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(6 * sizeof(float)));
//	m_VertexArray.LinkAttribute(3 /*m_v3Color*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(8 * sizeof(float)));
//
//	m_VertexArray.Unbind(); m_VertexBuffer.Unbind(); m_ElementBuffer.Unbind();
//}
//
//CMeshTris::CMeshTris(std::vector<glm::vec3>& _vVertexPositions, std::vector<stTriangle>& _vTriangles, std::vector <CTexture*>& _vTextures, CShader& _Shader)
//{
//	m_vVertexPositions = _vVertexPositions;
//	m_vTriangles = _vTriangles;
//	m_vTextures = _vTextures;
//	m_pShader = &_Shader;
//
//	m_bUpdateVertexArray = true;
//}