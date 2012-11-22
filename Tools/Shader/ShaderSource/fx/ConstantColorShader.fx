#include "Shader.fxh"

////////////////////////////////////////////////

struct SVSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

////////////////////////////////////////////////
// 頂点シェーダ

SVSOutput mainVS(SVSInput sIn)
{
	SVSOutput sOut = (SVSOutput)0;

	sOut.vPos = ApplyL2W(sIn.vPos);
	sOut.vPos = ApplyW2C(sOut.vPos);

	sOut.vColor = sIn.vColor;

	return sOut;
}

////////////////////////////////////////////////
// ピクセルシェーダ

float4 mainPS(SPSInput sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	return saturate(vOut);
}

////////////////////////////////////////////////

technique t0
{
	pass p0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
