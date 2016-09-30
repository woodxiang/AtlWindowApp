#pragma once

#include "OpenGLWnd.h"
#include "BoxLibIn2DRender.h"

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

		auto render = m_RenderWnd.GetRender();

		// init data
		//

		std::vector<int> boxCounts;
		boxCounts.push_back(1);

		std::vector<float> boxData;
		boxData.reserve(8);
		boxData.push_back(-0.25f);
		boxData.push_back(0.25f);
		boxData.push_back(-0.25f);
		boxData.push_back(-0.25f);
		boxData.push_back(0.25f);
		boxData.push_back(-0.25f);
		boxData.push_back(0.25f);
		boxData.push_back(0.25f);
		
		std::vector<float> meshCoords;

		meshCoords.reserve(8);
		meshCoords.push_back(-0.25f);
		meshCoords.push_back(-0.25f);
		meshCoords.push_back(-0.25f);
		meshCoords.push_back(0.25f);
		meshCoords.push_back(0.25f);
		meshCoords.push_back(-0.25f);
		meshCoords.push_back(0.25f);
		meshCoords.push_back(0.25f);

		std::vector<float> meshColors;
		meshColors.reserve(12);
		meshColors.push_back(1.0f);
		meshColors.push_back(0.0f);
		meshColors.push_back(0.0f);

		meshColors.push_back(0.0f);
		meshColors.push_back(1.0f);
		meshColors.push_back(0.0f);

		meshColors.push_back(0.0f);
		meshColors.push_back(0.0f);
		meshColors.push_back(1.0f);

		meshColors.push_back(1.0f);
		meshColors.push_back(1.0f);
		meshColors.push_back(0.0f);
		
		render->UpdateData(boxCounts, boxData, meshCoords, meshColors);
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
	COpenGLWnd<CBoxLibIn2DRender> m_RenderWnd;
};
