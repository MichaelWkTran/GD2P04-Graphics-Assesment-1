#include "3DTextureObject.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"

const int texHeight = 200, texWidth = 200, texDepth = 200;
bool tex3Dpattern[texWidth][texHeight][texDepth];
GLubyte* data = nullptr;
CTexture* p3DTexture = nullptr;

void Generate3DData()
{
	if (data != nullptr) return;

	//Generate 3D Pattern
	for (int x = 0; x < texWidth; x++)
		for (int y = 0; y < texHeight; y++)
			for (int z = 0; z < texDepth; z++)
			{
				if ((y / 10) % 2 == 0) tex3Dpattern[x][y][z] = false;
				else tex3Dpattern[x][y][z] = true;
			}

	//Generate 3D Data
	data = new GLubyte[texWidth * texHeight * texDepth * 4];
	for (int i = 0; i < texWidth; i++)
		for (int j = 0; j < texHeight; j++)
			for (int k = 0; k < texDepth; k++)
			{
				if (tex3Dpattern[i][j][k] == true)
				{
					//Yellow Colour
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+0] = (GLubyte)255;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+1] = (GLubyte)255;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+2] = (GLubyte)0;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+3] = (GLubyte)255;
				}
				else
				{
					//Blue Colour
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+0] = (GLubyte)0;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+1] = (GLubyte)0;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+2] = (GLubyte)225;
					data[i * (texWidth*texHeight*4) + j*(texHeight*4) + k*4+3] = (GLubyte)255;
				}
			}

	//Generate 3D Texture
	p3DTexture = new CTexture("3DTexture", 0, GL_TEXTURE_3D);

	p3DTexture->Bind();

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, texWidth, texHeight, texDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, texWidth, texHeight, texDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);

	p3DTexture->Unbind();
}

C3DTextureObject::C3DTextureObject()
{
	Generate3DData();
	m_Mesh.m_mapTextures.insert(std::make_pair("uni_samp3DDiffuse0", p3DTexture));

	m_Mesh.m_pShader = CShader::Find("3DTexture");
	if (m_Mesh.m_pShader == nullptr) m_Mesh.m_pShader = new CShader("3DTexture", "3DTexture.vert", "3DTexture.frag");
}