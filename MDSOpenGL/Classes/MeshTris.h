//#pragma once
//#include "Mesh.h"
//
//struct stTriangle
//{
//	GLuint m_Indicies[3];
//	glm::vec3 m_v3Normals[3];
//	glm::vec2 m_v3TextureUV[3];
//	glm::vec3 m_v3Colours[3] = { glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1) };
//};
//
//class CMeshTris : public CMesh
//{
//protected:
//	std::vector<glm::vec3> m_vVertexPositions;
//	std::vector<stTriangle> m_vTriangles;
//
//	virtual void UpdateVertexArray();
//
//public:
//	CMeshTris(std::vector<glm::vec3>& _vVertexPositions, std::vector<stTriangle>& _vTriangles, std::vector <CTexture*>& _vTextures, CShader& _Shader);
//};