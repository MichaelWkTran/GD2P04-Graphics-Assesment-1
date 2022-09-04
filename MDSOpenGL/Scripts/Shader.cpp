/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Shader.cpp																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/gtc/type_ptr.hpp>

std::map<const char*, CShader*> CShader::m_mapShaders;
const char* CShader::m_strDirective = "Resources/Shaders/";

CShader::CShader(const char* _pName, std::string _strVertexFile, std::string _strFragmentFile, std::string _strGeometryFile, void(*_pDefaultUniform)(CShader& _Shader))
{
	if (_pName == "") _pName = std::to_string(m_uiID).c_str();
	m_mapShaders.emplace(std::make_pair(m_strName = _pName, this));
	m_pDefaultUniform = _pDefaultUniform;

	//Set up Shaders
	std::string strVertexCode = GetFileContents(m_strDirective + _strVertexFile); const char* pVertexSource = strVertexCode.c_str();
	unsigned int GLuVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLuVertexShader, 1, &pVertexSource, NULL);
	glCompileShader(GLuVertexShader);
	CompileErrors(GLuVertexShader, "VERTEX");

	std::string strFragmentCode = GetFileContents(m_strDirective + _strFragmentFile); const char* pFragmentSource = strFragmentCode.c_str();
	unsigned int GLuFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLuFragmentShader, 1, &pFragmentSource, NULL);
	glCompileShader(GLuFragmentShader);
	CompileErrors(GLuFragmentShader, "FRAGMENT");

	unsigned int GLuGeometryShader = 0;
	if (m_bUsesGeometryShader = (_strGeometryFile != ""))
	{
		std::string strGeometryCode = GetFileContents(m_strDirective + _strGeometryFile); const char* pGeometrySource = strGeometryCode.c_str();
		GLuGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GLuGeometryShader, 1, &pGeometrySource, NULL);
		glCompileShader(GLuGeometryShader);
		CompileErrors(GLuGeometryShader, "GEOMETRY");
	}

	//Create and link to Program
	m_uiID = glCreateProgram();

	glAttachShader(m_uiID, GLuVertexShader);
	glAttachShader(m_uiID, GLuFragmentShader);
	if (m_bUsesGeometryShader) glAttachShader(m_uiID, GLuGeometryShader);

	glLinkProgram(m_uiID);
	CompileErrors(m_uiID, "PROGRAM");

	glDeleteShader(GLuVertexShader);
	glDeleteShader(GLuFragmentShader);
	if (m_bUsesGeometryShader) glDeleteShader(GLuGeometryShader);
}

CShader::~CShader()
{
	m_mapShaders.at(m_strName) = nullptr;
	m_mapShaders.erase(m_strName);
	glDeleteProgram(m_uiID);
}

std::string CShader::GetFileContents(std::string _pFileName)
{
	std::ifstream In(_pFileName, std::ios::binary);
	if (In)
	{
		std::string strContents;
		In.seekg(0, std::ios::end);
		strContents.resize((unsigned int)In.tellg());
		In.seekg(0, std::ios::beg);
		In.read(&strContents[0], strContents.size());
		In.close();
		return strContents;
	}
	throw errno;
}

void CShader::CompileErrors(unsigned int _uShader, std::string _pType)
{
	GLint GLiHasCompiled;
	char cInfoLog[1024];

	if (_pType != "PROGRAM")
	{
		glGetShaderiv(_uShader, GL_COMPILE_STATUS, &GLiHasCompiled);
		if (GLiHasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(_uShader, 1024, NULL, cInfoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << _pType << "\n" << cInfoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(_uShader, GL_LINK_STATUS, &GLiHasCompiled);
		if (GLiHasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(_uShader, 1024, NULL, cInfoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << _pType << "\n" << cInfoLog << std::endl;
		}
	}
}

CShader::operator int() const
{
	return m_uiID;
}

CShader::operator int* ()
{
	return nullptr;
}

const unsigned int& CShader::GetID()
{
	return m_uiID;
}

void CShader::Activate()
{
	glUseProgram(m_uiID);
}

void CShader::Activate(unsigned int _uiID)
{
	glUseProgram(_uiID);
}

void CShader::Deactivate()
{
	glUseProgram(0);
}

bool CShader::Empty()
{
	return m_mapShaders.empty();
}

unsigned int CShader::Size()
{
	return m_mapShaders.size();
}

unsigned int CShader::MaxSize()
{
	return m_mapShaders.max_size();
}

CShader* CShader::At(unsigned int _uiID)
{
	//Seach through the map to find the shader
	for (auto& pShader : m_mapShaders)
	{
		if (pShader.second->GetID() == _uiID) return pShader.second;
	}

	//Return nullptr if no shader is found
	return nullptr;
}

CShader* CShader::Find(const char* _pName)
{
	if (m_mapShaders.find(_pName) == m_mapShaders.end()) return nullptr;
	return m_mapShaders.at(_pName);
}

void CShader::Erase(unsigned int _uiID)
{
	CShader* pShader = At(_uiID);
	if (pShader == nullptr) return;

	delete pShader;
}

void CShader::Erase(std::string _strName)
{
	auto Iterator = m_mapShaders.find(_strName.c_str());
	if (Iterator == m_mapShaders.end()) return void();

	delete (*Iterator).second;
}

void CShader::Clear()
{
	for (auto& pShader : m_mapShaders)
	{
		if (pShader.second != nullptr) delete pShader.second;
		pShader.second = nullptr;
	}

	m_mapShaders.clear();
}

#pragma region Uniform Functions

void CShader::ResetUniforms()
{
	if (m_pDefaultUniform != nullptr)
	{
		m_pDefaultUniform(*this);
	}
}

void CShader::Uniform1f(std::string _pUniform, float _v0)
{
	Activate();
	glUniform1f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0);
	Deactivate();
}

void CShader::Uniform2f(std::string _pUniform, float _v0, float _v1)
{
	Activate();
	glUniform2f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1);
	Deactivate();
}

void CShader::Uniform3f(std::string _pUniform, float _v0, float _v1, float _v2)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2);
	Deactivate();
}

void CShader::Uniform3f(std::string _pUniform, glm::vec3 _v0)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0.x, _v0.y, _v0.z);
	Deactivate();
}

void CShader::Uniform4f(std::string _pUniform, float _v0, float _v1, float _v2, float _v3)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2, _v3);
	Deactivate();
}

void CShader::Uniform4f(std::string _pUniform, glm::vec4 _v0)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0.x, _v0.y, _v0.z, _v0.w);
	Deactivate();
}

void CShader::Uniform1i(std::string _pUniform, int _v0)
{
	Activate();
	glUniform1i(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0);
	Deactivate();
}

void CShader::Uniform2i(std::string _pUniform, int _v0, int _v1)
{
	Activate();
	glUniform2i(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1);
	Deactivate();
}

void CShader::Uniform3i(std::string _pUniform, int _v0, int _v1, int _v2)
{
	Activate();
	glUniform3i(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2);
}

void CShader::Uniform4i(std::string _pUniform, int _v0, int _v1, int _v2, int _v3)
{
	Activate();
	glUniform4i(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2, _v3);
	Deactivate();
}

void CShader::Uniform1ui(std::string _pUniform, unsigned int _v0)
{
	Activate();
	glUniform1ui(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0);
	Deactivate();
}

void CShader::Uniform2ui(std::string _pUniform, unsigned int _v0, unsigned int _v1)
{
	Activate();
	glUniform2ui(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1);
	Deactivate();
}

void CShader::Uniform3ui(std::string _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2)
{
	Activate();
	glUniform3ui(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2);
	Deactivate();
}

void CShader::Uniform4ui(std::string _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3)
{
	Activate();
	glUniform4ui(glGetUniformLocation(m_uiID, _pUniform.c_str()), _v0, _v1, _v2, _v3);
	Deactivate();
}

void CShader::UniformMatrix4fv(std::string _pUniform, int _iCount, bool _bTranspose, glm::mat4 _v)
{
	Activate();
	glUniformMatrix4fv(glGetUniformLocation(m_uiID, _pUniform.c_str()), _iCount, _bTranspose, glm::value_ptr(_v));
	Deactivate();
}

#pragma endregion