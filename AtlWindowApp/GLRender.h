//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2016] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#pragma once

#include <list>
#include <string>

class CGLRender
{
public:

	CGLRender();

	bool Init(HWND hWnd);

	void Resize(int width, int height);

	void Free();

	void virtual Render();

	void virtual Zoom(short factor) {}

	void virtual Move(int x, int y) {}

protected:
	void virtual OnRender(float timeescape) = 0;
	void virtual OnInitialize() = 0;
	void virtual OnResize(int width, int height) {	}
	void virtual OnFree() {	}

#ifdef ENABLE_OPENGL_DEBUG_CONTEXT
	void virtual OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message);
#endif

	static GLuint BuildProgram(HMODULE hModule, UINT vertId, UINT tscId, UINT tesId, UINT geoId, UINT fragId);
	static GLuint BuildProgram(std::string& vert, std::string& tcs, std::string& tes, std::string& geo, std::string& frag);
	static GLuint BuildProgram(std::list<GLuint>& shaders);
	static GLuint LoadShaderFromResource(GLuint shaderType, HMODULE hModule, UINT resourceId);
	static GLuint LoadShaderFromFile(GLuint shaderType, std::string& shaderPath);
	static GLuint LoadShaderFromString(GLuint shaderType, std::string& pStr);
	static std::string getCompileShaderErrorMessage(GLuint shader);

	HDC GetDC() { return m_hDC; }

private:
	static void APIENTRY OpenGLDebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

private:
	HGLRC m_hGlrc;
	HDC m_hDC;
	HWND m_hWnd;

	LARGE_INTEGER lastTimeStamp;
	LARGE_INTEGER freqency;
};
