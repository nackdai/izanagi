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
	float3 vHalf	: TEXCOORD0;	// ハーフベクトル
	float2 vUV		: TEXCOORD1;
	float3 vLight	: TEXCOORD2;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

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

	// 視点への方向ベクトル（ローカル座標）
	float3 vV = normalize(g_vEye - In.vPos).xyz;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

	// NOTE
	// ローカル座標での計算なので
	// ライトの方向ベクトルはCPU側でローカル座標に変換されていること

	float3 vL = -g_vLitParallelDir.xyz;

	// Tangent-space
	float3 vN = In.vNormal;
	float3 vT = In.vTangent.xyz;
	float3 vB = In.vTangent.w * cross(vN, vT);

	// TODO
	// 法線マップの座標系がY軸上向き、Z軸手前、X軸右向き（右手座標系）なので
	// それにあわせる
	vB.xyz = -vB.xyz;

	// Tangent-spaceに変換
	{
		vV.x = dot(vV, vT);
		vV.y = dot(vV, vB);
		vV.z = dot(vV, vN);

		Out.vLight.x = dot(vL, vT);
		Out.vLight.y = dot(vL, vB);
		Out.vLight.z = dot(vL, vN);
		Out.vLight = normalize(Out.vLight);
	}

	// Tangent-spaceでのハーフベクトルを計算
	Out.vHalf = normalize(Out.vLight + vV);
	
	Out.vColor.rgb *= In.vColor.rgb;
	Out.vColor.a = In.vColor.a;

	Out.vUV = In.vUV;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// 頂点シェーダでAmbientについては計算済み
	float4 vOut = In.vColor;

	float3 vN = tex2D(sTexNormal, In.vUV).xyz;
	vN = (vN - 0.5f) * 2.0f;
	vN = normalize(vN);

	float3 vL = In.vLight;
	float3 vH = In.vHalf;
	
	// Diffuse = Md * ∑(C * max(N・L, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
	
	// Specular = Ms * ∑(C * pow(max(N・H, 0), m))
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
