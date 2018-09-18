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
// https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-121
// https://github.com/septag/darkhammer/blob/master/data/shaders/hlsl/upsample-bilateral.ps.hlsl

const vec4 bilateralWeight[4] = vec4[4](
	vec4(9.0 / 16.0, 3.0 / 16.0, 3.0 / 16.0, 1.0 / 16.0),
	vec4(3.0 / 16.0, 9.0 / 16.0, 1.0 / 16.0, 3.0 / 16.0),
	vec4(3.0 / 16.0, 1.0 / 16.0, 9.0 / 16.0, 3.0 / 16.0),
	vec4(1.0 / 16.0, 3.0 / 16.0, 3.0 / 16.0, 9.0 / 16.0)
);

const float g_epsilon = 0.0001f;

vec4 fetch(sampler2D tex, vec2 base, vec2 offset)
{
	vec2 uv = base + invScreen.xy * offset;
	return texture2D(tex, uv);
}

void main()
{
	vec2 uv = gl_FragCoord.xy * invScreen.xy;
	ivec2 pos = ivec2(gl_FragCoord.xy);

	// NOTE
	// Origin is lower-left.
	// +Y
	// +-----+-----+
	// |(0,1)|(1,1)|
	// +-----+-----+
	// |(0,0)|(1,0)|
	// +-----+-----+->+X
	// 
	// +Y
	// +---+---+
	// | 2 | 3 |
	// +---+---+
	// | 0 | 1 |
	// +---+---+->+X

	// NOTE
	// In below, * is origin.
	// Below is sample pattern.
	//         +Y
	//     +---+---+---+---+
	//     | 2x| 2x| 3x| 3x|
	//     +---+---+---+---+
	//     | 2x| 2 | 3 | 3x|
	//     +---+---+---+---+
	//     | 0x| 0 | 1 | 1x|
	// -X<-+---*---+---+---+->+X
	//     | 0x| 0x| 1x| 1x|
	//     +---+---+---+---+
	//         -Y

	int idx = (pos.y & 0x01) * 2 + (pos.x & 0x01);

	vec4 lowResNmlDepth[4];
	vec4 lowResClr[4];

	switch (idx) {
	case 0:
		lowResNmlDepth[0] = fetch(s1, uv, vec2( 0,  0));
		lowResNmlDepth[1] = fetch(s1, uv, vec2(-1,  0));
		lowResNmlDepth[2] = fetch(s1, uv, vec2( 0, -1));
		lowResNmlDepth[3] = fetch(s1, uv, vec2(-1, -1));
		lowResClr[0] = fetch(s0, uv, vec2( 0,  0));
		lowResClr[1] = fetch(s0, uv, vec2(-1,  0));
		lowResClr[2] = fetch(s0, uv, vec2( 0, -1));
		lowResClr[3] = fetch(s0, uv, vec2(-1, -1));
		break;
	case 1:
		lowResNmlDepth[0] = fetch(s1, uv, vec2(0, -1));
		lowResNmlDepth[1] = fetch(s1, uv, vec2(0,  0));
		lowResNmlDepth[2] = fetch(s1, uv, vec2(1,  0));
		lowResNmlDepth[3] = fetch(s1, uv, vec2(1, -1));
		lowResClr[0] = fetch(s0, uv, vec2(0, -1));
		lowResClr[1] = fetch(s0, uv, vec2(0,  0));
		lowResClr[2] = fetch(s0, uv, vec2(1,  0));
		lowResClr[3] = fetch(s0, uv, vec2(1, -1));
		break;
	case 2:
		lowResNmlDepth[0] = fetch(s1, uv, vec2(-1, 0));
		lowResNmlDepth[1] = fetch(s1, uv, vec2( 0, 1));
		lowResNmlDepth[2] = fetch(s1, uv, vec2( 0, 0));
		lowResNmlDepth[3] = fetch(s1, uv, vec2(-1, 1));
		lowResClr[0] = fetch(s0, uv, vec2(-1, 0));
		lowResClr[1] = fetch(s0, uv, vec2( 0, 1));
		lowResClr[2] = fetch(s0, uv, vec2( 0, 0));
		lowResClr[3] = fetch(s0, uv, vec2(-1, 1));
		break;
	case 3:
		lowResNmlDepth[0] = fetch(s1, uv, vec2(0, 1));
		lowResNmlDepth[1] = fetch(s1, uv, vec2(1, 0));
		lowResNmlDepth[2] = fetch(s1, uv, vec2(0, 0));
		lowResNmlDepth[3] = fetch(s1, uv, vec2(1, 1));
		lowResClr[0] = fetch(s0, uv, vec2(0, 1));
		lowResClr[1] = fetch(s0, uv, vec2(1, 0));
		lowResClr[2] = fetch(s0, uv, vec2(0, 0));
		lowResClr[3] = fetch(s0, uv, vec2(1, 1));
		break;
	}

	vec4 hiResNmlDepth = texture2D(s2, uv);

	vec4 sum = vec4(0);
	float weight = 0.0001f;

	for (int i = 0; i < 4; i++) {
		float nmlWeight = clamp(pow(dot(hiResNmlDepth.xyz, lowResNmlDepth[i].xyz), 32), 0, 1);
		float depthWeight = clamp(1.0 / (g_epsilon + abs(hiResNmlDepth.w - lowResNmlDepth[i].w)), 0, 1);

		float w = nmlWeight * depthWeight * bilateralWeight[idx][i];
		sum += lowResClr[i] * w;
		weight += w;
	}

	sum /= weight;

	if (hiResNmlDepth.w <= 0.0f) {
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
