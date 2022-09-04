/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: 3DTextureObject.cpp																							*/
/*  DATE: Aug 25th, 2022																								*/
/************************************************************************************************************************/

#include "3DTextureObject.h"
#include "Texture.h"
#include "Shader.h"

const int iTexHeight = 200, iTexWidth = 200, iTexDepth = 200, iGridSize = 10;
bool bTex3Dpattern[iTexWidth][iTexHeight][iTexDepth];
GLubyte* pData = nullptr;
CTexture* p3DTexture = nullptr;

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Generate3DData()
//	 Purpose: 
//	 Returns: 

void Generate3DData()
{
	if (pData != nullptr) return;

	//Generate 3D Pattern
	for (int iPixelX = 0; iPixelX < iTexWidth; iPixelX++)
		for (int iPixelY = 0; iPixelY < iTexHeight; iPixelY++)
			for (int iPixelZ = 0; iPixelZ < iTexDepth; iPixelZ++)
			{
				bool bEvenXCell = (iPixelX / iGridSize) % 2 == 0;
				bool bEvenYCell = (iPixelY / iGridSize) % 2 == 0;
				bool bEvenZCell = (iPixelZ / iGridSize) % 2 == 0;

				if (bEvenXCell == bEvenYCell) { if (bEvenZCell == true) bTex3Dpattern[iPixelX][iPixelY][iPixelZ] = true; }
				else { if (bEvenZCell == false) bTex3Dpattern[iPixelX][iPixelY][iPixelZ] = true; }
			}

	//Generate 3D Data
	pData = new GLubyte[iTexWidth * iTexHeight * iTexDepth * 4];
	for (int iPixelX = 0; iPixelX < iTexWidth; iPixelX++)
		for (int iPixelY = 0; iPixelY < iTexHeight; iPixelY++)
			for (int iPixelZ = 0; iPixelZ < iTexDepth; iPixelZ++)
			{
				if (bTex3Dpattern[iPixelX][iPixelY][iPixelZ] == true)
				{
					//Yellow Colour
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+0] = (GLubyte)255;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+1] = (GLubyte)255;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+2] = (GLubyte)0;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+3] = (GLubyte)255;
				}
				else
				{
					//Blue Colour
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+0] = (GLubyte)0;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+1] = (GLubyte)0;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+2] = (GLubyte)225;
					pData[iPixelX * (iTexWidth*iTexHeight*4) + iPixelY*(iTexHeight*4) + iPixelZ*4+3] = (GLubyte)255;
				}
			}

	//Generate 3D Texture
	p3DTexture = new CTexture("3DTexture", 0, GL_TEXTURE_3D);

	p3DTexture->Bind();

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, iTexWidth, iTexHeight, iTexDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, iTexWidth, iTexHeight, iTexDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, pData);

	p3DTexture->Unbind();
}

//------------------------------------------------------------------------------------------------------------------------
// Procedure: Generate3DData()
//	 Purpose: 
//	 Returns: 

C3DTextureObject::C3DTextureObject()
{
	//Generate a 3D texture and apply it to the mesh
	Generate3DData();
	m_Mesh.m_mapTextures.insert(std::make_pair("uni_samp3DDiffuse0", p3DTexture));
	
	//Generate a new shader for the 3D object if none is found
	m_Mesh.m_pShader = CShader::Find("3DTexture");
	if (m_Mesh.m_pShader == nullptr) m_Mesh.m_pShader = new CShader("3DTexture", "3DTexture.vert", "3DTexture.frag");
}