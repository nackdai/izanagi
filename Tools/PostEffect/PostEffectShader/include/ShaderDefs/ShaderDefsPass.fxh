#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__

#include "ShaderDefsScript.fxh"
#include "ShaderDefsFunctor.fxh"

////////////////////////////////////////////////////////
// パス

//****************************************
// アノテーション

#if 0
// パスの関連付け設定
// ・ファンクタに複数のパスを持たせたいときに使用する。
// ・最大で４まで設定できる。
// ・自分自身を設定、二重登録は行うことができない。
#define SetConnectPass_1(pass0)							string ConnectPass = #pass0
#define SetConnectPass_2(pass0, pass1)					string ConnectPass = #pass0","#pass1
#define SetConnectPass_3(pass0, pass1, pass2)			string ConnectPass = #pass0","#pass1","#pass2
#define SetConnectPass_4(pass0, pass1, pass2, pass3)	string ConnectPass = #pass0","#pass1","#pass2","#pass3
#endif

// 出力先のシザリング設定
// ・ここに0以外の値が設定された場合は、シザリングを行うものとする。
// ・レンダーターゲットのサイズにinflateする形で設定する。
// ・レンダーターゲットが指定されない場合は、ここで設定された値は使用されない。
// ・x、y は[-32768 - 32767]の整数をセットすること。それ以外をセットしたときの動作は保障されない。
#define SetScissorRectInflate(x, y)		int2 ScissorRectInflate = {x, y}

// ソーステクスチャ矩形設定
// ・ソーステクスチャは、CPostEffectFunctorXXXX::GetMainSrcTexture で取得できるテクスチャのことである。
// ・ソーステクスチャ矩形のサイズにinflateする形で設定する。
// ・x、y は[-32768 - 32767]の整数をセットすること。それ以外をセットしたときの動作は保障されない。
#define SetSrcTexRectInflate(x, y)		int2 SrcTexRectInflate = {x, y}

// テクスチャオフセット
#define SetTextureOffset(offset)		string TextureOffset = #offset

//****************************************
// ステート

// NOTE
// ピクセルパイプラインレンダーステートのみ

#define SetAlphaBlendEnable(b)		AlphaBlendEnable = b
//#define SetAlphaFunc(func)			AlphaFunc = func
//#define SetAlphaRef(ref)			AlphaRef = ref
#define SetAlphaTestEnable(b)		AlphaTestEnable = b
#define SetBlendOp(op)				BlendOp = op
//#define SetDestBlend(blend)			DestBlend = blend
//#define SetSrcBlend(blend)			SrcBlend = blend
//#define SetAlphaBlend(op, src, dst)	SetBlendOp(op); SetSrcBlend(src); SetDestBlend(dst)

#if defined(__HLSL__)
#define SetAlphaFunc(func, ref)		AlphaFunc = func; AlphaRef = ref
#define SetBlendFunc(src, dst)		SrcBlend = src; DestBlend = dst
#else	// #if defined(__HLSL__)
#define SetAlphaFunc(func, ref)		AlphaFunc = float2(func, ref)
#define SetBlendFunc(src, dst)		BlendFunc = int2(src, dst)
#endif	// #if defined(__HLSL__)

#define SetAlphaBlend(op, src, dst)	SetBlendOp(op); SetBlendFunc(src, dst)

// 比較処理
#if defined(__HLSL__)
#define CMP_NEVER				NEVER			// 常にテストは失敗. 
#define CMP_LESS				LESS			// NEW < OLD なら成功. 
#define CMP_EQUAL				EQUAL			// NEW == OLD なら成功. 
#define CMP_LESSEQUAL			LESSEQUAL		// NEW <= OLD なら成功. 
#define CMP_GREATER				GREATER			// NEW > OLD なら成功. 
#define CMP_NOTEQUAL			NOTEQUAL		// NEW != OLD なら成功. 
#define CMP_GREATEREQUAL		GREATEREQUAL	// NEW >= OLD なら成功. 
#define CMP_ALWAYS				ALWAYS			// 常にテストは成功. 
#else	// #if defined(__HLSL__)
#define CMP_NEVER				Never			// 常にテストは失敗. 
#define CMP_LESS				Less			// NEW < OLD なら成功. 
#define CMP_EQUAL				Equal			// NEW == OLD なら成功. 
#define CMP_LESSEQUAL			LEqual			// NEW <= OLD なら成功. 
#define CMP_GREATER				Greater			// NEW > OLD なら成功. 
#define CMP_NOTEQUAL			NotEqual		// NEW != OLD なら成功. 
#define CMP_GREATEREQUAL		GEqual			// NEW >= OLD なら成功. 
#define CMP_ALWAYS				Always			// 常にテストは成功. 
#endif	// #if defined(__HLSL__)

// ブレンディング処理
#if defined(__HLSL__)
#define BLENDOP_ADD				ADD
#define BLENDOP_SUBTRACT		SUBTRACT
#define BLENDOP_REVSUBTRACT		REVSUBTRACT
#define BLENDOP_MIN				MIN
#define BLENDOP_MAX				MAX
#else	// #if defined(__HLSL__)
#define BLENDOP_ADD				Add
#define BLENDOP_SUBTRACT		Subtract
#define BLENDOP_REVSUBTRACT		ReverseSubtract
#define BLENDOP_MIN				Min
#define BLENDOP_MAX				Max
#endif	// #if defined(__HLSL__)

// ブレンディングモード
#if defined(__HLSL__)
#define BLEND_ZERO				ZERO			// (0, 0, 0, 0)
#define BLEND_ONE				ONE				// (1, 1, 1, 1)
#define BLEND_SRCCOLOR			SRCCOLOR		// (Rs, Gs, Bs, As)
#define BLEND_INVSRCCOLOR		INVSRCCOLOR		// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHA			SRCALPHA		// (As, As, As, As)
#define BLEND_INVSRCALPHA		INVSRCALPHA		// (1 - As, 1 - As, 1 - As, 1 - As)
#define BLEND_DESTALPHA			DESTALPHA		// (Ad, Ad, Ad, Ad)
#define BLEND_INVDESTALPHA		INVDESTALPHA	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
#define BLEND_DESTCOLOR			DESTCOLOR		// (Rd, Gd, Bd, Ad)
#define BLEND_INVDESTCOLOR		INVDESTCOLOR	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHASAT		SRCALPHASAT		// (f, f, f, 1)	(f = min(As, 1 - Ad)
#else	// #if defined(__HLSL__)
#define BLEND_ZERO				Zero				// (0, 0, 0, 0)
#define BLEND_ONE				One					// (1, 1, 1, 1)
#define BLEND_SRCCOLOR			SrcColor			// (Rs, Gs, Bs, As)
#define BLEND_INVSRCCOLOR		OneMinusSrcColor	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHA			SrcAlpha			// (As, As, As, As)
#define BLEND_INVSRCALPHA		OneMinusSrcAlpha	// (1 - As, 1 - As, 1 - As, 1 - As)
#define BLEND_DESTALPHA			DstAlpha			// (Ad, Ad, Ad, Ad)
#define BLEND_INVDESTALPHA		OneMinusDstAlpha	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
#define BLEND_DESTCOLOR			DestColor			// (Rd, Gd, Bd, Ad)
#define BLEND_INVDESTCOLOR		OneMinusDestColor	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
#define BLEND_SRCALPHASAT		SrcAlphaSaturate	// (f, f, f, 1)	(f = min(As, 1 - Ad)
#endif	// #if defined(__HLSL__)

// アルファブレンド設定
#define SetAlphaBlend_Normal()		SetAlphaBlend(BLENDOP_ADD, BLEND_SRCALPHA, BLEND_INVSRCALPHA)	// 通常
#define SetAlphaBlend_CsAs_Cd()		SetAlphaBlend(BLENDOP_ADD, BLEND_SRCALPHA, BLEND_ONE)			// 加算(アルファあり)
#define SetAlphaBlend_Cs_Cd()		SetAlphaBlend(BLENDOP_ADD, BLEND_ONE, BLEND_ONE)				// 加算(アルファなし)
#define SetAlphaBlend_NCsAs_Cd()	SetAlphaBlend(BLENDOP_REVSUBTRACT, BLEND_SRCALPHA, BLEND_ONE)	// 減算(アルファあり)
#define SetAlphaBlend_NCs_Cd()		SetAlphaBlend(BLENDOP_REVSUBTRACT, BLEND_ONE, BLEND_ONE)		// 減算(アルファなし)

// 頂点シェーダ設定
#define SetVertexShader(profile, EntryVS)	VertexShader = compile profile EntryVS()

#if 0
#define SetVertexShaderDefault(profile)		SetVertexShader(profile, VSDefault)

#define SetVertexShaderSampling(profile, num)	SetVertexShader(profile, VSSampling_##num)
#define SetVertexShaderSampling_1(profile)		SetVertexShaderSampling(profile, 1)
#define SetVertexShaderSampling_4(profile)		SetVertexShaderSampling(profile, 4)
#define SetVertexShaderSampling_8(profile)		SetVertexShaderSampling(profile, 8)
#define SetVertexShaderSampling_16(profile)		SetVertexShaderSampling(profile, 16)
#define SetVertexShaderSampling_5(profile)		SetVertexShaderSampling(profile, 5)
#define SetVertexShaderSampling_7(profile)		SetVertexShaderSampling(profile, 7)
#define SetVertexShaderSampling_9(profile)		SetVertexShaderSampling(profile, 9)
#define SetVertexShaderSampling_13(profile)		SetVertexShaderSampling(profile, 13)

#define SetVSDownScale4x4_Linear(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale4x4_Point(profile)		SetVertexShaderSampling_16(profile)
#define SetVSDownScale2x2_Linear(profile)		SetVertexShaderSampling_1(profile)
#define SetVSDownScale2x2_Point(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale3x3_Linear(profile)		SetVertexShaderSampling_4(profile)
#define SetVSDownScale3x3_Point(profile)		SetVertexShaderSampling_9(profile)
#define SetVSBloomBlurV_Linear(profile)			SetVertexShaderSampling_7(profile)	
#define SetVSBloomBlurV_Point(profile)			SetVertexShaderSampling_13(profile)
#define SetVSBloomBlurH_Linear(profile)			SetVertexShaderSampling_7(profile)
#define SetVSBloomBlurH_Point(profile)			SetVertexShaderSampling_13(profile)

#define SetVSGaussBlur5x5(profile)				SetVertexShaderSampling_13(profile)
#define SetVSRenderStar(profile)				SetVertexShaderSampling_8(profile)

#define SetVS9PointSample_Linear(profile)		SetVertexShaderSampling_5(profile)
#define SetVS9PointSample_Point(profile)		SetVertexShaderSampling_9(profile)
#endif

// ピクセルシェーダ設定
#define SetPixelShader(profile, EntryPS)	PixelShader = compile profile EntryPS()

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_PASS_FXH__)
