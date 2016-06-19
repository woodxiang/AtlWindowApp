#pragma once
#include <gl\GL.h>

class CMainWindow :
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(_T("MainWindow"), CS_HREDRAW | CS_VREDRAW, (HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = GetDC();

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
			MessageBox(_T("Choose PixelFormat Failed."), _T("ERROR"), MB_OK);
			goto error_exit;
		}

		if (SetPixelFormat(hDC, pf, &pfd) == FALSE)
		{
			MessageBox(_T("Set PixelFormat Failed."), _T("ERROR"), MB_OK);
			goto error_exit;
		}

		m_hGlrc = wglCreateContext(hDC);
		if (m_hGlrc == NULL)
		{
			MessageBox(_T("Create GL Context Failed."), _T("ERROR"), MB_OK);
			goto error_exit;
		}
		if (!wglMakeCurrent(hDC, m_hGlrc))
		{
			MessageBox(_T("Set Current GL Context Failed."), _T("ERROR"), MB_OK);
		}

	error_exit:
		ReleaseDC(hDC);
		return S_OK;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		EndPaint(&ps);
		Display();
		return S_OK;
	}

	HRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		//Invalidate();
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		wglMakeCurrent(NULL, NULL);
		if (m_hGlrc != NULL)
		{
			wglDeleteContext(m_hGlrc);
			m_hGlrc = NULL;
		}
		PostQuitMessage(0);
	}

	void Display()
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
