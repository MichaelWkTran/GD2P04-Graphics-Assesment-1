#include "Terrain.h"
#include <fstream>

CTerrain::CTerrain(const char* _strHeightmapDirectory, unsigned int _uiNumRows, unsigned int _uiNumCols, float _fHeightScaleFactor)
{
	m_Transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
	unsigned int uiVertexCount = _uiNumRows * _uiNumCols;
	std::vector<float> vHeightmap(uiVertexCount);

	//Load heightmap data from directory
	{
		std::vector<unsigned char> vFileData(uiVertexCount);

		//Read From File
		std::ifstream InFile;
		InFile.open(_strHeightmapDirectory, std::ios_base::binary);
		if (InFile)
		{
			// Read the RAW bytes.
			InFile.read((char*)&vFileData[0], (std::streamsize)vFileData.size());

			// Done with file.
			InFile.close();
		}

		//Assign to vHeightmap
		for (unsigned int i = 0; i < uiVertexCount; i++)
		{
			vHeightmap[i] = (float)vFileData[i] * _fHeightScaleFactor;
		}
	}

	//Smooth the Heightmap
	{
		std::vector<float> vDest(vHeightmap.size());

		for (unsigned int i = 0; i < _uiNumRows; i++)
			for (unsigned int j = 0; j < _uiNumCols; j++)
			{
				//Calculate the new vertex height by averaging the surounding pixels in the current height map 
				float fAverage = 0.0f;
				unsigned int uiCount = 0;

				for (unsigned int m = i - 1; m <= i + 1; m++)
					for (unsigned int n = j - 1; n <= j + 1; n++)
						if ((m >= 0 && m < _uiNumRows) && (n >= 0 && n < _uiNumCols))
						{
							//If mn are valid indices, include in average calculation
							fAverage += vHeightmap[(m * _uiNumCols) + n];
							uiCount++;
						}

				vDest[(i * _uiNumCols) + j] = fAverage / (float)uiCount;
			}

		//Replace the old heightmap with the filtered one
		vHeightmap = vDest;
	}

	//Set Mesh Vertices
	{
		std::vector<stVertex> vVertices(uiVertexCount);
		for (unsigned int x = 0; x < _uiNumCols; x++)
			for (unsigned int y = 0; y < _uiNumRows; y++)
			{
				unsigned int uiVectorIndex = (y * _uiNumCols) + x;
				vVertices[uiVectorIndex] = stVertex
				{
					glm::vec3(x, vHeightmap[uiVectorIndex], y) - (glm::vec3(_uiNumCols,0.0f,_uiNumRows) / 2.0f),
					glm::vec3(0.0f, 1.0f, 1.0f),
					glm::vec2((float)x / (float)_uiNumCols, (float)y / (float)_uiNumRows)
				};
			}

		m_Mesh.SetVerticies(vVertices);
	}

	//Set Mesh Indices
	{
		const unsigned int uiFaceNumber = (_uiNumRows - 1) * (_uiNumCols - 1) * 2;
		std::vector<unsigned int> vIndices(uiFaceNumber * 3);

		unsigned int k = 0;
		for (unsigned int i = 0; i < _uiNumRows - 1; i++)
			for (unsigned int j = 0; j < _uiNumCols - 1; j++)
			{
				vIndices[k] = (i * _uiNumCols) + j;
				vIndices[k + 1] = ((i + 1) * _uiNumCols) + j;
				vIndices[k + 2] = (i * _uiNumCols) + j + 1;

				vIndices[k + 3] = ((i + 1) * _uiNumCols) + j;
				vIndices[k + 4] = ((i + 1) * _uiNumCols) + j + 1;
				vIndices[k + 5] = (i * _uiNumCols) + j + 1;

				k += 6; // next quad
			}

		m_Mesh.SetIndicies(vIndices);
	}
}