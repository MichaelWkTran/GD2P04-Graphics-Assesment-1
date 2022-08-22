#include "GenerateMesh.h"
#include <glm/gtc/constants.hpp>

void gm::GenerateCube(CMesh<>& _Mesh, glm::vec3 _v3Scale/* = glm::vec3(1.0f,1.0f,1.0f)*/)
{
	std::vector<stVertex> vVertices =
	{
		//Coordinates
		stVertex{glm::vec3(1.0f,  -1.0f, -1.0f) * _v3Scale,},
		stVertex{glm::vec3(-1.0f, -1.0f, -1.0f) * _v3Scale,},
		stVertex{glm::vec3(1.0f,  -1.0f,  1.0f) * _v3Scale,},
		stVertex{glm::vec3(-1.0f, -1.0f,  1.0f) * _v3Scale,},
		stVertex{glm::vec3(1.0f,   1.0f, -1.0f) * _v3Scale,},
		stVertex{glm::vec3(-1.0f,  1.0f, -1.0f) * _v3Scale,},
		stVertex{glm::vec3(1.0f,   1.0f,  1.0f) * _v3Scale,},
		stVertex{glm::vec3(-1.0f,  1.0f,  1.0f) * _v3Scale,}
	};
	std::vector<unsigned int> vIndices =
	{
		0, 2, 1, //Top
		1, 2, 3,
		6, 4, 7, //Bottom
		7, 4, 5,
		2, 7, 3, //Front
		2, 6, 7,
		4, 1, 5, //Back
		4, 0, 1,
		7, 5, 3, //Right Side
		3, 5, 1,
		4, 6, 0, //Left Side
		0, 6, 2
	};

	//Set verticies and indicies of mesh
	_Mesh.SetVerticies(std::move(vVertices));
	_Mesh.SetIndicies(vIndices);
}

void gm::GenerateFlatCube(CMesh<>& _Mesh, glm::vec3 _v3Scale/* = glm::vec3(1.0f,1.0f,1.0f)*/)
{
	std::vector<stVertex> vVertices =
	{
		//Coordinates                                     Normals                      Texture Cordinate
		stVertex{glm::vec3(-0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f,1.0f)}, //Front
		stVertex{glm::vec3(-0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3( 0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,1.0f)},
											  
		stVertex{glm::vec3( 0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f,1.0f)}, //Back
		stVertex{glm::vec3( 0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3(-0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3(-0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f,-1.0f, 0.0f), glm::vec2(1.0f,1.0f)},
											  
		stVertex{glm::vec3( 0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f)}, //Right
		stVertex{glm::vec3( 0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3( 0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f,1.0f)},
											  
		stVertex{glm::vec3(-0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f,1.0f)}, //Left
		stVertex{glm::vec3(-0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3(-0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3(-0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f,1.0f)},
											  
		stVertex{glm::vec3(-0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f)}, //Front
		stVertex{glm::vec3(-0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f,1.0f)},
											  
		stVertex{glm::vec3(-0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f,1.0f)}, //Bottom
		stVertex{glm::vec3(-0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3( 0.5f,-0.5f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3( 0.5f,-0.5f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f,-1.0f), glm::vec2(1.0f,1.0f)}
	};
	std::vector<unsigned int> vIndices =
	{
		0,  1,  2,
		0,  2,  3,
		4,  5,  6,
		4,  6,  7,
		8,  9,  10,
		8,  10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};

	//Set verticies and indicies of mesh
	_Mesh.SetVerticies(vVertices);
	_Mesh.SetIndicies(vIndices);
}

void gm::GeneratePlane(CMesh<>& _Mesh, glm::vec3 _v3Scale)
{
	std::vector<stVertex> vVertices =
	{
		//Coordinates                                     Normals                      Texture Cordinate
		stVertex{glm::vec3(-0.5f, 0.0f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f,1.0f)},
		stVertex{glm::vec3(-0.5f, 0.0f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.0f, 0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f,0.0f)},
		stVertex{glm::vec3( 0.5f, 0.0f,-0.5f) * _v3Scale, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f,1.0f)},
	};
	std::vector<unsigned int> vIndices =
	{
		0,  1,  2,
		0,  2,  3,
	};

	_Mesh.SetVerticies(vVertices);
	_Mesh.SetIndicies(vIndices);
}

void gm::GenerateSphere(CMesh<>& _Mesh, const float _fRadius, const int _iFidelity)
{
	std::vector<stVertex> vVertices;
	std::vector<unsigned int> vIndices;

	//Angles to keep track of the sphere points 
	float fPhi = 0.0f, fTheta = 0.0f;

	//Each cycle moves down on the vertical (Y axis) to start the next ring

	//Create verticies
	for (int i = 0; i < _iFidelity; i++)
	{
		//A new  horizontal ring starts at 0 degrees
		fTheta = 0.0f;

		//Creates a horizontal ring and adds each new vertex point to the vertex array
		for (int j = 0; j < _iFidelity; j++)
		{
			//Calculate the new vertex position point with the new angles
			float x = cos(fPhi) * sin(fTheta);
			float y = cos(fTheta);
			float z = sin(fPhi) * sin(fTheta);

			//Set the position of the current vertex point
			vVertices.push_back
			(
				stVertex
				{
					//Coordinates                                                 
					glm::vec3(x * _fRadius,y * _fRadius,z * _fRadius),

					//Normals
					glm::vec3(x,y,z),

					//Texture Cordinate
					glm::vec2
					(
						(float)i / (_iFidelity - 1),
						1 - ((float)j / (_iFidelity - 1)) //1 minus in order to flip the direction of 0-1 (0 at the bottom)
					)
				}
			);

			//Theta (Y axis) angle is incremented based on the angle created by number of sections
			//As the sphere is built ring by ring, the theta is only needed to do half the circumferance therefore using just PI
			fTheta += (glm::pi<float>() / ((float)_iFidelity - 1.0f));
		}

		//Phi angle (X and Z axes) is incremented based on the angle created by the number of sections
		//Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		fPhi += (2.0f * glm::pi<float>()) / ((float)_iFidelity - 1.0f);
	}

	//Create indicies
	for (int i = 0; i < _iFidelity; i++)
	{
		for (int j = 0; j < _iFidelity; j++)
		{
			//First triangle of the quad
			vIndices.push_back((((i + 1) % _iFidelity) * _iFidelity) + ((j + 1) % _iFidelity));
			vIndices.push_back((i * _iFidelity) + (j));
			vIndices.push_back((((i + 1) % _iFidelity) * _iFidelity) + (j));

			//Second triangle of the quad
			vIndices.push_back((i * _iFidelity) + ((j + 1) % _iFidelity));
			vIndices.push_back((i * _iFidelity) + (j));
			vIndices.push_back((((i + 1) % _iFidelity) * _iFidelity) + ((j + 1) % _iFidelity));
		}
	}

	//Set verticies and indicies of mesh
	_Mesh.SetVerticies(vVertices);
	_Mesh.SetIndicies(vIndices);
}