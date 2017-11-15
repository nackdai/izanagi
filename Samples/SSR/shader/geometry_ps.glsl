#version 450
precision highp float;
precision highp int;

in vec4 varColor;
in vec3 varNormal;
in float depth;

uniform bool isSSRCaster;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outDepth;

void main()
{
	if (isSSRCaster) {
		outColor = varColor;
		outDepth = vec4(depth, 1, 1, 1);
	}
	else {
		// 自分自身にヒットしてしまうのを避けるため.
		// 目印として、深度値にマイナスを入れる.
		outColor = vec4(0);
		outDepth = vec4(-depth, 1, 1, 1);
	}
}
