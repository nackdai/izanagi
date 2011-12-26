// ポストエフェクト　被写界深度

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_DOF");

////////////////////////////////////////////////////

// シーンテクスチャ
texture texScene : INPUT_SCENE
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// 1/4 x 1/4 縮小バッファ
texture texDownScale4x4
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
>;

// ブラーテクスチャ
texture texBlur
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
>;

// 外部入力------------------------------

// Z値テクスチャ
texture texZ : INPUT
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// ウエイトテクスチャ
texture texWeight : INPUT
<
	SetTexFormat(PIXEL_FORMAT_A8);
	SetTexSize(512, 1);
>;

////////////////////////////////////////////////////

// シーン
sampler smplScene = sampler_state
{
	BindTex(texScene);

#ifdef __DOF_POINT__
	SetMinFilter(SMPL_FILTER_POINT);
	SetMagFilter(SMPL_FILTER_POINT);
#else
	SetMinFilter(SMPL_FILTER_LINEAR);
	SetMagFilter(SMPL_FILTER_LINEAR);
#endif
};

// ウエイト
sampler smplWeight = sampler_state
{
	BindTex(texWeight);
};

// Z値
sampler smplZ = sampler_state
{
	BindTex(texZ);
};

// 1/4 x 1/4 縮小
sampler smplDownScale4x4 = sampler_state
{
	BindTex(texDownScale4x4);

#ifdef __DOF_POINT__
	SetMinFilter(SMPL_FILTER_POINT);
	SetMagFilter(SMPL_FILTER_POINT);
#else
	SetMinFilter(SMPL_FILTER_LINEAR);
	SetMagFilter(SMPL_FILTER_LINEAR);
#endif
};

// ブラー
sampler smplBlur = sampler_state
{
	BindTex(texBlur);
};

////////////////////////////////////////////////////

// 視点から注視点までの距離
float fEyeToAtLength : CAMERA_PARAM_EYE_TO_AT_LENGTH
<
	SetDoNotStrip(true);
>;

// カメラ絞り
float fAperture : CAMERA_PARAM_APERTURE
<
	SetDoNotStrip(true);
>;

// カメラ画角
float fFov : CAMERA_PARAM_FOV
<
	SetDoNotStrip(true);
>;

// カメラNear
float fNear : CAMERA_PARAM_NEAR
<
	SetDoNotStrip(true);
>;

// カメラFar
float fFar : CAMERA_PARAM_FAR
<
	SetDoNotStrip(true);
>;

////////////////////////////////////////////////////

float _ComputeWeight(float fScale, float fWeight)
{
	return saturate(0.5f - fScale * fWeight);
}

////////////////////////////////////////////////////
// マージ

// NOTE
// カラーバッファはA成分のみへの描き込み設定にしておくこと

float4 mainMerge(S_PE_PS_IN sIn) : COLOR
{
	float fZ = ConvertZ(smplZ, sIn.vUV);
	float fWeight = tex2D(smplWeight, float2(fZ, 0.5f)).a;
	return float4(fWeight, fWeight, fWeight, fWeight);
}

////////////////////////////////////////////////////
// 1/4 x 1/4 縮小

#ifdef __DOF_POINT__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn) : COLOR
#else
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn) : COLOR
#endif
{
	float4 vOut = (float4)0.0f;
	float fWeight = 0.0f;

#ifdef __DOF_POINT__
	for (int i = 0; i < NUM_SAMPLING_DOWNSCALE_4x4_POINT; i++) {
#else
	for (int i = 0; i < NUM_SAMPLING_DOWNSCALE_4x4_LINEAR; i++) {
#endif

		float4 vColor_0 = tex2D(
							smplScene,
							GetSamplePointFromDownScale4x4(i * 2 + 0, sIn));
		float4 vColor_1 = tex2D(
							smplScene,
							GetSamplePointFromDownScale4x4(i * 2 + 1, sIn));

		// NOTE
		// ０になるのを回避するため少し小さくする・・・
		float fW_0 = _ComputeWeight(0.49609f, vColor_0.a);
		float fW_1 = _ComputeWeight(0.49609f, vColor_1.a);

		vOut += vColor_0 * fW_0;
		vOut += vColor_1 * fW_1;

		fWeight += fW_0 + fW_1;
	}

	vOut /= fWeight;
	return vOut;
}

////////////////////////////////////////////////////
// ブラー

// NOTE
// 4pixel以上ボケている場合の対応
// 1/4x1/4縮小では4pixelまでしかボケない

#ifdef __DOF_POINT__
// ポイントフィルタ
float4 mainBlur(S_PE_PS_IN_9POINTSAMPLE_POINT sIn) : COLOR
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	float4 vOut = (float4)0.0f;
	float fWeight = 0.00001f;
	
	// 0
	float4 vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(0, sIn));
	float fW = _ComputeWeight(2.82843f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 1
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(1, sIn));
	fW = _ComputeWeight(2.0f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 2
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(2, sIn));
	fW = _ComputeWeight(2.82843f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 3
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(3, sIn));
	fW = _ComputeWeight(2.0f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 4 中心
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(4, sIn));
	fW = saturate(2.0f * vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 5
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(5, sIn));
	fW = _ComputeWeight(2.0f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 6
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(6, sIn));
	fW= _ComputeWeight(2.82843f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 7
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(7, sIn));
	fW = _ComputeWeight(2.0f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	// 8
	vColor = tex2D(smplDownScale4x4, GetSamplePointFrom9PointSample(8, sIn));
	fW = _ComputeWeight(2.82843f, vColor.a);
	vOut += vColor * fW;
	fWeight += fW;

	vOut /= fWeight;
	return vOut;
}
#else	// #ifdef __DOF_POINT__
// リニアフィルタ
float4 mainBlur(S_PE_PS_IN_9POINTSAMPLE_LINEAR sIn) : COLOR
{
	// NOTE
	// +---+---+---+
	// |   |   3   |
	// +-0-+---+---+
	// |   | x |   |
	// +---+---+-2-+
	// |   1   |   |
	// +---+---+---+

	// 中心
	float4 vOut = tex2D(smplDownScale4x4, GetCenterUVFrom9PointSample(sIn));

	float fWeight = saturate(2.0f * vOut.a + 0.00001f);
	vOut *= fWeight;
	
	for (int i = 0; i < NUM_SAMPLING_9POINTSAMPLE_LINEAR; i++) {
		float4 vColor_0 = tex2D(
							smplDownScale4x4,
							GetSamplePointFrom9PointSample(i * 2 + 0, sIn));
		float4 vColor_1 = tex2D(
							smplDownScale4x4,
							GetSamplePointFrom9PointSample(i * 2 + 1, sIn));

		// NOTE
		// 2.23607f = sqrt(4 * 4 + 2 * 2) / 2 = sqrt(20) / 2 = sqrt(5)
		float fW_0 = _ComputeWeight(2.23607f, vColor_0.a);
		float fW_1 = _ComputeWeight(2.23607f, vColor_1.a);

		vOut += vColor_0 * fW_0;
		vOut += vColor_1 * fW_1;

		fWeight += fW_0 + fW_1;
	}

	vOut /= fWeight;
	return vOut;
}
#endif	// #ifdef __DOF_POINT__

////////////////////////////////////////////////////
// 最終描画

float4 mainFinal(S_PE_PS_IN sIn) : COLOR
{
	float4 vHigh = tex2D(smplScene, sIn.vUV);
	float4 vLow = tex2D(smplBlur, sIn.vUV);

	float fW_0 = vHigh.a;
	float fW_1 = _ComputeWeight(2.0f, vHigh.a);

	float4 vOut = (vHigh * fW_0 + vLow * fW_1) / (fW_0 + fW_1 + 0.00001f);

	return vOut;
}

float4 mainFinalDebug(S_PE_PS_IN sIn) : COLOR
{
#if 0
	float4 vHigh = tex2D(smplScene, sIn.vUV);

	float fOut = vHigh.a;

	return float4(fOut, fOut, fOut, fOut);
#elif 0
	float4 vOut = tex2D(smplWeight, sIn.vUV);
	return vOut;
#elif 0
	float fOut = ConvertZ(smplZ, sIn.vUV);
	return float4(fOut, fOut, fOut, fOut);
#else
	float4 vHigh = tex2D(smplScene, sIn.vUV);

	float fW_0 = vHigh.a;
	float fW_1 = _ComputeWeight(2.0f, vHigh.a);

	float fOut = fW_0 / (fW_0 + fW_1 + 0.00001f);

	return float4(fOut, fOut, fOut, fOut);
#endif
}

////////////////////////////////////////////////////

// 共通処理
DEFINE_PASSES(CommonPasses)
{
	// 合成
	pass passMerge
	<
		BeginScene(texScene);
		SetFunctorName("PostEffectFunctorDOFMerge");
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainMerge);
	}

	// 1/4 x 1/4 縮小
	pass passDownScale4x4
	<
		BeginScene(texDownScale4x4);
#ifdef __DOF_POINT__
		SetTextureOffset(g_vDownScale4x4SampleOffset_Point);
#else
		SetTextureOffset(g_vDownScale4x4SampleOffset_Linear);
#endif
	>
	{
#ifdef __DOF_POINT__
		SetVertexShader(vs_2_a, VSSampling_16);
#else
		SetVertexShader(vs_2_a, VSSampling_4);
#endif
		SetPixelShader(ps_2_a, mainDownScale4x4);
	}

	// ブラー
	pass passBlur
	<
		BeginScene(texBlur);
#ifdef __DOF_POINT__
		SetTextureOffset(g_v9PointSampleOffset_Point);
#else
		SetTextureOffset(g_v9PointSampleOffset_Linear);
#endif
	>
	{
#ifdef __DOF_POINT__
		SetVertexShader(vs_2_a, VSSampling_9);
#else
		SetVertexShader(vs_2_a, VSSampling_5);
#endif
		SetPixelShader(ps_2_a, mainBlur);
	}
}

////////////////////////////////////////////////////

technique t0
{	
	EmbedDefinePasses(CommonPasses)

	// 最終描画
	pass passFinalDebug
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainFinal);
	}
}

// デバッグ用
technique t1
{
	// 合成
	pass passMerge
	<
		BeginScene(texScene);
		SetFunctorName("PostEffectFunctorDOFMerge");
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainMerge);
	}

	// 最終描画
	pass passFinalDebug
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainFinalDebug);
	}
}
