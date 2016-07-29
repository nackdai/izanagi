#version 450
precision highp float;
precision highp int;

uniform sampler2D s0;

varying vec3 uv;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

void main()
{
    vec3 ref = normalize(uv);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(ref.y) + MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan(ref.x, ref.z) + MATH_PI;

    // Normalize [0, 1]
    vec2 uv = vec2(phi / MATH_PI2, 1.0 - theta / MATH_PI);

    gl_FragColor = texture2D(s0, uv);
}
