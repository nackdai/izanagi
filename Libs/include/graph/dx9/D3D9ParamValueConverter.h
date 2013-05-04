#ifdef __IZ_DX9__
#if !defined(__IZANAGI_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__)
#define __IZANAGI_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__

#include "izDefs.h"
#include "izDX9Defs.h"
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
    * IZANAGIのパラメータ -> D3D9のパラメータ
    */
    class CD3D9ParamValueConverter {
    private:
        CD3D9ParamValueConverter();
        ~CD3D9ParamValueConverter();

        NO_COPIABLE(CD3D9ParamValueConverter);

    public:
        // フィルモード
        FUNC_CONV_PARAM_VAL(FillMode, D3DFILLMODE, E_GRAPH_FILL_MODE);

        // ブレンド方法
        FUNC_CONV_PARAM_VAL(BlendOp, D3DBLENDOP, E_GRAPH_BLEND_OP);

        // ブレンド係数
        FUNC_CONV_PARAM_VAL(Blend, D3DBLEND, E_GRAPH_BLEND);

        // カリングモード
        FUNC_CONV_PARAM_VAL(Cull, D3DCULL, E_GRAPH_CULL);

        // 比較処理
        FUNC_CONV_PARAM_VAL(Cmp, D3DCMPFUNC, E_GRAPH_CMP_FUNC);

        // テクスチャアドレッシング
        FUNC_CONV_PARAM_VAL(TexAddr, D3DTEXTUREADDRESS, E_GRAPH_TEX_ADDRESS);

        // テクスチャフィルタ
        FUNC_CONV_PARAM_VAL(TexFilter, D3DTEXTUREFILTERTYPE, E_GRAPH_TEX_FILTER);

        // サンプラーステートタイプ
        FUNC_CONV_PARAM_VAL(SamplerStateType, D3DSAMPLERSTATETYPE, E_GRAPH_SAMPLER_STATE_TYPE);

        // ピクセルフォーマット
        FUNC_CONV_PARAM_VAL(PixelFormat, D3DFORMAT, E_GRAPH_PIXEL_FMT);

        // インデックスバッファフォーマット
        FUNC_CONV_PARAM_VAL(IdxBufFmt, D3DFORMAT, E_GRAPH_INDEX_BUFFER_FMT);

        // プリミティブタイプ
        FUNC_CONV_PARAM_VAL(PrimType, D3DPRIMITIVETYPE, E_GRAPH_PRIM_TYPE);

        // キューブマップフェイス
        FUNC_CONV_PARAM_VAL(CubeFace, D3DCUBEMAP_FACES, E_GRAPH_CUBE_TEX_FACE);

        // 頂点宣言のデータ型
        FUNC_CONV_PARAM_VAL(VtxDeclType, D3DDECLTYPE, E_GRAPH_VTX_DECL_TYPE);

        // 頂点データの使い方
        FUNC_CONV_PARAM_VAL(VtxDeclUsage, D3DDECLUSAGE, E_GRAPH_VTX_DECL_USAGE);
    };

    typedef CD3D9ParamValueConverter CTargetParamValueConverter;
}   // namespace graph
}   // namespace izanagi


#endif  // #if !defined(__IZANAGI_GRAPH_D3D9_PARAM_VALUE_CONVERTER_H__)
#endif  // #ifdef __IZ_DX9__
