#pragma once

#include "GLRender.h"
class CD2DemoRender :
	public CGLRender
{
public:

	CD2DemoRender()
	{
	}

	~CD2DemoRender()
	{
	}

protected:
	void  OnRender(float timeescape)
	{
		static float t = 0;
		t += timeescape;
		const GLfloat color[] = {
			(float)sin(t) * 0.5f + 0.5f,
			(float)cos(t) * 0.5f + 0.5f,
			0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glPointSize(20);
		glUseProgram(m_renderProgram);
		glDrawArrays(GL_POINTS, 0,3);
	}

	void OnInitialize()
	{
		m_renderProgram = BuildProgram(
			std::string("glsl\\triangle.vert"), 
			std::string(""),
			std::string(""),
			std::string("glsl\\triangle.geo"),
			std::string("glsl\\triangle.frag"));

		if (m_renderProgram == 0)
		{
			return;
		}

		glCreateVertexArrays(1, &m_renderVao);
		glBindVertexArray(m_renderVao);
	}

private:
	GLuint m_renderProgram;
	GLuint m_renderVao;

};

