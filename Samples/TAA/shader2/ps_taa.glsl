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

uniform vec4 sinTime;

uniform bool enableTAA = true;
uniform bool showDiff = false;

// https://software.intel.com/en-us/node/503873
vec3 RGB_YCoCg(vec3 c)
{
	// Y = R/4 + G/2 + B/4
	// Co = R/2 - B/2
	// Cg = -R/4 + G/2 - B/4
	return vec3(
		c.x / 4.0 + c.y / 2.0 + c.z / 4.0,
		c.x / 2.0 - c.z / 2.0,
		-c.x / 4.0 + c.y / 2.0 - c.z / 4.0);
}

vec3 YCoCg_RGB(vec3 c)
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

vec4 sample_color(sampler2D tex, vec2 uv)
{
	vec4 c = texture2D(tex, uv);
	return vec4(RGB_YCoCg(c.rgb), c.a);
}

vec4 resolve_color(vec4 c)
{
	return vec4(YCoCg_RGB(c.rgb).rgb, c.a);
}

vec4 clip_aabb(vec3 aabb_min, vec3 aabb_max, vec4 p, vec4 q)
{
	// note: only clips towards aabb center (but fast!)
	vec3 center = 0.5 * (aabb_max + aabb_min);
	vec3 halfsize = 0.5 * (aabb_max - aabb_min) + 0.00000001f;

	// ’†S‚©‚ç‚Ì‘Š‘ÎˆÊ’u.
	vec4 v_clip = q - vec4(center, p.w);

	// ‘Š‘ÎˆÊ’u‚Ì³‹K‰».
	vec3 v_unit = v_clip.xyz / halfsize;

	vec3 abs_unit = abs(v_unit);
	float ma_unit = max(abs_unit.x, max(abs_unit.y, abs_unit.z));

	if (ma_unit > 1.0) {
		return vec4(center, p.w) + v_clip / ma_unit;
	}
	else {
		return q;// point inside aabb
	}
}

vec4 temporalReprojection(
	vec2 uv, vec2 velocity,
	vec2 texelSize)
{
	// Read texels.
	vec4 texel0 = sample_color(s3, uv);
	vec4 texel1 = sample_color(s4, uv - velocity);

	vec2 du = vec2(texelSize.x, 0.0);
	vec2 dv = vec2(0.0, texelSize.y);

	vec4 ctl = sample_color(s3, uv - dv - du);
	vec4 ctc = sample_color(s3, uv - dv);
	vec4 ctr = sample_color(s3, uv - dv + du);
	vec4 cml = sample_color(s3, uv - du);
	vec4 cmc = sample_color(s3, uv);
	vec4 cmr = sample_color(s3, uv + du);
	vec4 cbl = sample_color(s3, uv + dv - du);
	vec4 cbc = sample_color(s3, uv + dv);
	vec4 cbr = sample_color(s3, uv + dv + du);

	vec4 cmin = min(ctl, min(ctc, min(ctr, min(cml, min(cmc, min(cmr, min(cbl, min(cbc, cbr))))))));
	vec4 cmax = max(ctl, max(ctc, max(ctr, max(cml, max(cmc, max(cmr, max(cbl, max(cbc, cbr))))))));

	vec4 cavg = (ctl + ctc + ctr + cml + cmc + cmr + cbl + cbc + cbr) / 9.0;

	vec4 cmin5 = min(ctc, min(cml, min(cmc, min(cmr, cbc))));
	vec4 cmax5 = max(ctc, max(cml, max(cmc, max(cmr, cbc))));
	vec4 cavg5 = (ctc + cml + cmc + cmr + cbc) / 5.0;
	cmin = 0.5 * (cmin + cmin5);
	cmax = 0.5 * (cmax + cmax5);
	cavg = 0.5 * (cavg + cavg5);

	vec2 chroma_extent = vec2(0.25 * 0.5 * (cmax.r - cmin.r));
	vec2 chroma_center = texel0.gb;
	cmin.yz = chroma_center - chroma_extent;
	cmax.yz = chroma_center + chroma_extent;
	cavg.yz = chroma_center;

	// Clipping.
	texel1 = clip_aabb(cmin.xyz, cmax.xyz, clamp(cavg, cmin, cmax), texel1);

	float lum0 = texel0.r;
	float lum1 = texel1.r;

	float unbiased_diff = abs(lum0 - lum1) / max(lum0, max(lum1, 0.2));
	float unbiased_weight = 1.0 - unbiased_diff;
	float unbiased_weight_sqr = unbiased_weight * unbiased_weight;

	// TODO
	float _FeedbackMin = 0.2;
	float _FeedbackMax = 1;

	float k_feedback = mix(_FeedbackMin, _FeedbackMax, unbiased_weight_sqr);

	return mix(texel0, texel1, k_feedback);
}

float PDnrand(vec2 n)
{
	return fract(sin(dot(n.xy, vec2(12.9898f, 78.233f)))* 43758.5453f);
}
vec2 PDnrand2(vec2 n)
{
	return fract(sin(dot(n.xy, vec2(12.9898f, 78.233f)))* vec2(43758.5453f, 28001.8384f));
}
vec3 PDnrand3(vec2 n)
{
	return fract(sin(dot(n.xy, vec2(12.9898f, 78.233f)))* vec3(43758.5453f, 28001.8384f, 50849.4141f));
}
vec4 PDnrand4(vec2 n)
{
	return fract(sin(dot(n.xy, vec2(12.9898f, 78.233f)))* vec4(43758.5453f, 28001.8384f, 50849.4141f, 12996.89f));
}

float PDsrand(vec2 n)
{
	return PDnrand(n) * 2 - 1;
}
vec2 PDsrand2(vec2 n)
{
	return PDnrand2(n) * 2 - 1;
}
vec3 PDsrand3(vec2 n)
{
	return PDnrand3(n) * 2 - 1;
}
vec4 PDsrand4(vec2 n)
{
	return PDnrand4(n) * 2 - 1;
}

#define ZCMP_GT(a, b) (a > b)

vec3 find_closest_fragment_3x3(vec2 uv)
{
	vec2 dd = 1.0 / textureSize(s2, 0).xy;
	vec2 du = vec2(dd.x, 0.0);
	vec2 dv = vec2(0.0, dd.y);

	vec3 dtl = vec3(-1, -1, texture2D(s2, uv - dv - du).x);
	vec3 dtc = vec3(0, -1, texture2D(s2, uv - dv).x);
	vec3 dtr = vec3(1, -1, texture2D(s2, uv - dv + du).x);

	vec3 dml = vec3(-1, 0, texture2D(s2, uv - du).x);
	vec3 dmc = vec3(0, 0, texture2D(s2, uv).x);
	vec3 dmr = vec3(1, 0, texture2D(s2, uv + du).x);

	vec3 dbl = vec3(-1, 1, texture2D(s2, uv + dv - du).x);
	vec3 dbc = vec3(0, 1, texture2D(s2, uv + dv).x);
	vec3 dbr = vec3(1, 1, texture2D(s2, uv + dv + du).x);

	vec3 dmin = dtl;
	if (ZCMP_GT(dmin.z, dtc.z)) dmin = dtc;
	if (ZCMP_GT(dmin.z, dtr.z)) dmin = dtr;

	if (ZCMP_GT(dmin.z, dml.z)) dmin = dml;
	if (ZCMP_GT(dmin.z, dmc.z)) dmin = dmc;
	if (ZCMP_GT(dmin.z, dmr.z)) dmin = dmr;

	if (ZCMP_GT(dmin.z, dbl.z)) dmin = dbl;
	if (ZCMP_GT(dmin.z, dbc.z)) dmin = dbc;
	if (ZCMP_GT(dmin.z, dbr.z)) dmin = dbr;

	return vec3(uv + dd.xy * dmin.xy, dmin.z);
}

void main()
{
	vec2 texSize = textureSize(s3, 0).xy;

	vec2 texelSize = 1.0 / texSize;
	vec2 uv = gl_FragCoord.xy / texSize;

	if (!enableTAA) {
		outColor = texture2D(s3, uv);
		outColor.w = 1;
		return;
	}

#if 1
	vec3 c_frag = find_closest_fragment_3x3(uv);
	vec2 velocity = texture2D(s2, c_frag.xy).xy;
#else
	vec2 velocity = texture2D(s2, uv).xy;
#endif

	// temporal resolve
	vec4 color_temporal = temporalReprojection(uv, velocity, texelSize);

	// prepare outputs
	vec4 to_buffer = resolve_color(color_temporal);

	vec4 noise4 = PDsrand4(uv + sinTime.x + 0.6959174) / 510.0;
	outColor = clamp(to_buffer + noise4, vec4(0), vec4(1));
	outColor.w = 1;

	if (showDiff) {
		outColor.rgb = abs(outColor.rgb - texture2D(s3, uv).rgb);
	}
}
