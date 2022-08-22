#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class CShader;

class CShaderManager
{
private:
	static std::map<const char* /*Shader name*/, CShader*> m_mapShaders;

public:
	static std::string GetFileContents(const char* _pFileName);

	static bool Empty();
	static unsigned int Size();
	static unsigned int MaxSize();

	CShader* operator[](unsigned int _uiID);
	static CShader* At(unsigned int _uiID);
	static CShader* Find(const char* _strName);

	static CShader* Insert(const char* _pName, const char* _pVertexFile, const char* _pFragmentFile, const char* _pGeometryFile = "");
	static void Erase(unsigned int _uiID);
	static void Erase(const char* _strName);
	static void Clear();

	static void Activate(unsigned int _uiID);
	static void Deactivate();
};

class CShader
{
	friend CShaderManager;

protected:
	unsigned int m_uiID;
	bool m_bUsesGeometryShader;

	void CompileErrors(unsigned int _uShader, const char* _pType);
	CShader(const char*&& _pVertexFile, const char*&& _pFragmentFile, const char*&& _pGeometryFile = "");

public:
	static std::string GetFileContents(const char* _pFileName);

	CShader(CShader const&) = delete;
	CShader& operator=(const CShader&) = delete;
	~CShader();

	operator int() const;
	explicit operator int* ();
	const unsigned int& GetID();
	void Activate();
	void Deactivate();

	void Uniform1f(const char* _pUniform, float _v0);
	void Uniform2f(const char* _pUniform, float _v0, float _v1);
	void Uniform3f(const char* _pUniform, float _v0, float _v1, float _v2);
	void Uniform3f(const char* _pUniform, glm::vec3 _v0);
	void Uniform4f(const char* _pUniform, float _v0, float _v1, float _v2, float _v3);
	void Uniform4f(const char* _pUniform, glm::vec4 _v0);
	void Uniform1i(const char* _pUniform, int _v0);
	void Uniform2i(const char* _pUniform, int _v0, int _v1);
	void Uniform3i(const char* _pUniform, int _v0, int _v1, int _v2);
	void Uniform4i(const char* _pUniform, int _v0, int _v1, int _v2, int _v3);
	void Uniform1ui(const char* _pUniform, unsigned int _v0);
	void Uniform2ui(const char* _pUniform, unsigned int _v0, unsigned int _v1);
	void Uniform3ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2);
	void Uniform4ui(const char* _pUniform, unsigned int _v0, unsigned int _v1, unsigned int _v2, unsigned int _v3);
	void Uniform1fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform2fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform3fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform4fv(const char* _pUniform, int _iCount, float* _v);
	void Uniform1iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform2iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform3iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform4iv(const char* _pUniform, int _iCount, int* _v);
	void Uniform1uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform2uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform3uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void Uniform4uiv(const char* _pUniform, int _iCount, unsigned int* _v);
	void UniformMatrix4fv(const char* _pUniform, int _iCount, bool _bTranspose, float* _v);
	void UniformMatrix4fv(const char* _pUniform, int _iCount, bool _bTranspose, glm::mat4 _v);
};