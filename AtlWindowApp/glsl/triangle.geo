#version 450 core

layout (points) in;
layout (points ) out;

void main(void)
{
	int i;
//	gl_Position = vec4(gl_in[0].gl_Position.x-0.1, gl_in[0].gl_Position.y-1.0, gl_in[0].gl_Position.z, 1.0);
//	EmitVertex();

//	gl_Position = vec4(gl_in[0].gl_Position.x-0.1, gl_in[0].gl_Position.y+1.0, gl_in[0].gl_Position.z, 1.0);
//	EmitVertex();

//	gl_Position = vec4(gl_in[0].gl_Position.x+0.1, gl_in[0].gl_Position.y-1.0, gl_in[0].gl_Position.z, 1.0);
//	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
}