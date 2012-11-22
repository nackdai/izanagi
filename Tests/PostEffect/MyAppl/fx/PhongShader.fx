// Phong Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
};

struct SVSOutput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;	// 法線
	float3 vHalf	: TEXCOORD1;	// ハーフベクトル
	float4 vColor	: COLOR;		// Ambient
};

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

/////////////////////////////////////////////////////////////
// 頂点シェーダ

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	
	// 視点への方向ベクトル
	float3 vV = normalize(g_vEye - Out.vPos);
	
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
	Out.vNormal = normalize(Out.vNormal);

	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;
	
	Out.vHalf = normalize(g_vLitParallelDir + vV);
	
	Out.vColor.a = 1.0f;
	
	return Out;
}

/////////////////////////////////////////////////////////////
// ピクセルシェーダ

float4 mainPS(SVSOutput In) : COLOR
{
	// 頂点シェーダでAmbientについては計算済み
	float4 vOut = In.vColor;
	
	// いるのか・・・
	float3 vN = normalize(In.vNormal);
	float3 vH = normalize(In.vHalf);
	
	// Diffuse = Md * ∑(C * max(N・L, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, g_vLitParallelDir));
	
	// Specular = Ms * ∑(C * pow(max(N・H, 0), m))
	vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));
	
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
