#pragma once

#include <gl\GL.h>

class CGLRender
{
public:
	bool Init(HWND hWnd)
	{
		HDC hDC = GetDC(hWnd);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		auto pf = ChoosePixelFormat(hDC, &pfd);
		if (pf == 0)
		{
			goto error_exit;
		}

		if (SetPixelFormat(hDC, pf, &pfd) == FALSE)
		{
			goto error_exit;
		}

		m_hGlrc = wglCreateContext(hDC);
		if (m_hGlrc == NULL)
		{
			goto error_exit;
		}
		if (!wglMakeCurrent(hDC, m_hGlrc))
		{
			goto error_exit;
		}
		ReleaseDC(hWnd, hDC);
		return true;

	error_exit:
		ReleaseDC(hWnd, hDC);
		return false;
	}

	void Resize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Free()
	{
		wglMakeCurrent(NULL, NULL);
		if (m_hGlrc != NULL)
		{
			wglDeleteContext(m_hGlrc);
			m_hGlrc = NULL;
		}
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
		glFlush();
	}

private:
	HGLRC m_hGlrc;
};
