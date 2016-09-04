#pragma once

#include "RibbonApp.h"

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
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEreaseBackground)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HRESULT hr = m_pRibbonFramework.CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER);
		if (FAILED(hr))
		{
			return hr;
		}

		CComObject<CRibbonApp> *ribbonApp = NULL;
		hr = CComObject<CRibbonApp>::CreateInstance(&ribbonApp);
		if (FAILED(hr))
		{
			return hr;
		}

		m_pRibbonFramework->Initialize(m_hWnd, ribbonApp);

		m_pRibbonFramework->LoadUI(GetModuleHandle(NULL), _T("APPLICATION_RIBBON"));

		m_Render.Init(m_hWnd);
		return S_OK;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Render();
		return S_OK;
	}

	HRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Resize(LOWORD(lParam), HIWORD(lParam));
		return S_OK;
	}

	HRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return S_OK;
	}

	LRESULT OnEreaseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return S_OK;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Render.Free();
		m_pRibbonFramework.Release();
		DestroyWindow();
		return S_OK;
	}

	void OnFinalMessage(HWND hWnd)
	{
		PostQuitMessage(0);
	}

	void Refresh()
	{
		//m_Render.Display();
	}

private:
	TRender m_Render;
	CComPtr<IUIFramework> m_pRibbonFramework;
};
