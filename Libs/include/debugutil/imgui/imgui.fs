#ifndef GLSL_EXTERNAL_INCLUDE
#define GLSL_EXTERNAL_INCLUDE(...) __VA_ARGS__
#endif

GLSL_EXTERNAL_INCLUDE(
#version 330\n

precision highp float;
precision highp int;

uniform sampler2D s0;

in vec2 Frag_UV;
in vec4 Frag_Color;

out vec4 Out_Color;

void main()
{
    Out_Color = Frag_Color * texture(s0, Frag_UV.st);
}
)
