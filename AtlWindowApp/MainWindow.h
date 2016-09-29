#pragma once

#include "OpenGLWnd.h"
#include "D2DemoRender.h"

class CMainWindow :
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(_T("Main Window"), CS_HREDRAW | CS_VREDRAW, (HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		GetClientRect(&rc);
		m_RenderWnd.Create(m_hWnd, rc);
		return S_OK;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_RenderWnd.DestroyWindow();
		DestroyWindow();
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		PostQuitMessage(0);
	}

	void Render()
	{
		m_RenderWnd.Render();
	}

private:
	COpenGLWnd<CD2DemoRender> m_RenderWnd;
};
