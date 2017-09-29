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

uniform sampler2D s0;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

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

void main()
{
	vec3 normal = normalize(worldNormal);

#if 0
	normal = perturbNormal2Arb(normal);
#else
	normal = normalize(normalScale * normal);
#endif

	vec3 wi = normalize(-eyeToVtx);

	// Compute reflection vector resulted from the clear coat of paint on the metallic
	// surface:
	float NdotV = clamp(dot(normal, wi), 0.0, 1.0);
	vec3 reflection = 2 * worldNormal * NdotV - wi;

	// [-pi/2, pi/2] -> [0, pi]
	float theta = asin(reflection.y) + MATH_PI1_2;

	// [-pi, pi] -> [0, 2pi]
	float phi = atan(reflection.x, reflection.z) + MATH_PI;

	// Normalize [0, 1]
	vec2 uv = vec2(phi / MATH_PI2, 1 - theta / MATH_PI);

	vec3 envMap = textureLod(s0, uv, glossLevel * MAX_REFLECTION_LOD).bgr;

	// Premultiply by alpha:
	// Omit, there is no alpha in env map.
	//envMap.rgb = envMap.rgb * envMap.a;

	// Brighten the environment map sampling result:
	envMap.rgb *= brightnessFactor;

	// Combine result of environment map reflection with the paint color:
	float envContribution = 1.0 - 0.5 * NdotV;

	envMap.rgb *= envContribution;

	outColor = vec4(envMap.rgb, 1.0);
}
