#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/gtc/type_ptr.hpp>

#pragma region CShaderManager functions

std::map<const char*, CShader*> CShaderManager::m_mapShaders;

std::string CShaderManager::GetFileContents(const char* _pFileName)
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

bool CShaderManager::Empty()
{
	return m_mapShaders.empty();
}

unsigned int CShaderManager::Size()
{
	return m_mapShaders.size();
}

unsigned int CShaderManager::MaxSize()
{
	return m_mapShaders.max_size();
}

CShader* CShaderManager::operator[](unsigned int _uiID)
{
	return At(_uiID);
}

CShader* CShaderManager::At(unsigned int _uiID)
{
	//If the the requested ID is out of bounds, then no shader is found
	if (_uiID > m_mapShaders.size()) return nullptr;

	//Seach through the map to find the shader
	for (auto& pShader : m_mapShaders)
	{
		if (pShader.second->GetID() == _uiID) return pShader.second;
	}

	//Return nullptr if no shader is found
	return nullptr;
}

CShader* CShaderManager::Find(const char* _strName)
{
	return m_mapShaders.at(_strName);
}

CShader* CShaderManager::Insert(const char* _pName, const char* _pVertexFile, const char* _pFragmentFile, const char* _pGeometryFile)
{
	CShader* pShader = new CShader(std::move(_pVertexFile), std::move(_pFragmentFile), std::move(_pGeometryFile));
	m_mapShaders.insert(std::pair<const char*, CShader*>(_pName, pShader));

	return pShader;
}

void CShaderManager::Erase(unsigned int _uiID)
{
	//If the the requested ID is out of bounds, then no shader is found
	if (_uiID > m_mapShaders.size()) return;

	//Seach through the map to find the shader
	for (auto& pShader : m_mapShaders)
	{
		if (pShader.second->GetID() == _uiID)
		{
			const char* pShaderName = pShader.first;

			delete pShader.second;
			pShader.second = nullptr;
			m_mapShaders.erase(pShaderName);

			return;
		}
	}
}

void CShaderManager::Erase(const char* _strName)
{
	auto Iterator = m_mapShaders.find(_strName);
	if (Iterator == m_mapShaders.end()) return void();

	delete (*Iterator).second;
	(*Iterator).second = nullptr;
	m_mapShaders.erase(Iterator);
}

void CShaderManager::Clear()
{
	for (auto& Shader : m_mapShaders)
	{
		if (Shader.second != nullptr) delete Shader.second;
		Shader.second = nullptr;
	}

	m_mapShaders.clear();
}

void CShaderManager::Activate(unsigned int _uiID)
{
	glUseProgram(_uiID);
}

void CShaderManager::Deactivate()
{
	glUseProgram(0);
}

#pragma endregion

#pragma region CShader functions

void CShader::CompileErrors(unsigned int _uShader, const char* _pType)
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

std::string CShader::GetFileContents(const char* _pFileName)
{
	return CShaderManager::GetFileContents(_pFileName);
}

CShader::CShader(const char*&& _pVertexFile, const char*&& _pFragmentFile, const char*&& _pGeometryFile/* = ""*/)
{
	//Set up Shaders
	std::string strVertexCode = GetFileContents(_pVertexFile); const char* pVertexSource = strVertexCode.c_str();
	unsigned int GLuVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLuVertexShader, 1, &pVertexSource, NULL);
	glCompileShader(GLuVertexShader);
	CompileErrors(GLuVertexShader, "VERTEX");

	std::string strFragmentCode = GetFileContents(_pFragmentFile); const char* pFragmentSource = strFragmentCode.c_str();
	unsigned int GLuFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLuFragmentShader, 1, &pFragmentSource, NULL);
	glCompileShader(GLuFragmentShader);
	CompileErrors(GLuFragmentShader, "FRAGMENT");

	unsigned int GLuGeometryShader = 0;
	if (_pGeometryFile == "")
	{
		m_bUsesGeometryShader = false;
	}
	else
	{
		m_bUsesGeometryShader = true;
		std::string strGeometryCode = GetFileContents(_pGeometryFile); const char* pGeometrySource = strGeometryCode.c_str();
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
	glDeleteProgram(m_uiID);
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

void CShader::Deactivate()
{
	glUseProgram(0);
}

void CShader::Uniform1f(const char* _pUniform, float _v0)
{
	Activate();
	glUniform1f(glGetUniformLocation(m_uiID, _pUniform), _v0);
}

void CShader::Uniform2f(const char* _pUniform, float _v0, float _v1)
{
	Activate();
	glUniform2f(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1);
}

void CShader::Uniform3f(const char* _pUniform, float _v0, float _v1, float _v2)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform3f(const char* _pUniform, glm::vec3 _v0)
{
	Activate();
	glUniform3f(glGetUniformLocation(m_uiID, _pUniform), _v0.x, _v0.y, _v0.z);
}

void CShader::Uniform4f(const char* _pUniform, float _v0, float _v1, float _v2, float _v3)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2, _v3);
}

void CShader::Uniform4f(const char* _pUniform, glm::vec4 _v0)
{
	Activate();
	glUniform4f(glGetUniformLocation(m_uiID, _pUniform), _v0.x, _v0.y, _v0.z, _v0.w);
}

void CShader::Uniform1i(const char* _pUniform, int _v0)
{
	Activate();
	glUniform1i(glGetUniformLocation(m_uiID, _pUniform), _v0);
}

void CShader::Uniform2i(const char* _pUniform, int _v0, int _v1)
{
	Activate();
	glUniform2i(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1);
}

void CShader::Uniform3i(const char* _pUniform, int _v0, int _v1, int _v2)
{
	Activate();
	glUniform3i(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform4i(const char* _pUniform, int _v0, int _v1, int _v2, int _v3)
{
	Activate();
	glUniform4i(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2, _v3);
}

void CShader::Uniform1ui(const char* _pUniform, unsigned int _v0)
{
	Activate();
	glUniform1ui(glGetUniformLocation(m_uiID, _pUniform), _v0);
}

void CShader::Uniform2ui(const char* _pUniform, unsigned int _v0, unsigned int _v1)
{
	Activate();
	glUniform2ui(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1);
}

void CShader::Uniform3ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2)
{
	Activate();
	glUniform3ui(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2);
}

void CShader::Uniform4ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3)
{
	Activate();
	glUniform4ui(glGetUniformLocation(m_uiID, _pUniform), _v0, _v1, _v2, _v3);
}

void CShader::UniformMatrix4fv(const char* _pUniform, int _iCount, bool _bTranspose, glm::mat4 _v)
{
	Activate();
	glUniformMatrix4fv(glGetUniformLocation(m_uiID, _pUniform), _iCount, _bTranspose, glm::value_ptr(_v));
}

#pragma endregion