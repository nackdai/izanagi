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

	// Compute modified Fresnel term for reflections from the first layer of microflakes. 
	float NdotV = clamp(dot(wi, normal), 0, 1);
	float NdotVSq = NdotV * NdotV;

	vec4 paintColor = NdotV * paintColor0
		+ NdotVSq * paintColor1
		+ NdotVSq * NdotVSq * paintColor2;

	outColor = vec4(paintColor.xyz, 1.0);
}
