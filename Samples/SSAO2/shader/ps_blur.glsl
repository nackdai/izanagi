#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // ssao

uniform vec4 invScreen;

#define BlurSize    (4)

void main()
{
    vec2 uv = varPos.xy;
    uv.y = 1.0 - uv.y;

    vec2 texelSize = invScreen.xy;
    vec2 hlim = vec2(-BlurSize * 0.5 + 0.5);

    float result = 0.0;

    for (int y = 0; y < BlurSize; y++) {
        for (int x = 0; x < BlurSize; x++) {
            vec2 offset = (hlim + vec2(float(x), float(y))) * texelSize;
            result += texture(s0, uv + offset).a;
        }
    }

    result /= float(BlurSize * BlurSize);

    outColor = vec4(result, result, result, 1);
}
