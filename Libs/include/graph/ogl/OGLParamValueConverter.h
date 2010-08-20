#if !defined(__URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__)
#define __URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__

#include "unDefs.h"
#include "unD3DDefs.h"
#include "graph/GraphDefs.h"

#define FUNC_ABSTRACT_TO_TARGET(p, ret, arg)	ret ConvAbstractToTarget_##p(arg v)
#define FUNC_TARGET_TO_ABSTRACT(p, ret, arg)	ret ConvTargetToAbstract_##p(arg v)

#define FUNC_CONV_PARAM_VAL(p, target, abst)\
	static FUNC_ABSTRACT_TO_TARGET(p, target, abst);\
	static FUNC_TARGET_TO_ABSTRACT(p, abst, target)

#define UN_OGL_PIXEL_FMT(fmt, type)	(((fmt) << 16) | (type))
#define UN_OGL_GET_PIXEL_FMT(val)	(((fmt) & 0xff00) >> 16)
#define UN_OGL_GET_PIXEL_TYPE(val)	((fmt) & 0xff)

namespace uranus {
	/**
	* URANUSのパラメータ -> OGLのパラメータ
	*/
	class COGLParamValueConverter {
	private:
		COGLParamValueConverter();
		~COGLParamValueConverter();

		NO_COPIABLE(COGLParamValueConverter);

	public:
		// フィルモード
		FUNC_CONV_PARAM_VAL(FillMode, GLenum, E_GRAPH_FILL_MODE);

		// ブレンド方法
		FUNC_CONV_PARAM_VAL(BlendOp, GLenum, E_GRAPH_BLEND_OP);

		// ブレンド係数
		FUNC_CONV_PARAM_VAL(Blend, GLenum, E_GRAPH_BLEND);

		// カリングモード
		FUNC_CONV_PARAM_VAL(Cull, GLenum, E_GRAPH_CULL);

		// 比較処理
		FUNC_CONV_PARAM_VAL(Cmp, GLenum, E_GRAPH_CMP_FUNC);

		// テクスチャアドレッシング
		FUNC_CONV_PARAM_VAL(TexAddr, GLenum, E_GRAPH_TEX_ADDRESS);

		// テクスチャフィルタ
		FUNC_CONV_PARAM_VAL(TexFilter, GLenum, E_GRAPH_TEX_FILTER);

		// サンプラーステートタイプ
		FUNC_CONV_PARAM_VAL(SamplerStateType, GLenum, E_GRAPH_SAMPLER_STATE_TYPE);

		// ピクセルフォーマット
		FUNC_CONV_PARAM_VAL(PixelFormat, GLenum, E_GRAPH_PIXEL_FMT);

		// インデックスバッファフォーマット
		FUNC_CONV_PARAM_VAL(IdxBufFmt, GLenum, E_GRAPH_INDEX_BUFFER_FMT);

		// プリミティブタイプ
		FUNC_CONV_PARAM_VAL(PrimType, GLenum, E_GRAPH_PRIM_TYPE);

		// キューブマップフェイス
		FUNC_CONV_PARAM_VAL(CubeFace, GLenum, E_GRAPH_CUBE_TEX_FACE);

		// 頂点宣言のデータ型
		FUNC_CONV_PARAM_VAL(VtxDeclType, GLenum, E_GRAPH_VTX_DECL_TYPE);

		// 頂点データの使い方
		FUNC_CONV_PARAM_VAL(VtxDeclUsage, GLenum, E_GRAPH_VTX_DECL_USAGE);
	};

	UN_C_ASSERT(sizeof(GLenum) == sizeof(UN_UINT));

	typedef COGLParamValueConverter CParamValueConverter;
}	// namespace uranus


#endif	// #if !defined(__URANUS_GRAPH_OGL_PARAM_VALUE_CONVERTER_H__)
