#pragma once

class CMainWindow :
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(_T("MainWindow"), CS_HREDRAW | CS_VREDRAW, (HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		EndPaint(&ps);
		return 0;
	}

	void OnFinalMessage(HWND hWnd)
	{
		PostQuitMessage(0);
	}
};
