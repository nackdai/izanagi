#include "../include/Shader.fxh"

// Phong Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
};

struct SVSOutput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;	// �@��
	float3 vHalf	: TEXCOORD1;	// �n�[�t�x�N�g��
	float4 vColor	: COLOR;		// Ambient
};

/////////////////////////////////////////////////////////////
// ���_�V�F�[�_

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = ApplyL2W(In.vPos);
	
	// Half�x�N�g���v�Z
	Out.vHalf = ComputeHalfVectorEx(0, Out.vPos);
	
	Out.vPos = ApplyW2C(Out.vPos);
	
	Out.vNormal = ApplyL2W(In.vNormal);
	Out.vNormal = normalize(Out.vNormal);

	// Ambient
	Out.vColor = ComputeAmbientLight();
	
	Out.vColor.a = 1.0f;
	
	return Out;
}

/////////////////////////////////////////////////////////////
// �s�N�Z���V�F�[�_

float4 mainPS(SVSOutput In) : COLOR
{
	// ���_�V�F�[�_��Ambient�ɂ��Ă͌v�Z�ς�
	float4 vOut = In.vColor;
	
	// ����̂��E�E�E
	float3 vN = normalize(In.vNormal);
	float3 vH = normalize(In.vHalf);
		
	vOut.rgb += ComputePhongLight(0, vN, vH);

	return vOut;
}

/////////////////////////////////////////////////////////////

technique PhongShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
