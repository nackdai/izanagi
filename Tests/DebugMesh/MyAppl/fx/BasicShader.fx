// BasicShader.fx

float4x4 g_mL2W;
float4x4 g_mW2C;

sampler sTex : register(s0);

////////////////////////////////////////////////

struct SVSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
	float2 vUV		: TEXCOORD0;
};

struct SVSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
	float2 vUV		: TEXCOORD0;
};

struct SPSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput
#define SVSOutput_NoTex	SPSInput_NoTex

////////////////////////////////////////////////
// 頂点シェーダ

SVSOutput mainVS(SVSInput sIn)
{
	SVSOutput sOut = (SVSOutput)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;
	sOut.vUV = sIn.vUV;

	return sOut;
}

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex sIn)
{
	SVSOutput_NoTex sOut = (SVSOutput_NoTex)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;

	return sOut;
}

////////////////////////////////////////////////
// ピクセルシェーダ

float4 mainPS(SPSInput sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	vOut *= tex2D(sTex, sIn.vUV);

	return saturate(vOut);
}

float4 mainPS_NoTex(SPSInput_NoTex sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	return saturate(vOut);
}

////////////////////////////////////////////////

// NOTE
// テクニックの切り替えは面倒なので、パスの切り替えで対応・・・

technique t0
{
	pass p0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}

	pass p1
	{
		VertexShader = compile vs_2_0 mainVS_NoTex();
		PixelShader = compile ps_2_0 mainPS_NoTex();
	}
}
