#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 vert_out_color;

uniform mat4 projection;

void main(void)
{
	gl_Position = projection * vec4(position, 1.0f);
	vert_out_color = color;
}