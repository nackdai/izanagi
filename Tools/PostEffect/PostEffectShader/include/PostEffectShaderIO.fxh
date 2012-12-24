#if !defined(__IZANAGI_POSTEFFECT_SHADER_IO_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_IO_FXH__

/********************************************
   シェーダ入出力
********************************************/

#define NUM_SAMPLING_4  (2)
#define NUM_SAMPLING_8  (4)
#define NUM_SAMPLING_16 (8)

#define NUM_SAMPLING_5  (2)
#define NUM_SAMPLING_7  (3)
#define NUM_SAMPLING_9  (4)
#define NUM_SAMPLING_13 (6)

//////////////////////////////////////
// サンプリング点が１個の場合

// ピクセルシェーダ入力
// デフォルト
struct S_PE_PS_IN{
    float4 vPos : POSITION;
    float2 vUV  : TEXCOORD0;
};

#define S_PE_PS_IN_SAMPLING_1 S_PE_PS_IN

//////////////////////////////////////
// サンプリング点が４個の場合

// ピクセルシェーダ入力
// 4ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_4{
    float4 vPos                 : POSITION;
    float4 vUV[NUM_SAMPLING_4]  : TEXCOORD0;
};

//////////////////////////////////////
// サンプリング点が８個の場合

// ピクセルシェーダ入力
// 8ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_8{
    float4 vPos                 : POSITION;
    float4 vUV[NUM_SAMPLING_8]  : TEXCOORD0;
};

//////////////////////////////////////
// サンプリング点が１６個の場合

// ピクセルシェーダ入力
// 16ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_16{
    float4 vPos                 : POSITION;
    float4 vUV[NUM_SAMPLING_16] : TEXCOORD0;
};

//////////////////////////////////////
// サンプリング点が５個の場合

// ピクセルシェーダ入力
// 5ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_5{
    float4 vPos                 : POSITION;
    float2 vUV_0                : TEXCOORD0;
    float4 vUV[NUM_SAMPLING_5]  : TEXCOORD1;
};

//////////////////////////////////////
// サンプリング点が７個の場合

// ピクセルシェーダ入力
// 7ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_7{
    float4 vPos                 : POSITION;
    float2 vUV_0                : TEXCOORD0;
    float4 vUV[NUM_SAMPLING_7]  : TEXCOORD1;
};

//////////////////////////////////////
// サンプリング点が９個の場合

// ピクセルシェーダ入力
// 9ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_9{
    float4 vPos                 : POSITION;
    float2 vUV_0                : TEXCOORD0;
    float4 vUV[NUM_SAMPLING_9]  : TEXCOORD1;
};

//////////////////////////////////////
// サンプリング点が１３個の場合

// ピクセルシェーダ入力
// 13ポイントサンプリング
struct S_PE_PS_IN_SAMPLING_13{
    float4 vPos                 : POSITION;
    float2 vUV_0                : TEXCOORD0;
    float4 vUV[NUM_SAMPLING_13] : TEXCOORD1;
};

//////////////////////////////////////

#define S_PE_PS_IN_DOWNSCALE_4x4_LINEAR S_PE_PS_IN_SAMPLING_4
#define S_PE_PS_IN_DOWNSCALE_4x4_POINT  S_PE_PS_IN_SAMPLING_16
#define S_PE_PS_IN_DOWNSCALE_2x2_LINEAR S_PE_PS_IN_SAMPLING_1
#define S_PE_PS_IN_DOWNSCALE_2x2_POINT  S_PE_PS_IN_SAMPLING_4
#define S_PE_PS_IN_DOWNSCALE_3x3_LINEAR S_PE_PS_IN_SAMPLING_4
#define S_PE_PS_IN_DOWNSCALE_3x3_POINT  S_PE_PS_IN_SAMPLING_9
#define S_PE_PS_IN_GAUSSBLUR_5x5        S_PE_PS_IN_SAMPLING_13
#define S_PE_PS_IN_BLOOMBLUR_LINEAR     S_PE_PS_IN_SAMPLING_7
#define S_PE_PS_IN_BLOOMBLUR_POINT      S_PE_PS_IN_SAMPLING_13
#define S_PE_PS_IN_STARBLUR             S_PE_PS_IN_SAMPLING_8
#define S_PE_PS_IN_9POINTSAMPLE_LINEAR  S_PE_PS_IN_SAMPLING_5
#define S_PE_PS_IN_9POINTSAMPLE_POINT   S_PE_PS_IN_SAMPLING_9

#define NUM_SAMPLING_DOWNSCALE_4x4_LINEAR   NUM_SAMPLING_4
#define NUM_SAMPLING_DOWNSCALE_4x4_POINT    NUM_SAMPLING_16
#define NUM_SAMPLING_DOWNSCALE_2x2_LINEAR   // 使用する意味がない・・・
#define NUM_SAMPLING_DOWNSCALE_2x2_POINT    NUM_SAMPLING_4
#define NUM_SAMPLING_DOWNSCALE_3x3_LINEAR   NUM_SAMPLING_4
#define NUM_SAMPLING_DOWNSCALE_3x3_POINT    NUM_SAMPLING_9
#define NUM_SAMPLING_GAUSSBLUR              NUM_SAMPLING_13
#define NUM_SAMPLING_BLOOMBLUR_LINEAR       NUM_SAMPLING_7
#define NUM_SAMPLING_BLOOMBLUR_POINT        NUM_SAMPLING_13
#define NUM_SAMPLING_STARBLUR               NUM_SAMPLING_8
#define NUM_SAMPLING_9POINTSAMPLE_LINEAR    NUM_SAMPLING_5
#define NUM_SAMPLING_9POINTSAMPLE_POINT     NUM_SAMPLING_9

#endif  // #if !defined(__IZANAGI_POSTEFFECT_SHADER_IO_FXH__)
