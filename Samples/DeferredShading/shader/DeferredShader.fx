struct SVSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vTexCoord    : TEXCOORD0;
};

struct SPSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vTexCoord    : TEXCOORD0;
    float4 viewSpace    : TEXCOORD1;
};

#define SVSOutput        SPSInput

struct SPSOutput {
    float4 albedo   : COLOR0;
    float4 normal   : COLOR1;
    float4 depth    : COLOR2;
};

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2V;
float4x4 g_mW2C;

float g_farClip;

texture tex;

sampler smplTex = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVSGeometryPass(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);

    Out.viewSpace = mul(Out.vPos, g_mW2V).z;
    Out.viewSpace /= g_farClip;

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vTexCoord = In.vTexCoord;

    return Out;
}

SPSOutput mainPSGeometryPass(SPSInput In)
{
    SPSOutput Out = (SPSOutput)0;

    Out.albedo = tex2D(smplTex, In.vTexCoord);
    Out.albedo.a = 1.0f;

    Out.normal.rgb = In.vNormal * 0.5f + 0.5f;  // normalize [-1, 1] ->[0, 1]
    Out.normal.a = 1.0f;

    Out.depth = In.viewSpace;
    Out.depth.a = 1.0f;

    return Out;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVSLightPass(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);

    Out.viewSpace = Out.vPos;

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vTexCoord = Out.vPos.xy / Out.vPos.w;
    Out.vTexCoord = (Out.vTexCoord + 1.0f) * 0.5f;

    Out.vTexCoord.y = 1.0f - Out.vTexCoord.y;
    Out.vTexCoord = saturate(Out.vTexCoord);

    return Out;
}

texture texDepth;

sampler smplDepth = sampler_state
{
    Texture = texDepth;
};

// 位置
float4 g_PointLightPos;

// 係数
float4 g_PointLightAttn;

// 色
float4 g_PointLightColor;

float4x4 g_mtxC2V;
float4x4 g_mtxV2W;

float4 mainPSLightPass(SPSInput In) : COLOR0
{
    // NOTE
    // どうも nVidia のドライバのバグか何かで頂点シェーダで計算したUV座標がおかしくなるのでピクセルシェーダで計算する.
    // https://www.opengl.org/discussion_boards/showthread.php/176928-Computing-Fragment-Coordinate-in-vertex-shader
    // バグではなく、最適化のせいぽい・・・.
    float4 clipSpace = mul(In.viewSpace, g_mW2C);
    float2 uv = clipSpace.xy / clipSpace.w;
    uv = (uv + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    // Depth format is R32F.
    float depth = tex2D(smplDepth, uv).r;

    // Convert depth [0, 1] -> [0, far] in view-space.
    depth *= g_farClip;

    float4 pos = float4(uv.x, uv.y, 0.0f, 1.0f);

    // Convert from texcoord to clip-space.
    pos.y = 1.0f - pos.y;
    pos.xy = pos.xy * 2.0f - 1.0f;  // [0, 1] -> [-1, 1]

    // NOTE
    // P = (X, Y, Z, 1)
    // mtxV2C = W 0 0 0
    //          0 H 0 0
    //          0 0 A 1
    //          0 0 B 0
    // Pview * mtxV2C = (Xclip, Yclip, Zclip, Wclip) = (Xclip, Yclip, Zclip, Zview)
    //  Wclip = Zview = depth
    // X' = Xclip / Wclip = Xclip / Zview = Xclip / depth
    // Y' = Yclip / Wclip = Yclip / Zview = Yclip / depth
    //
    // X' * depth = Xclip
    // Xview = Xclip * mtxC2V

    pos.xy *= depth;
    pos = mul(pos, g_mtxC2V);

    pos.z = depth;

    pos = mul(pos, g_mtxV2W);

    // ポイントライトの減衰
    float d = length(g_PointLightPos - pos);

    float attn = 1.0f / (g_PointLightAttn.x + g_PointLightAttn.y * d + g_PointLightAttn.z * d * d);

    float4 Out = attn * g_PointLightColor;
    Out.a = 1.0f;

    return Out;
}

/////////////////////////////////////////////////////////////

texture texAlbedo;
texture texLight;

sampler smplAlbedo = sampler_state
{
    Texture = texAlbedo;
};
sampler smplLight = sampler_state
{
    Texture = texLight;
};

struct SVSInputFinalPass {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
};

struct SPSInputFinalPass {
    float4 vPos     : POSITION;
    float2 vUV      : TEXCOORD0;
};

#define SVSOutputFinalPass SPSInputFinalPass

// スクリーンサイズの逆数
float4 g_vInvScreen = (float4)1.0f;

SVSOutputFinalPass mainVSFinalPass(SVSInputFinalPass In)
{
    SVSOutputFinalPass sOut;

    // 頂点位置
    sOut.vPos = In.vPos;
    sOut.vPos.xy *= g_vInvScreen.xy;

    // [0, 1] -> [-1, -1]
    sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

    // さらにY座標は反転させる
    sOut.vPos.y *= -1.0f;

    // UV座標
    sOut.vUV = In.vUV;

    return sOut;
}

float4 mainPSFinalPass(SPSInputFinalPass In) : COLOR
{
    float4 albedo = tex2D(smplAlbedo, In.vUV);
    float4 light = tex2D(smplLight, In.vUV);

    albedo += light;

    return albedo;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vTexCoord = In.vTexCoord;

    return Out;
}

float4 mainPS(SPSInput In) : COLOR0
{
    float4 Out = 0;

    Out = tex2D(smplTex, In.vTexCoord);

    return Out;
}

/////////////////////////////////////////////////////////////

technique GBuffer
{
    pass test
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
    }

    pass geometryPass
    {
        VertexShader = compile vs_3_0 mainVSGeometryPass();
        PixelShader = compile ps_3_0 mainPSGeometryPass();
    }

    pass lightPass
    {
        VertexShader = compile vs_3_0 mainVSLightPass();
        PixelShader = compile ps_3_0 mainPSLightPass();

        ZWriteEnable = false;
        ZEnable = false;
    }

    pass finalPass
    {
        VertexShader = compile vs_3_0 mainVSFinalPass();
        PixelShader = compile ps_3_0 mainPSFinalPass();
    }
}
