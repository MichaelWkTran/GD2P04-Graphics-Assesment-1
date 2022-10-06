/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Shader.h																										*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class CShader
{
private:
	static std::map<const char* /*Shader name*/, CShader*> m_mapShaders;
	const char* m_strName;
	unsigned int m_uiID;

	void CompileErrors(unsigned int _uShader, std::string _pType);

public:
	static const char* m_strDirective;
	void(*m_pDefaultUniform)(CShader& _Shader);

	CShader
	(
		const char* _pName,
		std::string _strVertexFile,
		std::string _strTessControlFile,
		std::string _strTessEvaluationFile,
		std::string _strGeometryFile,
		std::string _strFragmentFile,
		void(*_pDefaultUniform)(CShader& _Shader) = nullptr
	);
	CShader
	(
		const char* _pName,
		std::string _strVertexFile,
		std::string _strFragmentFile,
		void(*_pDefaultUniform)(CShader& _Shader) = nullptr
	) : CShader(_pName, _strVertexFile, "", "", "", _strFragmentFile, _pDefaultUniform) {}
	CShader
	(
		const char* _pName,
		std::string _strVertexFile,
		std::string _strGeometryFile,
		std::string _strFragmentFile,
		void(*_pDefaultUniform)(CShader& _Shader) = nullptr
	) : CShader(_pName, _strVertexFile, "", "", _strGeometryFile, _strFragmentFile, _pDefaultUniform) {}
	CShader(CShader const&) = delete;
	CShader& operator=(const CShader&) = delete;
	~CShader();

	operator int() const;
	explicit operator int*();
	const unsigned int& GetID();
	void Activate();
	static void Activate(unsigned int _uiID);
	static void Deactivate();
	
	static std::string GetFileContents(std::string _pFileName);
	static bool Empty();
	static unsigned int Size();
	static unsigned int MaxSize();
	static CShader* At(unsigned int _uiID);
	static CShader* Find(const char* _pName);
	static void Erase(unsigned int _uiID);
	static void Erase(std::string _strName);
	static void Clear();

	void ResetUniforms();
	void Uniform1f(std::string _pUniform, float _v0);
	void Uniform2f(std::string _pUniform, float _v0, float _v1);
	void Uniform3f(std::string _pUniform, float _v0, float _v1, float _v2);
	void Uniform3f(std::string _pUniform, glm::vec3 _v0);
	void Uniform4f(std::string _pUniform, float _v0, float _v1, float _v2, float _v3);
	void Uniform4f(std::string _pUniform, glm::vec4 _v0);
	void Uniform1i(std::string _pUniform, int _v0);
	void Uniform2i(std::string _pUniform, int _v0, int _v1);
	void Uniform3i(std::string _pUniform, int _v0, int _v1, int _v2);
	void Uniform4i(std::string _pUniform, int _v0, int _v1, int _v2, int _v3);
	void Uniform1ui(std::string _pUniform, unsigned int _v0);
	void Uniform2ui(std::string _pUniform, unsigned int _v0, unsigned int _v1);
	void Uniform3ui(std::string _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2);
	void Uniform4ui(std::string _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3);
	void Uniform1fv(std::string _pUniform, int _iCount, float* _v);
	void Uniform2fv(std::string _pUniform, int _iCount, float* _v);
	void Uniform3fv(std::string _pUniform, int _iCount, float* _v);
	void Uniform4fv(std::string _pUniform, int _iCount, float* _v);
	void Uniform1iv(std::string _pUniform, int _iCount, int* _v);
	void Uniform2iv(std::string _pUniform, int _iCount, int* _v);
	void Uniform3iv(std::string _pUniform, int _iCount, int* _v);
	void Uniform4iv(std::string _pUniform, int _iCount, int* _v);
	void Uniform1uiv(std::string _pUniform, int _iCount, unsigned int* _v);
	void Uniform2uiv(std::string _pUniform, int _iCount, unsigned int* _v);
	void Uniform3uiv(std::string _pUniform, int _iCount, unsigned int* _v);
	void Uniform4uiv(std::string _pUniform, int _iCount, unsigned int* _v);
	void UniformMatrix4fv(std::string _pUniform, int _iCount, bool _bTranspose, float* _v);
	void UniformMatrix4fv(std::string _pUniform, int _iCount, bool _bTranspose, glm::mat4 _v);
};