#include "stdafx.h"
#include "TextInfoDecorator.h"
#include <tchar.h>
#include <Windows.h>
#include <atlstr.h>

CTextInfoDecorator::CTextInfoDecorator()
{
}


CTextInfoDecorator::~CTextInfoDecorator()
{
}


void CTextInfoDecorator::OnDecorate(HDC hMem)
{
	SetBkMode(hMem, TRANSPARENT);
	SetTextColor(hMem, RGB(255, 0, 0));

	for each (auto& str in m_vecMessage)
	{
		int cursor = 0;
		CA2T v(str.c_str());
		SIZE sz;
		GetTextExtentPoint32(hMem, v, (int)str.length(), &sz);
		RECT r = { 0,cursor,sz.cx, sz.cy };
		DrawText(hMem, v, (int)str.length(), &r, 0);

		cursor += sz.cy;
	}
}