// Phong Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float3 vNormal	: NORMAL;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;	// 法線
	float3 vHalf	: TEXCOORD1;	// ハーフベクトル
	float2 vUV		: TEXCOORD2;
	float4 vColor	: COLOR;		// Ambient
};

#define SVSOutput		SPSInput

struct SVSInput_NoTex {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

struct SPSInput_NoTex {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

#define SVSOutput_NoTex	SPSInput_NoTex

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;
float4 g_vEye;

// マテリアル
float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

// ライト
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

sampler sTex : register(s0);

/////////////////////////////////////////////////////////////
// 頂点シェーダ

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	// 視点への方向ベクトル（ローカル座標）
	float3 vV = normalize(g_vEye - In.vPos);
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	Out.vNormal = normalize(In.vNormal);

	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

	// NOTE
	// ローカル座標での計算なので
	// ライトの方向ベクトルはCPU側でローカル座標に変換されていること

	Out.vHalf = normalize(-g_vLitParallelDir + vV);
	
	Out.vColor.a = 1.0f;

	Out.vUV = In.vUV;
	
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
	vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

	vOut *= tex2D(sTex, In.vUV);
	
	return vOut;
}

////////////////////////////////////////////////

SVSOutput_NoTex mainVS_Std(SVSInput_NoTex sIn)
{
	SVSOutput_NoTex sOut = (SVSOutput_NoTex)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;
	sOut.vUV = sIn.vUV;

	return sOut;
}

float4 mainPS_Std(SPSInput_NoTex sIn)	: COLOR
{
	float4 vOut = sIn.vColor * tex2D(sTex, sIn.vUV);

	return saturate(vOut);
}

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex sIn)
{
	SVSOutput_NoTex sOut = (SVSOutput_NoTex)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;

	return sOut;
}

float4 mainPS_NoTex(SPSInput_NoTex sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	return saturate(vOut);
}

/////////////////////////////////////////////////////////////

technique PhongShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}

#if 1
	pass p1
	{
		VertexShader = compile vs_2_0 mainVS_NoTex();
		PixelShader = compile ps_2_0 mainPS_NoTex();
	}

	pass p2
	{
		VertexShader = compile vs_2_0 mainVS_Std();
		PixelShader = compile ps_2_0 mainPS_Std();
	}
#endif
}
