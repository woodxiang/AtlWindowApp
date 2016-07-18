#pragma once
#include "GLRender.h"
class CD2DemoRender :
	public CGLRender
{
public:

	CD2DemoRender()
	{
	}

	virtual ~CD2DemoRender()
	{
	}

	void Display()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(0, 1);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-1, -1);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2i(1, -1);
		glEnd();

		glPointSize(50.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.7f, -0.5f, 0.0f);
		glVertex3f(0.6f, -0.7f, 0.0f);
		glVertex3f(0.6f, -0.8f, 0.0f);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(-0.7f, 0.5f, 0.0f);
		glEnd();

		glFlush();
	}
};

