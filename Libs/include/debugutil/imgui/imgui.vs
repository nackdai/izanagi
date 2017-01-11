#ifndef GLSL_EXTERNAL_INCLUDE
#define GLSL_EXTERNAL_INCLUDE(...) __VA_ARGS__
#endif

GLSL_EXTERNAL_INCLUDE(
#version 330\n

precision highp float;
precision highp int;

uniform mat4 ProjMtx;

in vec2 position;
in vec2 texcoord_0;
in vec4 color_0;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
	Frag_UV = texcoord_0;
	Frag_Color = color_0;
	gl_Position = ProjMtx * vec4(position.xy,0,1);
}
)