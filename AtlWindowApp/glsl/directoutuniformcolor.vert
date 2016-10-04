#version 450 core
layout (location = 0) in vec3 position;

uniform mat4 projection;

void main(void)
{
	gl_Position = projection * vec4(position, 1.0f);
}