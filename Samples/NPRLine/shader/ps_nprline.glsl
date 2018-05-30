#version 450
precision highp float;
precision highp int;

layout(location = 0) out vec4 outColor;


uniform sampler2D s0;   // color.
uniform sampler2D s1;	// normal/depth.
uniform sampler2D s2;	// object id.

in vec4 varPos;

uniform vec4 invScreen;

// NOTE
// N = 2
// 8 * N(=2) + 8 * 1(=N-1) = 16 + 8
const int M = 24;
uniform vec4 taps[M];

uniform vec4 creaseTaps[4];

uniform float radius;
uniform float depthThreshold;
uniform float normalThreshold;

void main()
{
	int centerGeomId = int(texelFetch(s2, ivec2(gl_FragCoord.xy), 0).r);

	vec4 nmldepth = texelFetch(s1, ivec2(gl_FragCoord.xy), 0);
	vec3 centerNml = nmldepth.xyz;
	float centerDepth = nmldepth.w;

	vec2 centerUV = gl_FragCoord.xy * invScreen.xy;

	if (centerGeomId == 0) {
		outColor = texture2D(s0, centerUV);
		return;
	}

	int m = 0;
	int d = 0;

	for (int i = 0; i < 24; i++) {
		vec2 uv = gl_FragCoord.xy + radius * taps[i].xy;
		uv *= invScreen.xy;

		int geomId = int(texture2D(s2, uv).r);
		float depth = texture2D(s1, uv).w;

		if (centerGeomId != geomId) {
			m++;
		}

		float t = abs(depth - centerDepth);
		if (t > depthThreshold) {
			d++;
		}
	}

	float edgeStrength = 0.0f;

	if (m > 0) {
		edgeStrength = clamp(1.0f - abs(m - M / 2) / (M / 2), 0.0f, 1.0f);
	}
	else {
		for (int i = 0; i < 4; i++) {
			vec2 uv = gl_FragCoord.xy + radius * creaseTaps[i].xy;

			nmldepth = texture2D(s1, uv * invScreen.xy);
			vec3 nml = nmldepth.xyz;

			float n = 1.0f - clamp(dot(centerNml, nml), 0.0f, 1.0f);

			if (n > normalThreshold) {
				edgeStrength = 1.0f;
			}
		}

		if (edgeStrength == 0.0f) {
			edgeStrength = clamp(1.0f - abs(d - M / 2) / (M / 2), 0.0f, 1.0f);
		}
	}

	vec4 clr = texture2D(s0, centerUV);

	outColor = clr * vec4(1.0f - edgeStrength);
	outColor.w = 1;
}
