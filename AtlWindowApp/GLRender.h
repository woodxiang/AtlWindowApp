#pragma once

#include <string>
#include <fstream>
#include <sstream>

class CGLRender
{
public:

	CGLRender()
	{
		QueryPerformanceFrequency(&freqency);
	}

	bool Init(HWND hWnd)
	{
		if (hWnd == NULL)
			return false;

		m_hWnd = hWnd;
		m_hDC = GetDC(hWnd);
		if (m_hDC == NULL)
		{
			return false;
		}

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		auto pf = ChoosePixelFormat(m_hDC, &pfd);
		if (pf == 0)
		{
			return false;
		}

		if (SetPixelFormat(m_hDC, pf, &pfd) == FALSE)
		{
			return false;
		}

		if (gladLoadWGL(m_hDC))
		{
			UINT gupIndex = 0;
			BOOL bRet = FALSE;
			HGPUNV hGpu = 0;
			do {
				bRet = wglEnumGpusNV(gupIndex, &hGpu);

				if (bRet)
				{
					BOOL bRet2 = FALSE;
					UINT deviceIndex = 0;
					do
					{
						GPU_DEVICE dev;
						bRet2 = wglEnumGpuDevicesNV(hGpu, deviceIndex, &dev);

					} while (bRet2);
				}
			} while (bRet);
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


		QueryPerformanceCounter(&lastTimeStamp);

		OnInitialize();

		return true;
	}

	void Resize(int width, int height)
	{
		if (m_hWnd == NULL)
		{
			return;
		}
		const double range = 3;

		if (height == 0)
			height = 1;

		if (width == 0)
			width = 1;
		wglMakeCurrent(m_hDC, m_hGlrc);

		OnResize(width, height);

		glViewport(0, 0, width, height);
	}

	void Free()
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

	void virtual Render()
	{
		LARGE_INTEGER currentTimeStamp;
		QueryPerformanceCounter(&currentTimeStamp);
		float timeescape = (float)((currentTimeStamp.QuadPart - lastTimeStamp.QuadPart)) / freqency.QuadPart;

		if (timeescape < 0)
			return;

		wglMakeCurrent(m_hDC, m_hGlrc);

		OnRender(timeescape);

		SwapBuffers(m_hDC);
	}

protected:
	void virtual OnRender(float timeescape) = 0;
	void virtual OnInitialize() = 0;
	void virtual OnResize(int width, int height) {	}
	void virtual OnFree() {	}

	static GLuint BuildProgream(std::string& vert, std::string& frag)
	{
		GLuint vertexShader = LoadVertexShader(vert);
		GLuint fragmentShader = LoadFragmentShader(vert);
		GLuint programe = glCreateProgram();
		glAttachShader(programe, vertexShader);
		glAttachShader(programe, fragmentShader);
		glLinkProgram(programe);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return programe;
	}

	static GLuint LoadVertexShader(std::string& shaderPath)
	{
		GLuint shader = glCreateShader(GL_VERTEX_SHADER);

		std::ifstream ifs(shaderPath);
		if (ifs.bad())
		{
			throw std::exception("Invalid vertex shader file.");
		}

		std::stringstream ss;

		ss << ifs.rdbuf();

		std::string content = ss.str();
		char* pConent = const_cast<char*>(content.c_str());
		glShaderSource(shader, 1, &pConent, NULL);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			std::string msg = getCompileShaderErrorMessage(shader);
			throw std::exception(msg.c_str());
		}

		return shader;
	}

	static std::string getCompileShaderErrorMessage(GLuint shader)
	{
		GLsizei bufSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

		std::string str;
		str.resize(bufSize);

		glGetShaderInfoLog(shader, bufSize / sizeof(GLchar), &bufSize, const_cast<char*>(str.c_str()));

		return str;
	}

	static GLuint LoadFragmentShader(std::string& shaderPath)
	{
		GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

		std::ifstream ifs(shaderPath);
		if (ifs.bad())
		{
			throw std::exception("Invalid fragement shader file.");
		}

		std::stringstream ss;

		ss << ifs.rdbuf();

		std::string content = ss.str();
		char* pConent = const_cast<char*>(content.c_str());
		glShaderSource(shader, 1, &pConent, NULL);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			std::string msg = getCompileShaderErrorMessage(shader);
			throw std::exception(msg.c_str());
		}
		return shader;
	}

private:
	HGLRC m_hGlrc;
	HDC m_hDC;
	HWND m_hWnd;

	LARGE_INTEGER lastTimeStamp;
	LARGE_INTEGER freqency;

};
