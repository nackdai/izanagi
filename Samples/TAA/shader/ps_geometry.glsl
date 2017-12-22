#version 330
precision highp float;
precision highp int;

in float depth;
in vec3 nml;

uniform int id;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outDepth;

void main()
{
	outNormal.xyz = normalize(nml) * 0.5 + 0.5;
	outNormal.w = 1;

	outDepth = vec4(depth, 1, 1, 1);
}
