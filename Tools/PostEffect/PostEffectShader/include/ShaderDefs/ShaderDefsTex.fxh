#if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__

////////////////////////////////////////////////////////
// テクスチャ

//****************************************
// アノテーション

#define SetTexSize(w, h)		float2 Dimensions = {w, h}		// テクスチャサイズ
#define SetTexRatio(w, h)		float2 ViewportRatio = {w, h}	// スクリーンサイズ比率
#define SetTexFormat(fmt)		string Format = fmt				// ピクセルフォーマット
#define SetIsRenderTarget(b)	bool IsRenderTarget = b			// レンダーターゲットかどうか
#define SetIsDynamic(b)			bool IsDynamic = b				// 動的かどうか

// ピクセルフォーマット設定値
#define PIXEL_FORMAT_RGBA8		"A8R8G8B8"
#define PIXEL_FORMAT_RGBA16F	"A16B16G16R16F"
#define PIXEL_FORMAT_RGBA32F	"A32B32G32R32F"
#define PIXEL_FORAMT_R16F		"R16F"
#define PIXEL_FORAMT_R32F		"R32F"
#define PIXEL_FORMAT_A2RGB10	"A2R10G10B10"
#define PIXEL_FORMAT_A8			"A8"

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_DEFS_TEX_FXH__)
