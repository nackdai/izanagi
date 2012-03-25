// Skin Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
	float2 vUV		: TEXCOORD0;

	float4 vIndices	: BLENDINDICES;
	float4 vWeight	: BLENDWEIGHT;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;	// 法線
	float3 vHalf	: TEXCOORD1;
	float2 vUV		: TEXCOORD2;
	float4 vColor	: COLOR0;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mW2C;

float4 g_vEye;

// マテリアル
float4 g_vMtrlDiffuse;
float4 specular;
float4 ambient;
float shininess;

// ライト
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

texture tex;

sampler sTex = sampler_state
{
	Texture = tex;
};

/////////////////////////////////////////////////////////////
// 頂点シェーダ

#define JOINT_MTX_NUM	(48)
float4x4 vJointMatrix[JOINT_MTX_NUM];

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	for (int i = 0; i < 4; i++) {
		float idx = In.vIndices[i];
		float weight = In.vWeight[i];

		float4x4 mtx = vJointMatrix[idx];

		Out.vPos += mul(In.vPos, mtx) * weight;
		Out.vNormal += mul(In.vNormal, (float3x3)mtx) * weight;
	}

	// 視点への方向ベクトル（ローカル座標）
	float3 vV = normalize(g_vEye - Out.vPos).xyz;
		
	Out.vPos = mul(Out.vPos, g_mW2C);

	Out.vUV = In.vUV;
#if 1
	Out.vUV.y = 1.0f - Out.vUV.y;
#endif

	// Ambient
	Out.vColor = ambient * g_vLitAmbientColor;

	// NOTE
	// ローカル座標での計算なので
	// ライトの方向ベクトルはCPU側でローカル座標に変換されていること

	Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// 頂点シェーダでAmbientについては計算済み
	float4 vOut = In.vColor;
	
	// いるのか・・・
	float3 vN = normalize(In.vNormal);
	float3 vH = normalize(In.vHalf);
	float3 vL = -g_vLitParallelDir.xyz;

	// Diffuse = Md * ∑(C * max(N・L, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
	
	// Specular = Ms * ∑(C * pow(max(N・H, 0), m))
	vOut.rgb += specular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(shininess, 0.00001f));

	// テクスチャ
	float4 texColor = tex2D(sTex, In.vUV);
	vOut.rgb *= texColor.rgb;

	vOut.a = texColor.a;

	return vOut;
}

/////////////////////////////////////////////////////////////

technique t0
{
	pass p0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
