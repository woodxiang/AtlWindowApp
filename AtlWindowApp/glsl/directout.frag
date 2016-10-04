#version 450 core

in vec3 geo_out_color;
out vec4 frag_out_color;

void main()
{
	frag_out_color = vec4(geo_out_color, 1.0);
}