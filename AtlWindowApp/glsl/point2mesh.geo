#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vec3 vert_out_color[];
out vec3 geo_out_color;

uniform float boxSize;
uniform mat4 projection;

void main(void)
{
	geo_out_color = vert_out_color[0];

	gl_Position = projection* (gl_in[0].gl_Position + vec4(0.0, -boxSize, 0.0, 0.0));
	EmitVertex();
	
	gl_Position = projection* (gl_in[0].gl_Position + vec4(boxSize, -boxSize, 0.0, 0.0));
	EmitVertex();

	gl_Position = projection* (gl_in[0].gl_Position + vec4(0, 0, 0.0, 0.0));
	EmitVertex();

	gl_Position = projection* (gl_in[0].gl_Position + vec4(boxSize, 0, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();
}