#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__

//////////////////////////////////
// ガンマ補正

// NOTE
// pow(RGB, 1/ガンマ値)

// 標準ガンマ値
#define NTSC_GAMMA  (2.2f)  // NTSCで定められているガンマ値
#define MAC_GAMMA   (1.8f)  // マッキントッシュにおける標準のガンマ値で、フォトショップが推奨する値
#define WIN_GAMMA   (2.35f) // Windowsの標準のガンマ値は 2.2 - 2.5 なので、その間をとってみた

// 標準ガンマ値の逆数
#define INV_NTSC_GAMMA  (1.0f / NTSC_GAMMA)
#define INV_MAC_GAMMA   (1.0f / MAC_GAMMA)
#define INV_WIN_GAMMA   (1.0f / WIN_GAMMA)

// ガンマ値 を引数とする
float3 ColorFilterGammaControl(float3 vRGB, float fGamma)
{
    return pow(vRGB, 1.0f / fGamma);
}

// 1/ガンマ値 を引数とする
float3 ColorFilterGammaControlEx(float3 vRGB, float fGammmaInv)
{
    return pow(vRGB, fGammmaInv);
}

// ITU-R BT. 709 recommendation (Rec. 709)に規定されている変換

// NOTE
// R' = 4.5 * R                         (if R < 0.0018)
//      1.099f * pow(R, 1/2.2) - 0.099  (if R >= 0.0018)
// G'、B'も同様
//
// R = R' / 4.5f                      (if R' < 0.0812)
//     pow((R + 0.099) / 1.099, 2.2)  (if R' >= 0.0812)
// G、Bも同様

#define __GAMMA_CTRL_REC_709__(c)\
    (c < 0.018f\
        ? 4.5f * c\
        : 1.099f * pow(c, INV_NTSC_GAMMA) - 0.099f)

// TU-R BT. 709 recommendation (Rec. 709)に規定されているガンマ補正
float3 ColorFilterGammaControlByRec709(float3 vRGB)
{
    float3 vGammaRGB;
    vGammaRGB.r = __GAMMA_CTRL_REC_709__(vRGB.r);
    vGammaRGB.g = __GAMMA_CTRL_REC_709__(vRGB.g);
    vGammaRGB.b = __GAMMA_CTRL_REC_709__(vRGB.b);

    return vGammaRGB;
}

#define __RGB_FROM_GAMMA_CTRL_REC_709__(c)\
    (c < 0.0812f\
        ? c / 4.5f\
        : pow((c + 0.099f) / 1.099f, NTSC_GAMMA))

// TU-R BT. 709 recommendation (Rec. 709)に規定されているガンマ補正したものをRGBに戻す
float3 ComputeRGBFromGammaControlByRec709(float3 vGammaRGB)
{
    float3 vRGB;
    vRGB.r = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.r);
    vRGB.g = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.g);
    vRGB.b = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.b);

    return vRGB;
}

#endif  // #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__)
