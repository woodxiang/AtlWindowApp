//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2016] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#include "stdafx.h"
#include "glad\glad.h"
#include "DecorateLayer.h"
#include "GLRender.h"
#include <vector>
#include <cassert>

CDecorateLayer::CDecorateLayer()
{
}

CDecorateLayer::~CDecorateLayer()
{
}

void CDecorateLayer::Initialize()
{
	GLuint prog = CGLRender::BuildProgram(std::string("glsl\\textout.vert"), std::string(), std::string(), std::string(), std::string("glsl\\textout.frag"));

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_uiDecorateProgram = prog;
	m_uiDecorateVAO = vao;
}

void CDecorateLayer::Resize(int width, int height)
{
	if (m_uiDecorateTexture != 0)
	{
		glDeleteTextures(1, &m_uiDecorateTexture);
	}

	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA32F, width, height);

	m_uiDecorateTexture = texture;
	m_iWidth = width;
	m_iHeight = height;
}

void CDecorateLayer::UpdateData(HDC hdc)
{
	std::vector<float> data(m_iWidth*m_iHeight * 4);

	HDC hBmpDc = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, m_iWidth, m_iHeight);
	SelectObject(hBmpDc, hBmp);

	OnDecorate(hBmpDc);

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	DWORD bmpStrip = (bmp.bmWidth * bmp.bmBitsPixel + 31) / 32;
	DWORD bmpBufSize = bmpStrip * 4 * bmp.bmHeight;

	std::vector<unsigned char> buf(bmpBufSize);

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmp.bmWidth;
	bi.biHeight = bmp.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	GetDIBits(hBmpDc, hBmp, 0, (UINT)bmp.bmHeight, buf.data(), (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	HANDLE hFile = CreateFile(L"captureqwsx.bmp",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = bmpBufSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)buf.data(), bmpBufSize, &dwBytesWritten, NULL);

	CloseHandle(hFile);
	DeleteDC(hBmpDc);
	DeleteObject(hBmp);

	for (int j = 0; j < m_iHeight; j++)
	{
		for (int i = 0; i < m_iWidth; i++)
		{
			unsigned char* pSrc = buf.data() + (j*bmpStrip + i) * 4;
			float* pTar = data.data() + (j* m_iWidth + i) * 4;

			*pTar = ((float)(*(pSrc + 2))) / 255.0f;
			*(pTar + 1) = ((float)(*(pSrc + 1))) / 255.0f;
			*(pTar + 2) = ((float)(*(pSrc))) / 255.0f;

			*(pTar + 3) = ((int)(*pSrc) + *(pSrc + 1) + *(pSrc + 2)) > 0 ? 1.0f : 0.0f;
		}
	}

	glTextureSubImage2D(m_uiDecorateTexture, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_FLOAT, data.data());
}

void CDecorateLayer::Uninitialize()
{
	glDeleteTextures(1, &m_uiDecorateTexture);
	glDeleteVertexArrays(1, &m_uiDecorateVAO);
	glDeleteProgram(m_uiDecorateProgram);
}

void CDecorateLayer::Render()
{
	glUseProgram(m_uiDecorateProgram);
	glBindTexture(GL_TEXTURE_2D, m_uiDecorateTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CDecorateLayer::OnDecorate(HDC hMem)
{

}