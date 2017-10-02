#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec3 worldNormal;
layout(location = 1) in vec3 eyeToVtx;
layout(location = 2) in vec2 vUV;

layout(location = 0) out vec4 outColor;

uniform vec4 paintColor0;
uniform vec4 paintColor1;
uniform vec4 paintColor2;

uniform float normalScale;
uniform float glossLevel;
uniform float brightnessFactor;

uniform float MAX_REFLECTION_LOD;

uniform float normalPerturbation;
uniform float microflakePerturbationA;
uniform float microflakePerturbation;
uniform float flakeScale;
uniform vec4 flakeColor;

uniform sampler2D s0;	// flakes normal map.

#define MATH_PI          (3.14159265358979323846f)	// pi
#define MATH_PI2         (MATH_PI * 2.0f)			// 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)			// pi/2

// http://www.chrisoat.com/papers/Oat-Tatarchuk-Isidoro-Layered_Car_Paint_Shader_Print.pdf
// http://2pha.com/blog/threejs-car-paint-shader-recreating-radeon-9700-demo/

#if 0
layout(location = 2) in vec3 viewSpacePos;
layout(location = 3) in vec2 vUv;

uniform sampler2D normalMap;

// 法線マップを摂動して、TangentSpaceに変換.
vec3 perturbNormal2Arb(vec3 surf_norm)
{
	// This function taken directly from the three.js phong fragment shader.
	// http://hacksoflife.blogspot.ch/2009/11/per-pixel-tangent-space-normal-mapping.html

	vec3 q0 = dFdx(viewSpacePos.xyz);
	vec3 q1 = dFdy(viewSpacePos.xyz);
	vec2 st0 = dFdx(vUv.st);
	vec2 st1 = dFdy(vUv.st);

	vec3 S = normalize(q0 * st1.t - q1 * st0.t);
	vec3 T = normalize(-q0 * st1.s + q1 * st0.s);
	vec3 N = normalize(surf_norm);

	vec3 mapN = texture2D(normalMap, vUv).xyz * 2.0 - 1.0;

	// 摂動.
	mapN.xy = normalScale * mapN.xy;

	mat3 tsn = mat3(S, T, N);
	return normalize(tsn * mapN);
}
#endif

vec3 getOrthoVector(vec3 n)
{
	vec3 p;

	// NOTE
    // dotを計算したときにゼロになるようなベクトル.
    // k は normalize 計算用.
    
    if (abs(n.z) > 0.0) {
        float k = sqrt(n.y * n.y + n.z * n.z);
        p.x = 0;
        p.y = -n.z / k;
        p.z = n.y / k;
    }
    else {
        float k = sqrt(n.x * n.x + n.y * n.y);
        p.x = n.y / k;
        p.y = -n.x / k;
        p.z = 0;
    }
    
    return p;
}

vec3 convToNTS(vec3 nml)
{
	vec3 n = normalize(nml);
	vec3 t = getOrthoVector(n);
	vec3 b = cross(n, t);

	vec3 ret = nml.z * n + nml.x * t + nml.y * b;
	return normalize(ret);
}

void main()
{
	vec3 normal = normalize(worldNormal);

	vec3 wi = normalize(-eyeToVtx);

	vec3 flakeNormal = texture2D(s0, vUV * flakeScale).bgr;
	flakeNormal = flakeNormal * 2.0 - 1.0;	// [0, 1] -> [-1, 1]

	flakeNormal = convToNTS(flakeNormal);

	// This shader simulates two layers of micro-flakes suspended in
	// the coat of paint. To compute the surface normal for the first layer,
	// the following formula is used:
	// Np1 = ( a * Np + b * N ) / || a * Np + b * N || where a << b
	vec3 np1 = microflakePerturbationA * flakeNormal + normalPerturbation * normal;

	// To compute the surface normal for the second layer of micro-flakes, which
	// is shifted with respect to the first layer of micro-flakes, we use this formula:
	// Np2 = ( c * Np + d * N ) / || c * Np + d * N || where c == d
	vec3 np2 = microflakePerturbation * (flakeNormal + normal);

	// Compute modified Fresnel term for reflections from the first layer of
	// microflakes. First transform perturbed surface normal for that layer into
	// world space and then compute dot product of that normal with the view vector:
	float fresnel1 = clamp(dot(np1, wi), 0.0, 1.0);

	// Compute modified Fresnel term for reflections from the second layer of
	// microflakes. Again, transform perturbed surface normal for that layer into
	// world space and then compute dot product of that normal with the view vector:
	float fresnel2 = clamp(dot(np2, wi), 0.0, 1.0);

	float fresnel1Sq = fresnel1 * fresnel1;

	vec4 paintColor = fresnel1 * flakeColor
		+ fresnel1Sq * flakeColor
		+ fresnel1Sq * fresnel1Sq * flakeColor
		+ pow(fresnel2, 32.0) * flakeColor;

	outColor = vec4(paintColor.rgb, 1.0);
}
