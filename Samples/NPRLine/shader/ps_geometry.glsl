#version 450
precision highp float;
precision highp int;

in vec4 clr;
in float depth;
in vec3 nml;

uniform int id;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oNmlDepth;
layout(location = 2) out vec4 oGeomId;

void main()
{
	oColor = clr;

	oNmlDepth.xyz = normalize(nml);
	oNmlDepth.w = depth;

	oGeomId.r = id;
}
