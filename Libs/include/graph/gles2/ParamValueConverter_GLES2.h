#if !defined(__IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_GLES2_H__)
#define __IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_GLES2_H__

#include "izDefs.h"
#include "izGLES2Defs.h"
#include "graph/GraphDefs.h"

#define FUNC_ABSTRACT_TO_TARGET(p, ret, arg)    ret ConvAbstractToTarget_##p(arg v)
#define FUNC_TARGET_TO_ABSTRACT(p, ret, arg)    ret ConvTargetToAbstract_##p(arg v)

#define FUNC_CONV_PARAM_VAL(p, target, abst)\
    static FUNC_ABSTRACT_TO_TARGET(p, target, abst);\
    static FUNC_TARGET_TO_ABSTRACT(p, abst, target)

namespace izanagi
{
namespace graph
{
    /**
    * IZANAGIのパラメータ -> GLES2のパラメータ
    */
    class CParamValueConverterGLES2
    {
    private:
        CParamValueConverterGLES2();
        ~CParamValueConverterGLES2();

        NO_COPIABLE(CParamValueConverterGLES2);

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
        static void ConvAbstractToTarget_PixelFormat(
            E_GRAPH_PIXEL_FMT fmt,
            GLenum& glFormat,
            GLenum& glType);

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

    typedef CParamValueConverterGLES2 CTargetParamValueConverter;
}   // namespace graph
}   // namespace izanagi


#endif  // #if !defined(__IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_GLES2_H__)
