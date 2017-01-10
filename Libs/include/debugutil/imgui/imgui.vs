#ifndef GLSL_EXTERNAL_INCLUDE
#define GLSL_EXTERNAL_INCLUDE(...) __VA_ARGS__
#endif

GLSL_EXTERNAL_INCLUDE(
#version 330

uniform mat4 ProjMtx;

in vec2 position;
in vec2 texcoord_0;
in vec4 color_0;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
	Frag_UV = UV;
	Frag_Color = Color;
	gl_Position = ProjMtx * vec4(Position.xy,0,1);
}
)