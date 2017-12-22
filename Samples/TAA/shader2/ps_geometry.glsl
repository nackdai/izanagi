#version 450
precision highp float;
precision highp int;

in float depth;
in vec3 nml;
in vec4 curCSPos;
in vec4 prevCSPos;

uniform vec4 invScreen;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outDepth;
layout(location = 2) out vec4 outMotion;

void main()
{
	vec2 curNDC = curCSPos.xy / curCSPos.w;
	vec2 prevNDC = prevCSPos.xy / prevCSPos.w;

	// [-1, 1] -> [-0.5, 0.5]
	// ”ÍˆÍ‚ð 2 ‚©‚ç 1 ‚ÉŽû‚ß‚é.
	vec2 motion = 0.5 * (curNDC - prevNDC);

	outMotion.xy = motion;
	outMotion.z = 0;
	outMotion.w = 1;

	outNormal.xyz = normalize(nml) * 0.5 + 0.5;
	outNormal.w = 1;

	outDepth = vec4(depth, 1, 1, 1);
}
