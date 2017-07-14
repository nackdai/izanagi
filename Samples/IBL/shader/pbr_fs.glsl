#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 albedo;
layout(location = 1) in vec3 reflection;
layout(location = 2) in vec3 wi;
layout(location = 3) in vec3 outNormal;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // diffuse
uniform sampler2D s1;   // pre-filter
uniform sampler2D s2;   // look-up-table

uniform float MAX_REFLECTION_LOD;
uniform float roughness;
uniform float metalic;
uniform vec4 F0;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

void main()
{
    vec3 V = -normalize(wi);
    vec3 R = normalize(reflection);
    vec3 N = normalize(outNormal);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(R.y) + MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan(R.x, R.z) + MATH_PI;

    // Normalize [0, 1]
    vec2 uv = vec2(phi / MATH_PI2, 1 - theta / MATH_PI);

    float NoV = max(dot(N, V), 0.0);
    vec3 F = F0.rgb + (vec3(1, 1, 1) - F0.rgb) * pow(1 - NoV, 5);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metalic;

    vec3 irradiance = texture2D(s0, uv).bgr;
    vec3 diffuse = irradiance * albedo.rgb;

    vec3 prefilteredColor = textureLod(s1, uv, roughness * MAX_REFLECTION_LOD).bgr;
    vec2 envBRDF = texture(s2, vec2(max(dot(N, V), 0.0), roughness)).bg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    outColor.rgb = kD * diffuse + specular;
    outColor.a = 1.0;
}
