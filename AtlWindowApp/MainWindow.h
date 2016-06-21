#pragma once
template <typename TRender>
class CMainWindow :
	public CWindowImpl<CMainWindow<TRender>, CWindow, CFrameWinTraits>
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
		m_Render.Init(m_hWnd);
		return S_OK;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		EndPaint(&ps);
		m_Render.Display();
		return S_OK;
	}

	HRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Resize(LOWORD(lParam), HIWORD(lParam));
		//Invalidate();
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		m_Render.Free();
		PostQuitMessage(0);
	}

private:
	TRender m_Render;
};
