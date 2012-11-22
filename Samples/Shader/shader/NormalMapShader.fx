// Phong Shader

#define USE_REFLECTION

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
	float4 vColor	: COLOR;
	float2 vUV		: TEXCOORD0;
	float4 vTangent : TANGENT;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vHalf	: TEXCOORD0;	// �n�[�t�x�N�g��
	float2 vUV		: TEXCOORD1;
	float3 vLight	: TEXCOORD2;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;
float4 g_vEye;

// �}�e���A��
float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

// ���C�g
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

texture texAlbedo;
texture texNormal;

sampler sTexAlbedo = sampler_state
{
	Texture = texAlbedo;
};

sampler sTexNormal = sampler_state
{
	Texture = texNormal;
};


/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	// ���_�ւ̕����x�N�g���i���[�J�����W�j
	float3 vV = normalize(g_vEye - In.vPos).xyz;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

	// NOTE
	// ���[�J�����W�ł̌v�Z�Ȃ̂�
	// ���C�g�̕����x�N�g����CPU���Ń��[�J�����W�ɕϊ�����Ă��邱��

	float3 vL = -g_vLitParallelDir.xyz;

	// Tangent-space
	float3 vN = In.vNormal;
	float3 vT = In.vTangent.xyz;
	float3 vB = In.vTangent.w * cross(vN, vT);

	// TODO
	// �@���}�b�v�̍��W�n��Y��������AZ����O�AX���E�����i�E����W�n�j�Ȃ̂�
	// ����ɂ��킹��
	vB.xyz = -vB.xyz;

	// Tangent-space�ɕϊ�
	{
		vV.x = dot(vV, vT);
		vV.y = dot(vV, vB);
		vV.z = dot(vV, vN);

		Out.vLight.x = dot(vL, vT);
		Out.vLight.y = dot(vL, vB);
		Out.vLight.z = dot(vL, vN);
		Out.vLight = normalize(Out.vLight);
	}

	// Tangent-space�ł̃n�[�t�x�N�g�����v�Z
	Out.vHalf = normalize(Out.vLight + vV);
	
	Out.vColor.rgb *= In.vColor.rgb;
	Out.vColor.a = In.vColor.a;

	Out.vUV = In.vUV;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// ���_�V�F�[�_��Ambient�ɂ��Ă͌v�Z�ς�
	float4 vOut = In.vColor;

	float3 vN = tex2D(sTexNormal, In.vUV).xyz;
	vN = (vN - 0.5f) * 2.0f;
	vN = normalize(vN);

	float3 vL = In.vLight;
	float3 vH = In.vHalf;
	
	// Diffuse = Md * ��(C * max(N�EL, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
	
	// Specular = Ms * ��(C * pow(max(N�EH, 0), m))
	vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

	vOut *= tex2D(sTexAlbedo, In.vUV);
	
	return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_NoLight(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
		
	Out.vColor = In.vColor;
	
	return Out;
}

float4 mainPS_NoLight(SPSInput In) : COLOR
{
	float4 vOut = In.vColor;
	
	return vOut;
}

/////////////////////////////////////////////////////////////

technique Phong
{
	pass P0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}

	pass P1
	{
		VertexShader = compile vs_3_0 mainVS_NoLight();
		PixelShader = compile ps_3_0 mainPS_NoLight();
	}

}
