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

uniform float blurSize = 0.2;

uniform bool enableTAA = true;
uniform bool showDiff = false;

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

// http://graphicrants.blogspot.jp/2013/12/tone-mapping.html
vec3 map(vec3 clr)
{
	float lum = RGB2YCoCg(clr).x;
	return clr / (1 + lum);
}

vec3 unmap(vec3 clr)
{
	float lum = RGB2YCoCg(clr).x;
	return clr / (1 - lum);
}

// http://twvideo01.ubm-us.net/o1/vault/gdc2016/Presentations/Pedersen_LasseJonFuglsang_TemporalReprojectionAntiAliasing.pdf
vec3 clipAABB(
	vec3 aabb_min,
	vec3 aabb_max,
	vec3 q)
{
	vec3 center = 0.5 * (aabb_max + aabb_min);

	vec3 halfsize = 0.5 * (aabb_max - aabb_min) + 0.00000001f;

	// 中心からの相対位置.
	vec3 clip = q - center;

	// 相対位置の正規化.
	vec3 unit = clip / halfsize;

	vec3 abs_unit = abs(unit);

	float ma_unit = max(abs_unit.x, max(abs_unit.y, abs_unit.z));

	if (ma_unit > 1.0) {
		// クリップ位置.
		return center + clip / ma_unit;
	}
	else {
		// point inside aabb
		return q;
	}
}

vec4 sampleColor(sampler2D s, vec2 uv)
{
	vec4 clr = texture2D(s, uv);

	clr.xyz = map(clr.xyz);

	clr.xyz = RGB2YCoCg(clr.xyz);

	return clr;
}

void main()
{
	// http://twvideo01.ubm-us.net/o1/vault/gdc2016/Presentations/Pedersen_LasseJonFuglsang_TemporalReprojectionAntiAliasing.pdf
	// https://github.com/playdeadgames/temporal/blob/master/Assets/Shaders/TemporalReprojection.shader
	// http://t-pot.com/program/152_TAA/
	// https://github.com/t-pot/TAA

	ivec2 texsize = textureSize(s3, 0);

	vec2 invScr = 1.0 / texsize.xy;;

	vec2 uv = gl_FragCoord.xy * invScr;

	if (!enableTAA) {
		outColor = texture2D(s3, uv);
		outColor.a = 1;
		return;
	}

	float center_depth = texture2D(s1, uv).x;
	if (center_depth <= 0) {
		outColor = texture2D(s3, uv);
		outColor.a = 1;
		return;
	}

	vec2 du = vec2(invScr.x, 0.0);
	vec2 dv = vec2(0.0, invScr.y);

	vec4 ctl = sampleColor(s3, uv - dv - du);
	vec4 ctc = sampleColor(s3, uv - dv);
	vec4 ctr = sampleColor(s3, uv - dv + du);
	vec4 cml = sampleColor(s3, uv - du);
	vec4 cmc = sampleColor(s3, uv);
	vec4 cmr = sampleColor(s3, uv + du);
	vec4 cbl = sampleColor(s3, uv + dv - du);
	vec4 cbc = sampleColor(s3, uv + dv);
	vec4 cbr = sampleColor(s3, uv + dv + du);

	vec4 cmin = min(ctl, min(ctc, min(ctr, min(cml, min(cmc, min(cmr, min(cbl, min(cbc, cbr))))))));
	vec4 cmax = max(ctl, max(ctc, max(ctr, max(cml, max(cmc, max(cmr, max(cbl, max(cbc, cbr))))))));

	vec4 cavg = (ctl + ctc + ctr + cml + cmc + cmr + cbl + cbc + cbr) / 9.0;

	vec4 cmin5 = min(ctc, min(cml, min(cmc, min(cmr, cbc))));
	vec4 cmax5 = max(ctc, max(cml, max(cmc, max(cmr, cbc))));
	vec4 cavg5 = (ctc + cml + cmc + cmr + cbc) / 5.0;
	cmin = 0.5 * (cmin + cmin5);
	cmax = 0.5 * (cmax + cmax5);
	cavg = 0.5 * (cavg + cavg5);

	vec4 center_color = sampleColor(s3, uv);

	vec4 neighbor_sum = vec4(0, 0, 0, 0);
	float weight = 0;

	const float cbcr_threshhold = 0.32;
	const float maxLen = 2.0;

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			vec2 offset = vec2(x, y);

#if 0
			float len = length(offset);
			float W = exp(-2.29 * len * len);

			vec4 neighbor = sampleColor(s4, uv + offset * invScr);
#else
			offset *= invScr;

			float depth = texture2D(s1, uv + offset).x;

			if (depth < 0) {
				continue;
			}

			vec2 velocity = texture2D(s2, uv + offset).xy;

			float len2 = dot(velocity, velocity) + 1e-6f;
			velocity /= len2;

			velocity *= min(len2, maxLen);

			vec4 neighbor = sampleColor(s4, uv + velocity);

			// NOTE
			// High-Quality Temporal Supersampling.
			// http://advances.realtimerendering.com/s2014/epic/TemporalAA.pptx
			// p.23
			float len = length(velocity);
			float W = exp(-2.29 * len * len);

			W *= 1 - clamp(len2 / maxLen, 0.0, 1.0);
#endif

			neighbor.xyz = clipAABB(cmin.xyz, cmax.xyz, neighbor.xyz);

			vec3 color_diff = abs(neighbor.xyz - center_color.xyz);

			vec3 ycc = color_diff.xyz;

			float cbcr_len = length(color_diff.yz);

			// 色相成分が大きく異なる時、閾値に収まる範囲に色を補正して合成
			if (cbcr_threshhold < cbcr_len) {
				ycc = (cbcr_threshhold / cbcr_len) * ycc;
				neighbor.rgb = center_color.rgb + ycc;
			}

			neighbor_sum.xyz += neighbor.xyz * W;
			weight += W;
		}
	}

	if (weight > 0) {
		neighbor_sum /= weight;
		weight /= 9;
		neighbor_sum.xyz = mix(center_color.xyz, neighbor_sum.xyz, weight);
		neighbor_sum.xyz = YCoCg2RGB(neighbor_sum.xyz);

		neighbor_sum.xyz = unmap(neighbor_sum.xyz);

		outColor = neighbor_sum;
		outColor.a = 1;
	}
	else {
		outColor = center_color;
	}

	if (showDiff) {
		outColor.xyz = abs(outColor.xyz - texture2D(s3, uv).xyz);
		outColor.a = 1;
	}
}
