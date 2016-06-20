#pragma once
#include "GLRender.h"

class CMainWindow :
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(_T("MainWindow"), CS_HREDRAW | CS_VREDRAW, (HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Init(m_hWnd);
		SetTimer(100, USER_TIMER_MINIMUM, NULL);
		return S_OK;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		EndPaint(&ps);
		return S_OK;
	}

	HRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Resize(LOWORD(lParam), HIWORD(lParam));
		m_Render.Display();
		return S_OK;
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Display();
		return S_OK;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		KillTimer(100);
		m_Render.Free();
		DestroyWindow();
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		PostQuitMessage(0);
	}

private:
	CGLRender m_Render;
};
