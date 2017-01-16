#version 450
precision highp float;
precision highp int;

uniform sampler2D depthMap;
uniform sampler2D image;

uniform vec4 invScreen;

uniform int mode;

void main()
{
#if 0
    float depth = texelFetch(depthMap, ivec2(gl_FragCoord.xy), 0).r;

    if (depth >= 1.0) {
        discard;
    }

    vec4 color = texelFetch(image, ivec2(gl_FragCoord.xy), 0);
    color /= color.w;

    gl_FragColor = vec4(vec3(color.xyz), 1.0);
#else
    vec2 uv = gl_FragCoord.xy * invScreen.xy;
    uv.y = 1.0 - uv.y;

    float depth = texture2D(depthMap, uv).r;

    if (depth >= 1.0) {
        discard;
    }

    vec4 color = texture2D(image, uv);

#if 0
    if (color.w < 0.000001) {
        color.w = 0.0;
    }
#else
    color.w = step(0.000001, color.w) * color.w;
#endif

#if 0
    if (color.w == 0.0) {
        gl_FragColor = vec4(vec3(0.0), 1.0);
    }
    else {
        color.xyz /= color.w;
        gl_FragColor = vec4(vec3(color.xyz), 1.0);
    }
#else
    color.w = max(color.w, 0.0000011);
    color.xyz /= color.w;
    gl_FragColor = vec4(vec3(color.xyz), 1.0);
#endif
#endif
}
