#pragma once
class CDecorateLayer
{
public:
	CDecorateLayer();
	virtual ~CDecorateLayer();

	void Initialize();

	void Resize(int width, int height);

	void UpdateData(HDC hdc);

	void Render();

	void Uninitialize();

protected:
	void virtual OnDecorate(HDC hdc);
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }

private:
	GLuint m_uiDecorateProgram;
	GLuint m_uiDecorateVAO;
	GLuint m_uiDecorateTexture;
	GLuint m_uiDecorateBuffer;

	int m_iWidth;
	int m_iHeight;
};

