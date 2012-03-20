// Basic Shader

struct SVSInput {
	float4 vPos	: POSITION;
};

struct SPSInput {
	float4 vPos	: POSITION;
	float3 vUV	: TEXCOORD0;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

texture tex;

samplerCUBE sTex = sampler_state
{
	Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	// NOTE
	// -1.0f - 1.0f のボックスであること！！
	Out.vUV = In.vPos.xyz;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	float4 vOut = texCUBE(sTex, In.vUV);

	return vOut;
}

/////////////////////////////////////////////////////////////

technique BasicShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
