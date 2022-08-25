#include "LightManager.h"
#include <fstream>
#include "Shader.h"

//Set Max Light Number
unsigned int stInfinitePointLight::uiMaxNumber = 10U;
unsigned int stPointLight::uiMaxNumber = 10U;
unsigned int stDirectionalLight::uiMaxNumber = 10U;
unsigned int stSpotLight::uiMaxNumber = 10U;

//Set Ambient Colour
glm::vec4 CLightManager::m_v4AmbientColour = glm::vec4(1.0f,1.0f,1.0f,0.2f);

//Set Light Vectors
std::vector<stInfinitePointLight> CLightManager::m_vInfinitePointLight;
std::vector<stPointLight> CLightManager::m_vPointLight;
std::vector<stDirectionalLight> CLightManager::m_vDirectionalLight;
std::vector<stSpotLight> CLightManager::m_vSpotLight;

const unsigned int CLightManager::TotalLights()
{
	return m_vInfinitePointLight.size() + m_vPointLight.size() + m_vDirectionalLight.size() + m_vSpotLight.size();
}

bool CLightManager::UpdateDiffuseShader(const char* _pShaderFile)
{
	//WARNING NEVER USE IN RUNTIME

	auto lamModifyLightTypeNumDefine = [](std::string& _strBuffer, const char* _strSequence, int _iLightTypeCount, int& _iLightTypeIndex)
	{
		if (_strBuffer.find(_strSequence) == std::string::npos) return;
		
		if (_iLightTypeCount <= 0)_iLightTypeCount = 1;
		_strBuffer = _strSequence + std::to_string(_iLightTypeCount);
		_iLightTypeIndex++;
	};

	std::string strBuffer;
	std::ifstream ifstreamFile(_pShaderFile); if (!ifstreamFile.is_open()) return false;
	std::ofstream ofstreamFile("temp.txt"); if (!ofstreamFile.is_open()) { ifstreamFile.close(); return false; }

	int iLightTypeIndex = 0;
	while (!ifstreamFile.eof())
	{
		getline(ifstreamFile, strBuffer);

		//Find the line to replace in the old file and
		//replace it with the desired line
		switch (iLightTypeIndex)
		{
		case 0:
			lamModifyLightTypeNumDefine(strBuffer, "#define MAX_INF_POINT_LIGHT ", stInfinitePointLight::uiMaxNumber, iLightTypeIndex);
			break;
		case 1:
			lamModifyLightTypeNumDefine(strBuffer, "#define MAX_POINT_LIGHT ", stPointLight::uiMaxNumber, iLightTypeIndex);
			break;
		case 2:
			lamModifyLightTypeNumDefine(strBuffer, "#define MAX_DIRECTIONAL_LIGHT ", stDirectionalLight::uiMaxNumber, iLightTypeIndex);
			break;
		case 3:
			lamModifyLightTypeNumDefine(strBuffer, "#define MAX_SPOT_LIGHT ", stSpotLight::uiMaxNumber, iLightTypeIndex);
			break;
		}

		//Write to the temporary file
		ofstreamFile << strBuffer;

		//Add a line if not the end of the file
		if (!ifstreamFile.eof()) ofstreamFile << std::endl;
	}

	ifstreamFile.close();
	ofstreamFile.close();

	remove(_pShaderFile);
	return rename("temp.txt", _pShaderFile) == 0;
}

bool CLightManager::UpdateDiffuseShaders(std::vector<const char*> _vShaderFiles)
{
	//WARNING NEVER USE IN RUNTIME

	for (auto pShaderFiles : _vShaderFiles)
	{
		if (!UpdateDiffuseShader(pShaderFiles)) return false;
	}

	return true;
}

void CLightManager::UpdateShaderUniforms(CShader* _pShader)
{
	_pShader->Activate();

	//Set the uniforms foreach light type
	for (int i = 0; i < (int)m_vInfinitePointLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_InfinitePointLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vInfinitePointLight[i].v3LightPosition);
		_pShader->Uniform4f(("uni_InfinitePointLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vInfinitePointLight[i].v4LightColour);
	}
	
	for (int i = 0; i < (int)m_vPointLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_PointLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vPointLight[i].v3LightPosition);
		_pShader->Uniform4f(("uni_PointLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vPointLight[i].v4LightColour);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationExponent").c_str(), m_vPointLight[i].fAttenuationExponent);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationLinear").c_str(), m_vPointLight[i].fAttenuationLinear);
		_pShader->Uniform1f(("uni_PointLight[" + std::to_string(i) + "].fAttenuationConstant").c_str(), m_vPointLight[i].fAttenuationConstant);
	}

	for (int i = 0; i < (int)m_vDirectionalLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_DirectionalLight[" + std::to_string(i) + "].v3LightDirection").c_str(), m_vDirectionalLight[i].v3LightDirection);
		_pShader->Uniform4f(("uni_DirectionalLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vDirectionalLight[i].v4LightColour);
	}

	for (int i = 0; i < (int)m_vSpotLight.size(); i++)
	{
		_pShader->Uniform3f(("uni_SpotLight[" + std::to_string(i) + "].v3LightPosition").c_str(), m_vSpotLight[i].v3LightPosition);
		_pShader->Uniform3f(("uni_SpotLight[" + std::to_string(i) + "].v3LightDirection").c_str(), m_vSpotLight[i].v3LightDirection);
		_pShader->Uniform4f(("uni_SpotLight[" + std::to_string(i) + "].v4LightColour").c_str(), m_vSpotLight[i].v4LightColour);
		_pShader->Uniform1f(("uni_SpotLight[" + std::to_string(i) + "].fOuterCone").c_str(), m_vSpotLight[i].fOuterCone);
		_pShader->Uniform1f(("uni_SpotLight[" + std::to_string(i) + "].fInnerCone").c_str(), m_vSpotLight[i].fInnerCone);
	}
	
	//Check whether the type of light is used or not
	
	//Set the uniforms for how may lights are in the world for each type
	_pShader->Uniform1i("uni_iInfinitePointLightNum", m_vInfinitePointLight.size());
	_pShader->Uniform1i("uni_iPointLightNum"		, m_vPointLight.size());
	_pShader->Uniform1i("uni_iDirectionalLightNum"	, m_vDirectionalLight.size());
	_pShader->Uniform1i("uni_iSpotLightNum"			, m_vSpotLight.size());

	//Set uni_v4AmbientColour
	_pShader->Uniform4f("uni_v4AmbientColour", m_v4AmbientColour);

	_pShader->Deactivate();
}

void CLightManager::UpdateShaderUniforms(std::vector<CShader*> _vShaders)
{
	for (auto pShader : _vShaders)
	{
		UpdateShaderUniforms(pShader);
	}
}