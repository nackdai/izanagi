#version 450
precision highp float;
precision highp int;

uniform sampler2D s0;
uniform sampler2D s1;

varying vec3 uv;

#define MATH_PI          (3.14159265358979323846f)   // pi
#define MATH_PI2         (MATH_PI * 2.0f)         // 2pi
#define MATH_PI1_2       (MATH_PI * 0.5f)         // pi/2

uniform vec4 invScreen;

uniform bool canFoveated;

void main()
{
    vec3 ref = normalize(uv);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(ref.y) + MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan(ref.x, ref.z) + MATH_PI;

    // Normalize [0, 1]
    vec2 uv = vec2(phi / MATH_PI2, 1 - theta / MATH_PI);
    gl_FragColor = texture2D(s0, uv);

    if (canFoveated) {
        vec2 maskuv = gl_FragCoord.xy * invScreen.xy;
        vec4 mask = texture2D(s1, maskuv);

        gl_FragColor.rgb *= mask.r;
    }
}
