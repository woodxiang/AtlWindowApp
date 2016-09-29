#version 450 core

out vec4 frag_out_color;

uniform vec3 uniform_out_color;

void main()
{
	frag_out_color = vec4(uniform_out_color, 1.0);
}