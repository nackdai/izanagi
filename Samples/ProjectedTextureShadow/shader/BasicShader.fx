// Basic Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

struct SVSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput_NoTex	SPSInput_NoTex

struct SPSInput_WithShadow {
	float4 vPos			: POSITION;
	float4 vColor		: COLOR;
	float4 vShadowUV	: TEXCOORD0;
};

#define SVSOutput_WithShadow	SPSInput_WithShadow

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

float4x4 g_mShadowTex;

texture tex;

sampler sTex = sampler_state
{
	Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	Out.vUV = In.vUV;
	Out.vColor = In.vColor;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	float4 vOut = tex2D(sTex, In.vUV);

	vOut.rgb *= In.vColor.rgb;
	vOut.rgb *= In.vColor.a;

	return vOut;
}

////////////////////////////////////////////////

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex In)
{
	SVSOutput_NoTex Out = (SVSOutput_NoTex)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	Out.vColor = In.vColor;

	return Out;
}

float4 mainPS_NoTex(SPSInput_NoTex In)	: COLOR
{
	float4 vOut = In.vColor;

	return vOut;
}

/////////////////////////////////////////////////////////////

float4 mainPS_ForShadow(SPSInput In) : COLOR
{
	return In.vColor;
}

/////////////////////////////////////////////////////////////

SVSOutput_WithShadow mainVS_WithShadow(SVSInput_NoTex In)
{
	SVSOutput_WithShadow Out = (SVSOutput_WithShadow)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	Out.vColor = In.vColor;

	Out.vShadowUV = mul(In.vPos, g_mShadowTex);

	return Out;
}

float4 mainPS_WithShadow(SPSInput_WithShadow In) : COLOR
{
	float2 uv = In.vShadowUV.xy / In.vShadowUV.w;
	float4 Out = In.vColor;
	Out.rgb *= (1.0f - tex2D(sTex, uv).rgb);

	return Out;
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

technique BasicShader_NoTex
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS_NoTex();
		PixelShader = compile ps_2_0 mainPS_NoTex();
	}
}

technique BasicShader_ForShadow
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS_ForShadow();
	}
}

technique BasicShader_WithShadow
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS_WithShadow();
		PixelShader = compile ps_2_0 mainPS_WithShadow();
	}
}
