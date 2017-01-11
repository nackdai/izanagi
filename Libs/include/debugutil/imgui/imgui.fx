float4x4 ProjMtx;

struct VS_INPUT
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};

#define VS_OUTPUT   PS_INPUT

VS_OUTPUT mainVS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos.xy, 0.f, 1.f), ProjMtx);
    output.col = input.col;
    output.uv = input.uv;
    return output;
}

sampler sTex : register(s0);

float4 mainPS(PS_INPUT input) : COLOR
{
    float4 out_col = input.col * tex2D(sTex, input.uv);
    return out_col;
}
