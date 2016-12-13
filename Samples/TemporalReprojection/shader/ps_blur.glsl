#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // ssao
uniform sampler2D s1;   // depth

uniform vec4 invScreen;

#define BlurSize    (4)

void main()
{
    vec2 uv = varPos.xy;
    uv.y = 1.0 - uv.y;

    vec2 texelSize = invScreen.xy;

#if 0
    vec2 hlim = vec2(-BlurSize * 0.5 + 0.5);

    float result = 0.0;

    for (int y = 0; y < BlurSize; y++) {
        for (int x = 0; x < BlurSize; x++) {
            vec2 offset = (hlim + vec2(float(x), float(y))) * texelSize;
            result += texture(s0, uv + offset).r;
        }
    }

    result /= float(BlurSize * BlurSize);

    outColor = vec4(result, result, result, 1);
#else
    const int numSamples = 9;

    const float samplerOffsets[numSamples] = float[](
        -8.0f, -6.0f, -4.0f, -2.0f, 0.0f, 2.0f, 4.0f, 6.0f, 8.0f);

    float depth = texture(s1, uv).r;

    float result = 0.0f;
    float weightSum = 0.0f;

    for (int y = 0; y < numSamples; y++)
    {
        for (int x = 0; x < numSamples; x++)
        {
            // Compute sample position for Gaussian blur.
            vec2 sampleOffset = vec2(samplerOffsets[x], samplerOffsets[y]) * texelSize;
            vec2 samplePos = uv.xy + sampleOffset;

            // Get the linear depth of the current sample. 
            float sampleDepth = texture(s1, samplePos).r;

            // Compute bilateral weighting for the current sample, than bigger the 
            // difference between the depth values, than smaller the weighting. 
            // 0.0001 is added to avoid division by zero. 
            float weight = (1.0f / (0.0001f + abs(depth - sampleDepth)));

            // Sample the point and do the weighting. 
            result += texture(s0, samplePos, 0).r * weight;

            // Add the weight to the total weight for computing the average later. 
            weightSum += weight;
        }
    }

    result /= weightSum;

    outColor = vec4(result, result, result, 1);
#endif
}
