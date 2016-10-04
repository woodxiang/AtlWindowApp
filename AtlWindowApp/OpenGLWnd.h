//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2016] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#pragma once

template <class CRender>
class COpenGLWnd :
	public CWindowImpl<COpenGLWnd<CRender>, CWindow, CControlWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(_T("OpenGLWindow"), CS_HREDRAW | CS_VREDRAW, (HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseLeftButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseLeftButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnMouseRightButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnMouseRightButtonUp)

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
		return S_OK;
	}

	HRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Resize(LOWORD(lParam), HIWORD(lParam));
		return S_OK;
	}

	HRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Resize(LOWORD(lParam), HIWORD(lParam));
		return S_OK;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Free();
		DestroyWindow();
		return S_OK;
	}

	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_Render.Zoom(zDelta);
		return S_OK;
	}

	LRESULT OnMouseLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::SetCapture(m_hWnd);

		if (m_hOldCursor == 0)
		{
			m_hOldCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		}

		m_iLastXPosition = GET_X_LPARAM(lParam);
		m_iLastYPosition = GET_Y_LPARAM(lParam);

		m_enumState = Moving;

		return S_OK;
	}
	LRESULT OnMouseLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_enumState == Moving)
		{
			m_enumState = Normal;
			if (m_hOldCursor != 0)
			{
				SetCursor(m_hOldCursor);
				m_hOldCursor = 0;
			}

			::ReleaseCapture();
		}
		return S_OK;
	}

	LRESULT OnMouseRightButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::SetCapture(m_hWnd);

		if (m_hOldCursor == 0)
		{
			m_hOldCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		}

		m_iLastXPosition = GET_X_LPARAM(lParam);
		m_iLastYPosition = GET_Y_LPARAM(lParam);

		m_enumState = Rotating;

		return S_OK;
	}
	LRESULT OnMouseRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_enumState == Rotating)
		{
			m_enumState = Normal;
			if (m_hOldCursor != 0)
			{
				SetCursor(m_hOldCursor);
				m_hOldCursor = 0;
			}

			::ReleaseCapture();
		}
		return S_OK;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_enumState == Moving)
		{
			m_Render.Move(GET_X_LPARAM(lParam) - m_iLastXPosition, GET_Y_LPARAM(lParam) - m_iLastYPosition);
		}
		else if (m_enumState == Rotating)
		{
			m_Render.Rotate(GET_X_LPARAM(lParam) - m_iLastXPosition, GET_Y_LPARAM(lParam) - m_iLastYPosition);
		}
		m_iLastXPosition = GET_X_LPARAM(lParam);
		m_iLastYPosition = GET_Y_LPARAM(lParam);
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		PostQuitMessage(0);
	}

	void Render()
	{
		m_Render.Render();
	}

	CRender* GetRender() { return &m_Render; }

private:
	enum State
	{
		Normal,
		Moving,
		Rotating,
	} m_enumState;

	CRender m_Render;

	HCURSOR m_hOldCursor;
	int m_iLastXPosition = -1;
	int m_iLastYPosition = -1;
};