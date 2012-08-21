// SpotLight Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
	float distance	: TEXCOORD0;
	float attn		: TEXCOORD1;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

// 位置
float4 g_SpotLightPos;

// 方向
float4 g_SpotLightDir;

// 係数
float4 g_SpotLight;

// 色
float4 g_SpotLightColor;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = mul(In.vPos, g_mL2W);

	// ライトへの方向ベクトル
	float3 vLightDir = g_SpotLightPos.xyz - Out.vPos.xyz;

	// ライトとの距離
	Out.distance = length(vLightDir);

	// 単位ベクトルにする
	vLightDir = normalize(vLightDir);

	// ライトベクトルの逆向きとスポットライトとの内積
	Out.attn = max(dot(g_SpotLightDir.xyz, -vLightDir), 0.0f);

	Out.vPos = mul(Out.vPos, g_mW2C);
		
	Out.vColor = In.vColor;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// 減衰を計算
	float d = In.distance;
	float attn = pow(In.attn, g_SpotLight.w) / (g_SpotLight.x + g_SpotLight.y * d + g_SpotLight.z * d * d);

	float4 vOut = In.vColor + attn * g_SpotLightColor;
	vOut.a = In.vColor.a;
	
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

technique PointLight
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
