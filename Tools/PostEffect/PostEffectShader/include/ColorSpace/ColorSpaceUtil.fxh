#if !defined(__IZANAGI_POSTEFFECT_COLOR_SPACE_UTIL_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_SPACE_UTIL_FXH__ 

static const float3 RGB2Y  = {  0.29891f,  0.58661f,  0.11448f };

// RGB -> Y
float RGBToY(float3 vRGB)
{
    return dot(vRGB, RGB2Y);
}

// 最大チャンネルを取得
float _GetMaxChannel(float3 vCol)
{
    return max(max(vCol.r, vCol.g), vCol.b);
}

// 最小チャンネルを取得
float _GetMinChannel(float3 vCol)
{
    return min(min(vCol.r, vCol.g), vCol.b);
}

// 色相(Hue)を計算
float _ComputeHue(float3 vRGB, float fMax, float fDelta)
{
    float fH;

    // NOTE
    // 0 - 360 -> 0 - 1 になるように変換する

    if (vRGB.r == fMax) {
        // MAX = R
        fH = ((vRGB.g - vRGB.b) / fDelta) / 6.0f;
    }
    else if (vRGB.g == fMax) {
        // MAX = G
        fH = ((vRGB.b - vRGB.r) / fDelta) / 6.0f + 1.0f / 3.0f;
    }
    else if (vRGB.b == fMax) {
        // MAX = B
        fH = ((vRGB.r - vRGB.g) / fDelta) / 6.0f + 2.0f / 3.0f;
    }

    // 0.0f - 1.0f に収まるようにする
    fH = (fH < 0.0f ? fH + 1.0f : fH);
    fH = (fH > 1.0f ? fH - 1.0f : fH);

    return fH;
}

#endif  // #if !defined(__IZANAGI_POSTEFFECT_COLOR_SPACE_UTIL_FXH__)
