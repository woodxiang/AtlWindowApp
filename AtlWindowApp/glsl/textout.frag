#version 440 core
layout (location = 0) out vec4 o_color;
uniform sampler2D s;
void main(void)
{    
    o_color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);	
}