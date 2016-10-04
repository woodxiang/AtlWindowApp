#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 vert_out_color;

void main(void)
{
	gl_Position = vec4(position, 1.0f);
	vert_out_color = color;
}