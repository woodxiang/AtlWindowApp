#version 450 core

out vec4 color;

void main(void)
{
	const vec4 colors[3] = vec4[3]{
		vec4(0.0, 0.8, 1.0, 1.0),
		vec4(0.0, 0.8, 1.0, 1.0),
		vec4(0.0, 0.8, 1.0, 1.0)
	};

	color = colors[gl_VertexID];
}