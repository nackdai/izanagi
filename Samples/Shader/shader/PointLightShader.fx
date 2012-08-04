// PointLight Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
		
	Out.vColor = In.vColor;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	float4 vOut = In.vColor;
	
	return vOut;
}

/////////////////////////////////////////////////////////////

technique PointLight
{
	pass P0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
