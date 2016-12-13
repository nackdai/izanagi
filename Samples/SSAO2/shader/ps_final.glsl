#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // albedo
uniform sampler2D s1;   // ssao

void main()
{
    vec2 uv = varPos.xy;
    uv.y = 1.0 - uv.y;

    vec4 albedo = texture(s0, uv);
    float ssao = texture(s1, uv).r;

    albedo.rgb *= ssao;

    outColor = albedo;
}
