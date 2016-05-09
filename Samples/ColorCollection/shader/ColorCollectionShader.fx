struct SVSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
};

texture tex;

sampler sTex = sampler_state
{
    Texture = tex;
};

#define SVSOutput SPSInput

// スクリーンサイズの逆数
float4 g_vInvScreen = (float4)1.0f;

SVSOutput mainVS(SVSInput sIn)
{
    SVSOutput sOut;

    // 頂点位置
    sOut.vPos = sIn.vPos;
    sOut.vPos.xy *= g_vInvScreen.xy;

    // [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
    sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

    // さらにY座標は反転させる
    sOut.vPos.y *= -1.0f;

    // 頂点カラー
    sOut.vColor = sIn.vColor;

    // UV座標
    sOut.vUV = sIn.vUV;

    return sOut;
}

// NOTE
// 色収差補正.
// http://www.nikon.co.jp/profile/technology/life/imaging/aberration/

// NOTE
// https://www.shadertoy.com/view/4sX3z4
// https://www.shadertoy.com/view/4s2XDd

// NOTE
// Oculus's color collection.
// https://github.com/dghost/glslRiftDistort

// NOTE
// Oclus's parameters?
// https://github.com/evanw/oculus-rift-webgl/blob/master/inverse-distortion.html

float4 mainPS(SPSInput sIn) : COLOR
{
    float aberrationStrength = 1.25f;
    float vignetteStrength = 0.6f;

    float2 d = sIn.vUV - float2(0.5f, 0.5f);
    float dist = dot(d, d);

    float2 uvR = sIn.vUV * (1.0 + dist * 0.02 * aberrationStrength);
    float2 uvG = sIn.vUV;
    float2 uvB = sIn.vUV * (1.0 - dist * 0.02 * aberrationStrength);

    float r = tex2D(sTex, uvR).r;
    float g = tex2D(sTex, uvG).g;
    float b = tex2D(sTex, uvB).b;

    float4 outColor = float4(r, g, b, 1.0f);

    // Apply vignette.
    outColor *= 1.0f - dist * vignetteStrength;
    outColor.a = 1.0f;

    return outColor;
}

technique ColorCollection
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
