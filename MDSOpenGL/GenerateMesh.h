#pragma once
#include "Classes/Mesh.h"

namespace gm
{
	void GenerateCube(CMesh<>& _Mesh, glm::vec3 _v3Scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void GenerateFlatCube(CMesh<>& _Mesh, glm::vec3 _v3Scale = glm::vec3(1.0f,1.0f,1.0f));
	void GeneratePlane(CMesh<>& _Mesh, glm::vec3 _v3Scale = glm::vec3(1.0f, 1.0f, 1.0f));
	void GenerateSphere(CMesh<>& _Mesh, const float _fRadius, const int _iFidelity);
}