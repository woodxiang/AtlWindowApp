#pragma once
#include "atlbase.h"
#include "MainWindow.h"

class CMainApp :
	public CAtlExeModuleT<CMainApp>
{
public:

	HRESULT PreMessageLoop(_In_ int nShowCmd) throw()
	{
		m_MainWindow.Create(NULL);
		if (m_MainWindow.m_hWnd == NULL)
		{
			return S_FALSE;
		}

		m_MainWindow.ShowWindow(nShowCmd);

		return S_OK;
	}

private:
	CMainWindow m_MainWindow;

};

