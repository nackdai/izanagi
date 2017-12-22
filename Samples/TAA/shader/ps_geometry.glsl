#version 330
precision highp float;
precision highp int;

in float depth;
in vec3 nml;
in vec4 prevCSPos;

uniform vec4 invScreen;

layout(location = 0) out vec4 outNormal;
layout(location = 1) out vec4 outDepth;
layout(location = 2) out vec4 outMotion;

void main()
{
	vec2 curScreenPos = gl_FragCoord.xy * invScreen.xy;

	vec2 prevScreenPos = prevCSPos.xy / prevCSPos.w;
	prevScreenPos *= vec2(0.5, 0.5) + vec2(0.5);

	// [-1, 1] -> [0, 1]
	prevScreenPos = prevScreenPos * vec2(0.5) + vec2(0.5);

	vec2 motion = prevScreenPos - curScreenPos;

	outMotion.xy = motion;
	outMotion.z = 0;
	outMotion.w = 1;

	outNormal.xyz = normalize(nml) * 0.5 + 0.5;
	outNormal.w = 1;

	outDepth = vec4(depth, 1, 1, 1);
}
