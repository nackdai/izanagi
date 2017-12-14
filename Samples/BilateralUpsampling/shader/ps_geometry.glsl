#version 330
precision highp float;
precision highp int;

in float depth;
in vec3 nml;

uniform int id;

layout(location = 0) out vec4 oScreen;

void main()
{
	oScreen.xyz = normalize(nml);
	oScreen.w = depth;
}
