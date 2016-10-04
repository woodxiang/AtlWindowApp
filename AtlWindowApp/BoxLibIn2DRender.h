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

#include "GLRender.h"
#include <vector>
#include <glm/glm.hpp>
#include "TextInfoDecorator.h"

class CBoxLibIn2DRender :
	public CGLRender
{
public:

	CBoxLibIn2DRender()
	{
	}

	virtual ~CBoxLibIn2DRender()
	{
	}

	void UpdateData(std::vector<int>& boxCounts, std::vector<float>& boxData, std::vector<float>& meshCoords, std::vector<float>& meshColors);

	void Zoom(short factor) noexcept;

	void Move(int x, int y) noexcept;

	void Rotate(int x, int y) noexcept;

protected:
	void  OnRender(float timeescape) noexcept;

	void OnInitialize() noexcept;

	void OnResize(int width, int height) noexcept;

	void OnFree() noexcept;

	void ClearVectexArray();

	void ClearBuffers();

private:

	std::vector<int> m_boxCounts;
	int m_szMeshCount;

	GLuint m_boxProg;
	GLuint m_meshProg;
	GLuint m_boxVAO;
	GLuint m_boxBuf;
	GLuint m_meshVAO;
	GLuint m_meshCoordsBuf;
	GLuint m_meshColorsBuf;

	GLint m_uniformColorLocation;
	GLint m_projectionLocationInBoxProgram;
	GLint m_projectionLocationInMeshProgram;

	glm::mat4 m_matrixProjection;
	glm::mat4 m_matrixCamera;
	glm::mat4 m_matrixModel;

	float m_screenScale;

	CTextInfoDecorator m_decorateLayer;

	float m_lastFrameTime;

	bool m_bIsPerspective;
};
