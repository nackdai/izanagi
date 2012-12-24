#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__

//////////////////////////////////
// CMY

// NOTE
// CMYは印刷の過程で利用する減法混色の表現法である。
// 絵の具の三原色。基本色は白で、それに色の度合いを加えて、黒色にしていく。
// すなわち、始めは白いキャンバスから始め、インクを加えて暗くしていく
// （反射光を減らす、すなわち減法）ということである。

// NOTE
// CMY -> シアン (Cyan)、マゼンタ (Magenta)、イエロー (Yellow) 

// NOTE
// CMY = 1 - RGB

// RGB -> CMY
float3 RGBToCMY(float3 vRGB)
{
    return (float3(1.0f, 1.0f, 1.0f) - vRGB);
}

// CMY -> RGB
float3 CMYToRGB(float3 vCMY)
{
    return (float3(1.0f, 1.0f, 1.0f) - vCMY);
}

#endif  // #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__)
