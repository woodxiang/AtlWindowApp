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
		const GLfloat color[] = {
			(float)sin(timeescape) * 0.5f + 0.5f,
			(float)cos(timeescape) * 0.5f + 0.5f,
			0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glPointSize(20);
		glUseProgram(m_renderProgram);
		glDrawArrays(GL_TRIANGLES, 0,3);
	}

	void OnInitialize()
	{
		m_renderProgram = BuildProgram(
			std::string("glsl\\triangle.vert"), 
			std::string(),
			std::string(),
			std::string(),
			std::string("glsl\\triangle.frag"));

		glCreateVertexArrays(1, &m_renderVao);
		glBindVertexArray(m_renderVao);

	}

private:
	GLuint m_renderProgram;
	GLuint m_renderVao;

};

