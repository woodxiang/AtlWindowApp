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

	bool Init(HWND hWnd) noexcept;

	void Resize(int width, int height) noexcept;

	void Free() noexcept;

	void virtual Render() noexcept;

	void virtual Zoom(short factor) noexcept {}

	void virtual Move(int x, int y) noexcept {}

	void virtual Rotate(int x, int y) noexcept {}

protected:
	void virtual OnRender(float timeescape) noexcept = 0;
	void virtual OnInitialize() noexcept = 0;
	void virtual OnResize(int width, int height) noexcept {	}
	void virtual OnFree() noexcept {	}

#ifdef ENABLE_OPENGL_DEBUG_CONTEXT
	void virtual OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message);
#endif

	static GLuint BuildProgram(HMODULE hModule, UINT vertId, UINT tscId, UINT tesId, UINT geoId, UINT fragId);
	static GLuint BuildProgram(std::string& vert, std::string& tcs, std::string& tes, std::string& geo, std::string& frag);
	static GLuint BuildProgram(std::list<GLuint>& shaders);
	static GLuint LoadShaderFromResource(GLuint shaderType, HMODULE hModule, UINT resourceId);
	static GLuint LoadShaderFromFile(GLuint shaderType, std::string& shaderPath);
	static GLuint LoadShaderFromString(GLuint shaderType, std::string& pStr);

	HDC GetDC() { return m_hDC; }

private:
	static void APIENTRY OpenGLDebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

private:
	HGLRC m_hGlrc;
	HDC m_hDC;
	HWND m_hWnd;

	LARGE_INTEGER lastTimeStamp;
	LARGE_INTEGER freqency;

	friend class CDecorateLayer;
};
