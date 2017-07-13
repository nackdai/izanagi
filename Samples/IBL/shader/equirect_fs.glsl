#version 450
precision highp float;
precision highp int;

uniform sampler2D s0;
uniform sampler2D s1;

layout(location = 0) in vec3 ref;

layout(location = 0) out vec4 oColor;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

void main()
{
    vec3 normref = normalize(ref);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(normref.y) + MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan(normref.x, normref.z) + MATH_PI;

    // Normalize [0, 1]
    vec2 uv = vec2(phi / MATH_PI2, 1 - theta / MATH_PI);
    oColor = texture2D(s0, uv).bgra;
    oColor.a = 1.0;
}
