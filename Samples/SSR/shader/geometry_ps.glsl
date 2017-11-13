#version 450
precision highp float;
precision highp int;

in vec4 varColor;
in vec3 varNormal;
in float depth;

uniform bool isWriteColor;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outDepth;

void main()
{
	if (isWriteColor) {
		outColor = varColor;
	}
    outDepth = vec4(depth, 1, 1, 1);
}
