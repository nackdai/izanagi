#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 albedo;
layout(location = 1) in vec3 reflection;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

void main()
{
    vec3 normref = normalize(reflection);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(normref.y) + MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan(normref.x, normref.z) + MATH_PI;

    // Normalize [0, 1]
    vec2 uv = vec2(phi / MATH_PI2, 1 - theta / MATH_PI);

    vec4 spec = texture2D(s0, uv).bgra;

    outColor = spec * albedo;
    outColor.a = 1.0;
}
