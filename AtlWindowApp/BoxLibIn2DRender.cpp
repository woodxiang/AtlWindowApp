//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2016] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#include "StdAfx.h"

#include "BoxLibIn2DRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Exceptions.h"

void CBoxLibIn2DRender::UpdateData(std::vector<int>& boxCounts, std::vector<float>& boxData, std::vector<float>& meshCoords, std::vector<float>& meshColors)
{
	ClearBuffers();

	GLuint buf[2];

	// update box
	//

	if (boxData.size() > 0)
	{
		glBindVertexArray(m_boxVAO);
		glCreateBuffers(1, &buf[0]);
		glNamedBufferStorage(buf[0], boxData.size() * sizeof(GLfloat), boxData.data(), 0);
		glVertexArrayVertexBuffer(m_boxVAO, 0, buf[0], 0, sizeof(GLfloat) * 2);
		glVertexArrayAttribFormat(m_boxVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_boxVAO, 0, 0);
		glBindVertexArray(m_boxVAO);
		glEnableVertexAttribArray(0);

		m_boxBuf = buf[0];
		// update mesh
		//

		glBindVertexArray(m_meshVAO);
		glCreateBuffers(2, &buf[0]);
		glNamedBufferStorage(buf[0], meshCoords.size() * sizeof(GLfloat), meshCoords.data(), 0);
		glVertexArrayVertexBuffer(m_meshVAO, 0, buf[0], 0, sizeof(GLfloat) * 2);
		glVertexArrayAttribFormat(m_meshVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_meshVAO, 0, 0);
		glEnableVertexArrayAttrib(m_meshVAO, 0);

		glNamedBufferStorage(buf[1], meshColors.size() * sizeof(GLfloat), meshColors.data(), 0);
		glVertexArrayVertexBuffer(m_meshVAO, 1, buf[1], 0, sizeof(GLfloat) * 3);
		glVertexArrayAttribFormat(m_meshVAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_meshVAO, 1, 1);
		glEnableVertexArrayAttrib(m_meshVAO, 1);

		m_meshCoordsBuf = buf[0];
		m_meshColorsBuf = buf[1];
	}
	m_boxCounts.swap(boxCounts);
	m_szMeshCount = (int)(meshCoords.size() / 8);
}

void CBoxLibIn2DRender::Zoom(short factor) noexcept
{
	float rate = 1.0f + ((float)factor / 120 / 10);
	m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(rate, rate, rate)) * m_Model;
}

void CBoxLibIn2DRender::Move(int x, int y) noexcept
{
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3((float)x / m_screenScale, (float)-y / m_screenScale, 0.0f)) * m_Model;
}

void CBoxLibIn2DRender::OnRender(float timeescape) noexcept
{
	char fps[32];
	sprintf_s(fps, "FPS:%d", (int)(1.0f / timeescape));
	m_decorateLayer.UpdateMessage(0, std::string(fps));
	m_decorateLayer.UpdateData(GetDC());

	const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	const GLfloat boxColors[][3] = {
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
	};
	glClearBufferfv(GL_COLOR, 0, color);

	if (m_szMeshCount > 0)
	{
		GLint p = 0;

		glm::mat4 m = m_projection * m_Model;

		glUseProgram(m_meshProg);
		glBindVertexArray(m_meshVAO);
		glUniformMatrix4fv(m_projectionLocationInBoxProgram, 1, GL_FALSE, glm::value_ptr(m));
		for (int i = 0; i < m_szMeshCount; i++)
		{
			glDrawArrays(GL_TRIANGLE_STRIP, p, 4);
			p += 4;
		}

		glUseProgram(m_boxProg);
		glBindVertexArray(m_boxVAO);

		glUniformMatrix4fv(m_projectionLocationInMeshProgram, 1, GL_FALSE, glm::value_ptr(m));
		p = 0;
		for (int i = 0; i < (int)m_boxCounts.size(); i++)
		{
			auto count = m_boxCounts[i];
			if (i < 6)
			{
				glUniform3f(m_uniformColorLocation, boxColors[i][0], boxColors[i][1], boxColors[i][2]);
			}
			for (int i = 0; i < count; i++)
			{
				glDrawArrays(GL_LINE_LOOP, p, 4);
				p += 4;
			}
		}

		m_decorateLayer.Render();
		glBindVertexArray(0);
	}
}

void CBoxLibIn2DRender::OnInitialize() noexcept
{
	m_boxProg = BuildProgram(std::string("glsl\\directoutuniformcolor.vert"), std::string(), std::string(), std::string(), std::string("glsl\\directoutuniformcolor.frag"));
	assert(m_boxProg != 0);
	if (m_boxProg == 0)
	{
		return;
	}

	m_uniformColorLocation = glGetUniformLocation(m_boxProg, "uniform_out_color");
	assert(m_uniformColorLocation != -1);
	if (m_uniformColorLocation == -1)
	{
		return;
	}

	m_projectionLocationInBoxProgram = glGetUniformLocation(m_boxProg, "projection");
	assert(m_projectionLocationInBoxProgram != -1);
	if (m_projectionLocationInBoxProgram == -1)
	{
		return;
	}

	m_meshProg = BuildProgram(std::string("glsl\\directout.vert"), std::string(), std::string(), std::string(), std::string("glsl\\directout.frag"));
	assert(m_meshProg != 0);
	if (m_meshProg == 0)
	{
		return;
	}

	m_projectionLocationInMeshProgram = glGetUniformLocation(m_meshProg, "projection");
	assert(m_projectionLocationInMeshProgram != -1);
	if (m_projectionLocationInMeshProgram == -1)
	{
		return;
	}

	glCreateVertexArrays(1, &m_boxVAO);
	glCreateVertexArrays(1, &m_meshVAO);

	m_decorateLayer.Initialize();
	m_decorateLayer.AppendMessage(std::string("FPS:0"));
}

void CBoxLibIn2DRender::OnResize(int width, int height) noexcept
{
	m_decorateLayer.Resize(width, height);
	m_decorateLayer.UpdateData(GetDC());

	float ratio = (float)width / height;

	if (ratio > 1)
	{
		m_projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f);
		m_screenScale = (float)height;
	}
	else
	{
		m_projection = glm::ortho(-1.0f, 1.0f, -1.0f / ratio, 1.0f / ratio);
		m_screenScale = (float)width;
	}
}

void CBoxLibIn2DRender::OnFree() noexcept
{
	m_decorateLayer.Uninitialize();

	ClearBuffers();
	ClearVectexArray();

	if (m_boxProg != 0)
	{
		glDeleteProgram(m_boxProg);
	}

	if (m_meshProg != 0)
	{
		glDeleteProgram(m_meshProg);
	}
}

void CBoxLibIn2DRender::ClearVectexArray()
{
	if (m_boxVAO != 0)
	{
		glDeleteVertexArrays(1, &m_boxVAO);
		m_boxVAO = 0;
	}

	if (m_meshVAO != 0)
	{
		glDeleteVertexArrays(1, &m_meshVAO);
		m_boxBuf = 0;
	}
}

void CBoxLibIn2DRender::ClearBuffers()
{
	if (m_boxBuf != 0)
	{
		glDeleteBuffers(1, &m_boxBuf);
		m_boxBuf = 0;
	}

	if (m_meshCoordsBuf != 0)
	{
		glDeleteBuffers(1, &m_meshCoordsBuf);
		m_boxBuf = 0;
	}

	if (m_meshColorsBuf != 0)
	{
		glDeleteBuffers(1, &m_meshColorsBuf);
		m_boxBuf = 0;
	}
}