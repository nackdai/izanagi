#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // normal
uniform sampler2D s1;	// depth
uniform sampler2D s2;	// motion
uniform sampler2D s3;	// current color
uniform sampler2D s4;	// previous color

uniform float colorBoxSigma = 1.0;
uniform float lerpRatio = 0.1;
uniform bool enableTAA = true;
uniform bool showDiff = false;

ivec2 offsets[8] = ivec2[8](
	ivec2(-1, -1),
	ivec2(-1, 1),
	ivec2(1, -1),
	ivec2(1, 1),
	ivec2(1, 0),
	ivec2(0, -1),
	ivec2(0, 1),
	ivec2(-1, 0)
);

// https://software.intel.com/en-us/node/503873
vec3 RGB2YCoCg(vec3 c)
{
	// Y = R/4 + G/2 + B/4
	// Co = R/2 - B/2
	// Cg = -R/4 + G/2 - B/4
	return vec3(
		c.x / 4.0 + c.y / 2.0 + c.z / 4.0,
		c.x / 2.0 - c.z / 2.0,
		-c.x / 4.0 + c.y / 2.0 - c.z / 4.0);
}

vec3 YCoCg2RGB(vec3 c)
{
	// R = Y + Co - Cg
	// G = Y + Cg
	// B = Y - Co - Cg
	return clamp(
		vec3(
			c.x + c.y - c.z,
			c.x + c.z,
			c.x - c.y - c.z),
		0, 1);
}

// Catmull-Rom filtering code from http://vec3.ca/bicubic-filtering-in-fewer-taps/
vec3 bicubicSampleCatmullRom(
	sampler2D tex,
	vec2 samplePos, 
	vec2 texDim)
{
	vec2 invTextureSize = 1.0 / texDim;
	vec2 tc = floor(samplePos - 0.5) + 0.5;
	vec2 f = samplePos - tc;
	vec2 f2 = f * f;
	vec2 f3 = f2 * f;

	vec2 w0 = f2 - 0.5 * (f3 + f);
	vec2 w1 = 1.5 * f3 - 2.5 * f2 + 1;
	vec2 w3 = 0.5 * (f3 - f2);
	vec2 w2 = 1 - w0 - w1 - w3;

	vec2 w12 = w1 + w2;

	vec2 tc0 = (tc - 1) * invTextureSize;
	vec2 tc12 = (tc + w2 / w12) * invTextureSize;
	vec2 tc3 = (tc + 2) * invTextureSize;

	vec3 result =
		texture2D(tex, vec2(tc0.x, tc0.y)).rgb  * (w0.x  * w0.y) +
		texture2D(tex, vec2(tc0.x, tc12.y)).rgb * (w0.x  * w12.y) +
		texture2D(tex, vec2(tc0.x, tc3.y)).rgb  * (w0.x  * w3.y) +
		texture2D(tex, vec2(tc12.x, tc0.y)).rgb  * (w12.x * w0.y) +
		texture2D(tex, vec2(tc12.x, tc12.y)).rgb * (w12.x * w12.y) +
		texture2D(tex, vec2(tc12.x, tc3.y)).rgb  * (w12.x * w3.y) +
		texture2D(tex, vec2(tc3.x, tc0.y)).rgb  * (w3.x  * w0.y) +
		texture2D(tex, vec2(tc3.x, tc12.y)).rgb * (w3.x  * w12.y) +
		texture2D(tex, vec2(tc3.x, tc3.y)).rgb  * (w3.x  * w3.y);

	return result;
}

void main()
{
	vec2 texSize = textureSize(s3, 0).xy;

	ivec2 pos = ivec2(gl_FragCoord.xy);
	vec2 uv = gl_FragCoord.xy / texSize;

	if (!enableTAA) {
		outColor = texelFetch(s3, pos, 0);
		outColor.w = 1;
		return;
	}

	// Fetch the current pixel color and compute the color bounding box
	// Details here: http://www.gdcvault.com/play/1023521/From-the-Lab-Bench-Real
	// and here: http://cwyman.org/papers/siga16_gazeTrackedFoveatedRendering.pdf

	vec3 color = texelFetch(s3, pos, 0).rgb;
	color = RGB2YCoCg(color);

	vec3 colorAvg = color;
	vec3 colorVar = color * color;

	for (int i = 0; i < 8; i++) {
		vec3 c = texelFetch(s3, pos + offsets[i], 0).rgb;
		c = RGB2YCoCg(c);

		colorAvg += c;
		colorVar += c * c;
	}

	colorAvg /= 9;
	colorVar /= 9;

	vec3 sigma = sqrt(max(vec3(0), colorVar - colorAvg * colorAvg));
	vec3 colorMin = colorAvg - colorBoxSigma * sigma;
	vec3 colorMax = colorAvg + colorBoxSigma * sigma;

	// Find the longest motion vector
	vec2 motion = texelFetch(s2, pos, 0).xy;
	for (int i = 0; i < 8; i++) {
		vec2 m = texelFetch(s2, pos + offsets[i], 0).xy;
		motion = dot(m, m) > dot(motion, motion) ? m : motion;
	}

	// Use motion vector to fetch previous frame color (history)
	vec3 history = bicubicSampleCatmullRom(s4, (uv + motion) * texSize, texSize);

	history = RGB2YCoCg(history);

	history = clamp(history, colorMin, colorMax);
	vec3 result = YCoCg2RGB(mix(history, color, lerpRatio));

	outColor.xyz = showDiff ? abs(result - color) : result;
	outColor.w = 1;
}
