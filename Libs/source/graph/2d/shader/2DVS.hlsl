// 2D用頂点シェーダ

#include "2DShader.hlsl"

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
