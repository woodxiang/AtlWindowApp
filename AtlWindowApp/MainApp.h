#pragma once
#include "atlbase.h"
#include "MainWindow.h"
#include "ChangeBackgroudRender.h"
#include "GLRender.h"

class CMainApp :
	public CAtlExeModuleT<CMainApp>
{
public:

	HRESULT PreMessageLoop(_In_ int nShowCmd) noexcept
	{
		HRESULT hr = CAtlExeModuleT<CMainApp>::PreMessageLoop(nShowCmd);
		if (FAILED(hr))
			return hr;

		hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(hr))
		{
			return hr;
		}
		m_MainWindow.Create(NULL);
		if (m_MainWindow.m_hWnd == NULL)
		{
			return S_FALSE;
		}

		m_MainWindow.ShowWindow(nShowCmd);

		return S_OK;
	}

	HRESULT PostMessageLoop() throw()
	{
		HRESULT hr = __super::PostMessageLoop();
		if (FAILED(hr))
			return hr;
		::CoUninitialize();
		return S_OK;
	}

	void RunMessageLoop() noexcept
	{
		MSG msg;

		do
		{
			if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				OnIdel();
			}
			else
			{
				if (msg.message != WM_QUIT)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		} while (msg.message != WM_QUIT);
	}

	void OnIdel()
	{
		m_MainWindow.Refresh();
	}

private:
	CMainWindow<CChangeBackgroudRender> m_MainWindow;

};

