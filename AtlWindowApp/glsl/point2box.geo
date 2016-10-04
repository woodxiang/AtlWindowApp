#version 450 core

layout(points) in;
layout(line_strip, max_vertices=5) out;

uniform float boxSize;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * gl_in[0].gl_Position;
	EmitVertex();
	
	gl_Position = projection *(gl_in[0].gl_Position + vec4(boxSize, 0.0, 0.0, 0.0));
	EmitVertex();

	gl_Position = projection *(gl_in[0].gl_Position + vec4(boxSize, -boxSize, 0.0, 0.0));
	EmitVertex();

	gl_Position = projection *(gl_in[0].gl_Position + vec4(0.0, -boxSize, 0.0, 0.0));
	EmitVertex();

	gl_Position = projection * gl_in[0].gl_Position;
	EmitVertex();

	EndPrimitive();
}