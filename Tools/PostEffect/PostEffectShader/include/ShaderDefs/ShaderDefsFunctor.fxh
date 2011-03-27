#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__

////////////////////////////////////////////////////////
// ファンクタ設定

// ファンクタ名セット
#define SetFunctorName(name)	string FunctorName = name

// ファンクタ引数セット
#define SetFunctorArgsF_1(v0)				float FunctorArgsF = v0
#define SetFunctorArgsF_2(v0, v1)			float2 FunctorArgsF = {v0, v1};
#define SetFunctorArgsF_3(v0, v1, v2)		float3 FunctorArgsF = {v0, v1, v2};
#define SetFunctorArgsF_4(v0, v1, v2, v3)	float4 FunctorArgsF = {v0, v1, v2, v3};
#define SetFunctorArgsS_1(v0)				string FunctorArgsS = #v0
#define SetFunctorArgsS_2(v0, v1)			string FunctorArgsS = #v0","#v1
#define SetFunctorArgsS_3(v0, v1, v2)		string FunctorArgsS = #v0","#v1","#v2
#define SetFunctorArgsS_4(v0, v1, v2, v3)	string FunctorArgsS = #v0","#v1","#v2","#v3

#if 0
/********************************************
   各ファンクタIDセット（要ファンクタ引数）
*********************************************/

// 繰り返し縮小
#define SetFunctorLoopDownScale(cnt)\
	SetFunctorName("PostEffectFunctorLoopDownScale");\
	SetFunctorArgsF_1(cnt)

// テクスチャ切り替え
#define SetFunctorSwitchTex(tex0, tex1)\
	SetFunctorName("PostEffectFunctorSwitchTex");\
	SetFunctorArgsS_2(tex0, tex1)
#endif

#if 0
////////////////////////////////////////////////////////
// 複数パスファンクタ

// 複数パスファンクタ定義開始
#define MULTI_PASS_FUNCTOR(name, functor_name)\
	technique name\
	<\
		bool IsMultiPassFunctor = true;\
		SetFunctorName(functor_name);\
	>

// 複数パスファンクタ埋め込み
#define EmbedMultiPassFunctor(name)\
	pass name\
	<\
		bool IsEmbedMultiPassFunctor = true;\
	> {}
#endif

#if 0
////////////////////////////////////////////////////////
// パス外部記述

// カテゴリ的にここがいいのかどうかは分からんが・・・

// パスの記述をテクニックの外部に記述することでコードを読みやすくするのに使う

// 複数パスパック定義開始
#define DEFINE_PASSES(name)\
	technique name\
	<\
		bool IsDefinePasses = true;\
	>

// 複数パスパック埋め込み
#define EmbedDefinePasses(name)\
	pass name\
	<\
		bool IsEmbedDefinePasses = true;\
	> {}
#endif

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_FUNCTOR_ID_FXH__)
