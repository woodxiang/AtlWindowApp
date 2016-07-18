#pragma once

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

		m_hGlrc = wglCreateContext(m_hDC);
		if (m_hGlrc == NULL)
		{
			return false;
		}
		if (!wglMakeCurrent(m_hDC, m_hGlrc))
		{
			return false;
		}

		if (!gladLoadGL()) {
			printf("Something went wrong!\n");
			exit(-1);
		}

		QueryPerformanceCounter(&lastTimeStamp);

		return true;
	}

	void Resize(int width, int height)
	{
		if (m_hWnd == NULL)
			return;

		const double range = 3;

		if (height == 0)
			height = 1;

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLdouble aspectRatio = 1.0* width / height;

		if (aspectRatio <= 1)
		{
			glOrtho(-range, range, -range / aspectRatio, range / aspectRatio, range, -range);
		}
		else
		{
			glOrtho(-range * aspectRatio, range * aspectRatio, -range, range, range, -range);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//Display();
	}

	void Free()
	{
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

	void virtual Display()
	{
		LARGE_INTEGER currentTimeStamp;
		QueryPerformanceCounter(&currentTimeStamp);
		int timeescape = (int)((currentTimeStamp.QuadPart - lastTimeStamp.QuadPart) * 1000 / freqency.QuadPart);

		if (timeescape < 0)
			return;

		float colorShift = (float)(timeescape % 10000) / 10000;

		glClear(GL_COLOR_BUFFER_BIT);

		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POINT_SMOOTH);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f - colorShift, 1.0f - colorShift, 1.0f - colorShift);
		glVertex2i(0, 1);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-1, -1);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2i(1, -1);
		glEnd();

		glPointSize(100);

		glBegin(GL_POINTS);
		glVertex2d(2.0, 2.0);
		glEnd();

		SwapBuffers(m_hDC);
	}

private:
	HGLRC m_hGlrc;
	HDC m_hDC;
	HWND m_hWnd;

	LARGE_INTEGER lastTimeStamp;
	LARGE_INTEGER freqency;
};
