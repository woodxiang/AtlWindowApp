//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2016] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#include "StdAfx.h"

#include "GLRender.h"

#include <sstream>

CGLRender::CGLRender()
{
	QueryPerformanceFrequency(&freqency);
}
#ifdef ENABLE_OPENGL_DEBUG_CONTEXT
void APIENTRY CGLRender::OpenGLDebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam)
{
	reinterpret_cast<CGLRender*>(userParam)->OnDebugMessage(source, type, id, severity, length, message);
}

#endif

bool CGLRender::Init(HWND hWnd)
{
	if (hWnd == NULL)
		throw std::invalid_argument("hWnd is NULL");

	m_hWnd = hWnd;
	m_hDC = ::GetDC(hWnd);
	if (m_hDC == NULL)
	{
		
	}

	if (!gladLoadGL())
	{
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		auto pf = ChoosePixelFormat(m_hDC, &pfd);
		if (pf == 0)
		{
			return false;
		}

		if (SetPixelFormat(m_hDC, pf, &pfd) == FALSE)
		{
			return false;
		}

		m_hGlrc = wglCreateContext(m_hDC);

		if (m_hGlrc == NULL)
		{
			return false;
		}
		if (!wglMakeCurrent(m_hDC, m_hGlrc))
		{
			return false;
		}

		if (!gladLoadGL())
		{
			return false;
		}

		if (!gladLoadWGL(m_hDC))
		{
			return false;
		}

		wglDeleteContext(m_hGlrc);
	}

	//if (gladLoadWGL(m_hDC))
	//{
	//	UINT gupIndex = 0;
	//	BOOL bRet = FALSE;
	//	HGPUNV hGpu = 0;
	//	do {
	//		bRet = wglEnumGpusNV(gupIndex, &hGpu);

	//		if (bRet)
	//		{
	//			BOOL bRet2 = FALSE;
	//			UINT deviceIndex = 0;
	//			do
	//			{
	//				GPU_DEVICE dev;
	//				bRet2 = wglEnumGpuDevicesNV(hGpu, deviceIndex, &dev);

	//			} while (bRet2);
	//		}
	//	} while (bRet);
	//}

	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0,        //End
	};

	int iPixelFormats;
	unsigned int numFormats = 0;

	if (!wglChoosePixelFormatARB(m_hDC, attribList, NULL, 1, &iPixelFormats, &numFormats))
	{
		return false;
	}

	if (!numFormats)
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));

	DescribePixelFormat(m_hDC, iPixelFormats, sizeof(pfd), &pfd);

	SetPixelFormat(m_hDC, iPixelFormats, &pfd);

	int contextAttributes[]{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
#ifdef ENABLE_OPENGL_DEBUG_CONTEXT
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
		0,0
	};

	m_hGlrc = wglCreateContextAttribsARB(m_hDC, NULL, contextAttributes);
	if (!m_hGlrc)
	{
		return false;
	}

	if (!wglMakeCurrent(m_hDC, m_hGlrc))
	{
		return false;
	}

#ifdef ENABLE_OPENGL_DEBUG_CONTEXT
	glDebugMessageCallback((GLDEBUGPROC)OpenGLDebugProc, this);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	QueryPerformanceCounter(&lastTimeStamp);

	OnInitialize();

	return true;
}

void CGLRender::Resize(int width, int height)
{
	const double range = 3;

	if (height == 0)
		height = 1;

	if (width == 0)
		width = 1;
	wglMakeCurrent(m_hDC, m_hGlrc);

	OnResize(width, height);

	glViewport(0, 0, width, height);
}

void CGLRender::Free()
{
	wglMakeCurrent(m_hDC, m_hGlrc);

	OnFree();

	wglMakeCurrent(NULL, NULL);

	if (m_hWnd != NULL && m_hDC != NULL)
	{
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}

	if (m_hGlrc != NULL)
	{
		wglDeleteContext(m_hGlrc);
		m_hGlrc = NULL;
	}

	m_hWnd = NULL;
}

void CGLRender::Render()
{
	LARGE_INTEGER currentTimeStamp;
	QueryPerformanceCounter(&currentTimeStamp);
	float timeescape = (float)((currentTimeStamp.QuadPart - lastTimeStamp.QuadPart)) / freqency.QuadPart;

	lastTimeStamp = currentTimeStamp;
	if (timeescape < 0)
		return;

	wglMakeCurrent(m_hDC, m_hGlrc);

	OnRender(timeescape);

	SwapBuffers(m_hDC);
}

#ifdef ENABLE_OPENGL_DEBUG_CONTEXT

void CGLRender::OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message)
{
	assert(false);
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
}
#endif

GLuint CGLRender::BuildProgram(HMODULE hModule, UINT vertId, UINT tcsId, UINT tesId, UINT geoId, UINT fragId)
{
	std::list<GLuint> shaders;

	struct {
		GLuint type;
		UINT id;
	} inputs[] = {
		{ GL_VERTEX_SHADER, vertId },
		{ GL_TESS_CONTROL_SHADER, tcsId },
		{ GL_TESS_EVALUATION_SHADER, tesId },
		{ GL_GEOMETRY_SHADER, geoId },
		{ GL_FRAGMENT_SHADER, fragId }
	};

	try
	{
		for each (auto& var in inputs)
		{
			if (var.id != 0)
			{
				GLuint shader = LoadShaderFromResource(var.type, hModule, var.id);
				shaders.push_back(shader);
			}
		}
	}
	catch (COpenglShaderCompileException)
	{
		for each (auto shader in shaders)
		{
			glDeleteShader(shader);
		}

		throw;
	}

	return BuildProgram(shaders);
}

GLuint CGLRender::BuildProgram(std::string& vert, std::string& tcs, std::string& tes, std::string& geo, std::string& frag)
{
	std::list<GLuint> shaders;

	struct {
		GLuint type;
		std::string path;
	} inputs[] = {
		{GL_VERTEX_SHADER, vert},
		{ GL_TESS_CONTROL_SHADER, tcs },
		{ GL_TESS_EVALUATION_SHADER, tes },
		{ GL_GEOMETRY_SHADER, geo },
		{GL_FRAGMENT_SHADER, frag}
	};

	try
	{
		for each (auto& var in inputs)
		{
			if (!var.path.empty())
			{
				GLuint shader = LoadShaderFromFile(var.type, var.path);
				shaders.push_back(shader);
			}
		}
	}
	catch (COpenglShaderCompileException)
	{
		for each (auto shader in shaders)
		{
			glDeleteShader(shader);
		}
		throw;
	}

	return BuildProgram(shaders);
}

GLuint CGLRender::BuildProgram(std::list<GLuint>& shaders)
{
	GLuint program = glCreateProgram();

	for each (auto shader in shaders)
	{
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	for each (auto shader in shaders)
	{
		glDeleteShader(shader);
	}

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		auto& ex = COpenglProgramLinkException(program);
		glDeleteProgram(program);
		throw ex;
	}

	return program;
}

GLuint CGLRender::LoadShaderFromResource(GLuint shaderType, HMODULE hModule, UINT resourceId)
{
	HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(resourceId), _T("TEXT"));
	if (hRes == NULL)
	{
		return 0;
	}

	HGLOBAL hStr = LoadResource(hModule, hRes);

	DWORD size = SizeofResource(hModule, hRes);
	void * pData = LockResource(hStr);

	std::string str = reinterpret_cast<char*>(pData);
	return LoadShaderFromString(shaderType, str);
}

GLuint CGLRender::LoadShaderFromFile(GLuint shaderType, std::string& shaderPath)
{
	std::ifstream ifs(shaderPath);
	if (ifs.bad())
	{
		return 0;
	}

	std::stringstream ss;

	ss << ifs.rdbuf();

	std::string content = ss.str();
	return LoadShaderFromString(shaderType, content);
}

GLuint CGLRender::LoadShaderFromString(GLuint shaderType, std::string& str)
{
	char* pConent = const_cast<char*>(str.c_str());

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &pConent, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		auto& exp = COpenglShaderCompileException(shader);
		glDeleteShader(shader);
		throw exp;
	}

	return shader;
}

COpenglShaderCompileException::COpenglShaderCompileException(GLuint shader)
	:m_szMessage(nullptr), exception()
{
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize != 0)
	{
		m_szMessage = new char[bufSize];
		glGetShaderInfoLog(shader, bufSize / sizeof(GLchar), &bufSize, m_szMessage);
		assert(false);
	}
}

COpenglProgramLinkException::COpenglProgramLinkException(GLuint program)
	:m_szMessage(nullptr), exception()
{
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize != 0)
	{
		m_szMessage = new char[bufSize];
		glGetProgramInfoLog(program, bufSize / sizeof(GLchar), &bufSize, m_szMessage);
		assert(false);
	}
}