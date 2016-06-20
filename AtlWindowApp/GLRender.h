#pragma once

#include <gl\GL.h>

class CGLRender
{
public:
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
		return true;
	}

	void Resize(int width, int height)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLdouble aspectRatio = 1.0* width / height;

		if (aspectRatio <= 1)
		{
			glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, -1.0);
		}
		else
		{
			glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, 1.0, -1.0);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
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
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(0, 1);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-1, -1);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2i(1, -1);

		glEnd();
		//glFlush();
		SwapBuffers(m_hDC);
	}

private:
	HGLRC m_hGlrc;
	HDC m_hDC;
	HWND m_hWnd;
};
