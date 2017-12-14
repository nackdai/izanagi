#version 330
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // color
uniform sampler2D s1;	// normal/depth
uniform sampler2D s2;	// normal/depth hi-resolution

uniform vec4 invScreen;

// NOTE
// http://d.hatena.ne.jp/hanecci/20131030/p1
// https://github.com/septag/darkhammer/blob/master/data/shaders/hlsl/upsample-bilateral.ps.hlsl

const vec2 offsets[4] = vec2[4](
	vec2( 0.0f,  1.0f),
	vec2( 1.0f,  0.0f),
	vec2( 0.0f, -1.0f),
	vec2(-1.0f, 0.0f)
);

const float g_epsilon = 0.0001f;

void main()
{
	vec2 uv = gl_FragCoord.xy * invScreen.xy;

	vec2 coords[4];

	for (int i = 0; i < 4; i++) {
		coords[i] = uv + invScreen.xy * offsets[i];
	}

	// normal weight.

	float nmlWeight[4];
	
	vec3 nmlHires = texture2D(s2, uv).xyz;

	for (int i = 0; i < 4; i++) {
		vec3 nmlCoarse = texture2D(s1, coords[i]).xyz;
		nmlWeight[i] = pow(abs(dot(nmlCoarse, nmlHires)), 32);
	}

	// depth weight.

	float depthWeight[4];

	float depthHires = texture2D(s2, uv).w;

	for (int i = 0; i < 4; i++) {
		float depthCoars = texture2D(s1, coords[i]).w;
		depthWeight[i] = 1.0f / (g_epsilon + abs(depthHires - depthCoars));
	}

	vec4 sum = vec4(0);
	float weight = 0.0001f;

	for (int i = 0; i < 4; i++) {
		float w = nmlWeight[i] * depthWeight[i];
		sum += texture2D(s0, coords[i]) * w;
		weight += w;
	}

	sum /= weight;

	if (depthHires <= 0.0f) {
		// Background.
		outColor = texture2D(s0, uv);
		outColor.a = 1.0f;
		return;
	}
	else {
		outColor = sum;
		outColor.a = 1.0;
	}
}
