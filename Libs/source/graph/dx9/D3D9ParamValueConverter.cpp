#include "graph/dx9/D3D9ParamValueConverter.h"

namespace izanagi
{
namespace graph
{
    #define DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst)    target CD3D9ParamValueConverter::ConvAbstractToTarget_##p(abst v)
    #define DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst)    abst CD3D9ParamValueConverter::ConvTargetToAbstract_##p(target v)

    #define GET_TARGET_TO_ABSTRACT(tbl, v, abst)\
        IZ_INT ret = -1;\
        for (IZ_UINT i = 0; i < COUNTOF(tbl); ++i) {\
            if (tbl[i] == v) {\
                ret = i;\
                break;\
            }\
        }\
        IZ_ASSERT(ret >= 0);\
        return static_cast<abst>(ret)

    #define DEF_FUNC_CONV_PARAM_VAL(p, target, abst, tbl)\
        DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst) { IZ_ASSERT(v < COUNTOF(tbl)); return tbl[v]; }\
        DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst) { GET_TARGET_TO_ABSTRACT(tbl, v, abst); }

    //////////////////////////////////////////
    // フィルモード

    namespace {
        D3DFILLMODE FillModeTbl[] = {
            D3DFILL_POINT,
            D3DFILL_WIREFRAME,
            D3DFILL_SOLID,
        };
        IZ_C_ASSERT(COUNTOF(FillModeTbl) == E_GRAPH_FILL_MODE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(FillMode, D3DFILLMODE, E_GRAPH_FILL_MODE, FillModeTbl)

    //////////////////////////////////////////
    // ブレンド方法

    namespace {
        D3DBLENDOP BlendOpTbl[] = {
            D3DBLENDOP_ADD,
            D3DBLENDOP_SUBTRACT,
            D3DBLENDOP_REVSUBTRACT,
#if 0
            D3DBLENDOP_MIN,
            D3DBLENDOP_MAX,
#endif
        };
        IZ_C_ASSERT(COUNTOF(BlendOpTbl) == E_GRAPH_BLEND_OP_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(BlendOp, D3DBLENDOP, E_GRAPH_BLEND_OP, BlendOpTbl)

    //////////////////////////////////////////
    // ブレンド係数

    namespace {
        D3DBLEND BlendTbl[] = {
            D3DBLEND_ZERO,
            D3DBLEND_ONE,
            D3DBLEND_SRCCOLOR,
            D3DBLEND_INVSRCCOLOR,
            D3DBLEND_SRCALPHA,
            D3DBLEND_INVSRCALPHA,
            D3DBLEND_DESTALPHA,
            D3DBLEND_INVDESTALPHA,
            D3DBLEND_DESTCOLOR,
            D3DBLEND_INVDESTCOLOR,
            D3DBLEND_SRCALPHASAT,
        };
        IZ_C_ASSERT(COUNTOF(BlendTbl) == E_GRAPH_BLEND_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Blend, D3DBLEND, E_GRAPH_BLEND, BlendTbl)

    //////////////////////////////////////////
    // カリングモード

    namespace {
        D3DCULL CullTbl[] = {
            D3DCULL_NONE,
            D3DCULL_CW,
            D3DCULL_CCW,
        };
        IZ_C_ASSERT(COUNTOF(CullTbl) == E_GRAPH_CULL_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Cull, D3DCULL, E_GRAPH_CULL, CullTbl)

    //////////////////////////////////////////
    // 比較処理

    namespace {
        D3DCMPFUNC CmpTbl[] = {
            D3DCMP_NEVER,
            D3DCMP_LESS,
            D3DCMP_EQUAL,
            D3DCMP_LESSEQUAL,
            D3DCMP_GREATER,
            D3DCMP_NOTEQUAL,
            D3DCMP_GREATEREQUAL,
            D3DCMP_ALWAYS,
        };
        IZ_C_ASSERT(COUNTOF(CmpTbl) == E_GRAPH_CMP_FUNC_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Cmp, D3DCMPFUNC, E_GRAPH_CMP_FUNC, CmpTbl)

    //////////////////////////////////////////
    // テクスチャアドレッシング

    namespace {
        D3DTEXTUREADDRESS TexAddrTbl[] = {
            D3DTADDRESS_WRAP,
            D3DTADDRESS_MIRROR,
            D3DTADDRESS_CLAMP,
#if 0
            D3DTADDRESS_BORDER,
            D3DTADDRESS_MIRRORONCE,
#endif
        };
        IZ_C_ASSERT(COUNTOF(TexAddrTbl) == E_GRAPH_TEX_ADDRESS_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(TexAddr, D3DTEXTUREADDRESS, E_GRAPH_TEX_ADDRESS, TexAddrTbl)

    //////////////////////////////////////////
    // テクスチャフィルタ

    namespace {
        D3DTEXTUREFILTERTYPE TexFilterTbl[] = {
            D3DTEXF_NONE,
            D3DTEXF_POINT,
            D3DTEXF_LINEAR,
            D3DTEXF_ANISOTROPIC,

#if 0
            D3DTEXF_PYRAMIDALQUAD,
            D3DTEXF_GAUSSIANQUAD,
#endif
        };
        IZ_C_ASSERT(COUNTOF(TexFilterTbl) == E_GRAPH_TEX_FILTER_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(TexFilter, D3DTEXTUREFILTERTYPE, E_GRAPH_TEX_FILTER, TexFilterTbl)

    //////////////////////////////////////////
    // サンプラーステートタイプ

    namespace {
        D3DSAMPLERSTATETYPE SamplerStateTypeTbl[] = {
            D3DSAMP_ADDRESSU,
            D3DSAMP_ADDRESSV,
            D3DSAMP_ADDRESSW,
            D3DSAMP_MAGFILTER,
            D3DSAMP_MINFILTER,
            D3DSAMP_MIPFILTER,
        };
        IZ_C_ASSERT(COUNTOF(SamplerStateTypeTbl) == E_GRAPH_SAMPLER_STATE_TYPE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(SamplerStateType, D3DSAMPLERSTATETYPE, E_GRAPH_SAMPLER_STATE_TYPE, SamplerStateTypeTbl)

    //////////////////////////////////////////
    // ピクセルフォーマット

    namespace {
        D3DFORMAT PixelFormatTbl[] = {
            D3DFMT_A8R8G8B8,
            D3DFMT_A8B8G8R8,
            D3DFMT_A4R4G4B4,
            D3DFMT_A2R10G10B10,
            D3DFMT_A8,
            D3DFMT_R32F,
            D3DFMT_A16B16G16R16F,
            D3DFMT_A32B32G32R32F,
            D3DFMT_DXT1,
            D3DFMT_DXT3,
            D3DFMT_DXT5,
            D3DFMT_X8R8G8B8,
            D3DFMT_D24S8,
        };
        IZ_C_ASSERT(COUNTOF(PixelFormatTbl) == E_GRAPH_PIXEL_FMT_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(PixelFormat, D3DFORMAT, E_GRAPH_PIXEL_FMT, PixelFormatTbl)

    //////////////////////////////////////////
    // インデックスバッファフォーマット

    namespace {
        D3DFORMAT IdxBufFmtTbl[] = {
            D3DFMT_INDEX16,
            D3DFMT_INDEX32,
        };
        IZ_C_ASSERT(COUNTOF(IdxBufFmtTbl) == E_GRAPH_INDEX_BUFFER_FMT_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(IdxBufFmt, D3DFORMAT, E_GRAPH_INDEX_BUFFER_FMT, IdxBufFmtTbl)

    //////////////////////////////////////////
    // プリミティブタイプ

    namespace {
        D3DPRIMITIVETYPE PrimTypeTbl[] = {
            D3DPT_POINTLIST,
            D3DPT_LINELIST,
            D3DPT_LINESTRIP,
            D3DPT_TRIANGLELIST,
            D3DPT_TRIANGLESTRIP,
            D3DPT_TRIANGLEFAN,
        };
        IZ_C_ASSERT(COUNTOF(PrimTypeTbl) == E_GRAPH_PRIM_TYPE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(PrimType, D3DPRIMITIVETYPE, E_GRAPH_PRIM_TYPE, PrimTypeTbl)

    //////////////////////////////////////////
    // キューブフェイス

    namespace {
        D3DCUBEMAP_FACES CubeFaceTbl[] = {
            D3DCUBEMAP_FACE_POSITIVE_X,
            D3DCUBEMAP_FACE_NEGATIVE_X,
            D3DCUBEMAP_FACE_POSITIVE_Y,
            D3DCUBEMAP_FACE_NEGATIVE_Y,
            D3DCUBEMAP_FACE_POSITIVE_Z,
            D3DCUBEMAP_FACE_NEGATIVE_Z,
        };
        IZ_C_ASSERT(COUNTOF(CubeFaceTbl) == E_GRAPH_CUBE_TEX_FACE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(CubeFace, D3DCUBEMAP_FACES, E_GRAPH_CUBE_TEX_FACE, CubeFaceTbl);

    //////////////////////////////////////////
    // 頂点宣言のデータ型

    namespace {
        D3DDECLTYPE VtxDeclTypeTbl[] = {
            D3DDECLTYPE_FLOAT1,
            D3DDECLTYPE_FLOAT2,
            D3DDECLTYPE_FLOAT3,
            D3DDECLTYPE_FLOAT4,
            D3DDECLTYPE_D3DCOLOR,
                             
            D3DDECLTYPE_UBYTE4,
            D3DDECLTYPE_SHORT2,
            D3DDECLTYPE_SHORT4,

            D3DDECLTYPE_UBYTE4N,
            D3DDECLTYPE_SHORT2N,
            D3DDECLTYPE_SHORT4N,
            D3DDECLTYPE_USHORT2N,
            D3DDECLTYPE_USHORT4N,
            D3DDECLTYPE_UDEC3,
            D3DDECLTYPE_DEC3N,
            D3DDECLTYPE_FLOAT16_2,
            D3DDECLTYPE_FLOAT16_4,
            D3DDECLTYPE_UNUSED,
        };
        IZ_C_ASSERT(COUNTOF(VtxDeclTypeTbl) == E_GRAPH_VTX_DECL_TYPE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(VtxDeclType, D3DDECLTYPE, E_GRAPH_VTX_DECL_TYPE, VtxDeclTypeTbl);

    //////////////////////////////////////////
    // 頂点データの使い方

    namespace {
        D3DDECLUSAGE VtxDeclUsageTbl[] = {
            D3DDECLUSAGE_POSITION,
            D3DDECLUSAGE_BLENDWEIGHT, 
            D3DDECLUSAGE_BLENDINDICES,
            D3DDECLUSAGE_NORMAL,      
            D3DDECLUSAGE_PSIZE,       
            D3DDECLUSAGE_TEXCOORD,    
            D3DDECLUSAGE_TANGENT,     
            D3DDECLUSAGE_BINORMAL,    
            D3DDECLUSAGE_TESSFACTOR,  
            D3DDECLUSAGE_POSITIONT,   
            D3DDECLUSAGE_COLOR,       
            D3DDECLUSAGE_FOG,         
            D3DDECLUSAGE_DEPTH,       
            D3DDECLUSAGE_SAMPLE,      
        };
        IZ_C_ASSERT(COUNTOF(VtxDeclUsageTbl) == E_GRAPH_VTX_DECL_USAGE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(VtxDeclUsage, D3DDECLUSAGE, E_GRAPH_VTX_DECL_USAGE, VtxDeclUsageTbl);
}   // namespace graph
}   // namespace izanagi
