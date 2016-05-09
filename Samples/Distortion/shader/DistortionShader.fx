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
// Brown のレンズ歪みモデル.
// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20140801#f20140801-01-01
// http://www.roboken.esys.tsukuba.ac.jp/~ohya/pdf/Robomech2014-KNS.pdf

// NOTE
// Oculus's distortion.
// http://rifty-business.blogspot.jp/2013/08/understanding-oculus-rift-distortion.html
// https://github.com/dghost/glslRiftDistort

// NOTE
// Oclus's parameters?
// https://github.com/evanw/oculus-rift-webgl/blob/master/inverse-distortion.html

float4 mainPS(SPSInput sIn) : COLOR
{
    float2 d = sIn.vUV - 0.5f;
    float d2 = d.x * d.x + d.y * d.y;
    float d4 = d2 * d2;

    //float t = 1.0f + 0.18f * d2 + 0.115f * d4;
    float t = 1.0f + 0.22f * d2 + 0.24f * d4;

    float2 uv = float2(t * d.x + 0.5f, t * d.y + 0.5f);

    float4 outColor = tex2D(sTex, uv);

    if (uv.x < 0.0f || uv.y < 0.0f || uv.x > 1.0f || uv.y > 1.0f) {
        outColor.rgb = 0.0f;
        outColor.a = 1.0f;
    }

    return outColor;
}

technique Distortion
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
