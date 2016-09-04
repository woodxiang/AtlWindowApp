#pragma once
#include "GLRender.h"
class CChangeBackgroudRender :
	public CGLRender
{
public:

	CChangeBackgroudRender()
	{
	}

	~CChangeBackgroudRender()
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
		glDrawArrays(GL_POINTS, 0,3);
	}

	void OnInitialize()
	{
		m_renderProgram = BuildProgream(std::string("glsl\\triangle.vert"), std::string("glsl\\point.frag"));

		glCreateVertexArrays(1, &m_renderVao);
		glBindVertexArray(m_renderVao);

	}

private:
	GLuint m_renderProgram;
	GLuint m_renderVao;

};

