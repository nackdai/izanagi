#include "graph/gles2/ParamValueConverter_GLES2.h"

namespace izanagi
{
namespace graph
{
    #define DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst)    target CParamValueConverterGLES2::ConvAbstractToTarget_##p(abst v)
    #define DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst)    abst CParamValueConverterGLES2::ConvTargetToAbstract_##p(target v)

    #define GET_TARGET_TO_ABSTRACT(tbl, v, abst)\
        IZ_INT ret = -1;\
        for (IZ_UINT i = 0; i < COUNTOF(tbl); ++i) {\
            if (tbl[i] == v) {\
                ret = i;\
                break;\
            }\
        }\
        IZ_ASSERT(ret > 0);\
        return static_cast<abst>(ret)

    #define DEF_FUNC_CONV_PARAM_VAL(p, target, abst, tbl)\
        DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst) { IZ_ASSERT(v < COUNTOF(tbl)); return tbl[v]; }\
        DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst) { GET_TARGET_TO_ABSTRACT(tbl, v, abst); }

    //////////////////////////////////////////
    // フィルモード

    // Nothing...

    //////////////////////////////////////////
    // ブレンド方法

    namespace {
        GLenum BlendOpTbl[] = {
            GL_FUNC_ADD,
            GL_FUNC_SUBTRACT,
            GL_FUNC_REVERSE_SUBTRACT,
        };
        IZ_C_ASSERT(COUNTOF(BlendOpTbl) == E_GRAPH_BLEND_OP_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(BlendOp, GLenum, E_GRAPH_BLEND_OP, BlendOpTbl)

    //////////////////////////////////////////
    // ブレンド係数

    namespace {
        GLenum BlendTbl[] = {
            GL_ZERO,
            GL_ONE,
            GL_SRC_COLOR,
            GL_ONE_MINUS_SRC_COLOR,
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA,
            GL_DST_ALPHA,
            GL_ONE_MINUS_DST_ALPHA,
            GL_DST_COLOR,
            GL_ONE_MINUS_DST_COLOR,
            GL_SRC_ALPHA_SATURATE,
        };
        IZ_C_ASSERT(COUNTOF(BlendTbl) == E_GRAPH_BLEND_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Blend, GLenum, E_GRAPH_BLEND, BlendTbl)

    //////////////////////////////////////////
    // カリングモード

    // NOTE
    // izanagiでは左手座標系なので
    // カリングの標準はCounterClockWiseにする
    namespace {
        GLenum CullTbl[] = {
            GL_FRONT_AND_BACK,
            GL_BACK,    // CW
            GL_FRONT,   // CCW
        };
        IZ_C_ASSERT(COUNTOF(CullTbl) == E_GRAPH_CULL_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Cull, GLenum, E_GRAPH_CULL, CullTbl)

    //////////////////////////////////////////
    // 比較処理

    namespace {
        GLenum CmpTbl[] = {
            GL_NEVER,
            GL_LESS,
            GL_EQUAL,
            GL_LEQUAL,
            GL_GREATER,
            GL_NOTEQUAL,
            GL_GEQUAL,
            GL_ALWAYS,
        };
        IZ_C_ASSERT(COUNTOF(CmpTbl) == E_GRAPH_CMP_FUNC_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(Cmp, GLenum, E_GRAPH_CMP_FUNC, CmpTbl)

    //////////////////////////////////////////
    // テクスチャアドレッシング

    namespace {
        GLenum TexAddrTbl[] = {
            GL_REPEAT,
            GL_MIRRORED_REPEAT,
            GL_CLAMP_TO_EDGE,
        };
        IZ_C_ASSERT(COUNTOF(TexAddrTbl) == E_GRAPH_TEX_ADDRESS_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(TexAddr, GLenum, E_GRAPH_TEX_ADDRESS, TexAddrTbl)

    //////////////////////////////////////////
    // テクスチャフィルタ

    namespace {
        GLenum TexFilterTbl[] = {
            0,
            GL_NEAREST,
            GL_LINEAR,
            0,
        };
        IZ_C_ASSERT(COUNTOF(TexFilterTbl) == E_GRAPH_TEX_FILTER_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(TexFilter, GLenum, E_GRAPH_TEX_FILTER, TexFilterTbl)

    //////////////////////////////////////////
    // サンプラーステートタイプ

    namespace {
        GLenum SamplerStateTypeTbl[] = {
            GL_TEXTURE_WRAP_S,
            GL_TEXTURE_WRAP_T,
            0,
            GL_TEXTURE_MAG_FILTER,
            GL_TEXTURE_MIN_FILTER,
            0,
        };
        IZ_C_ASSERT(COUNTOF(SamplerStateTypeTbl) == E_GRAPH_SAMPLER_STATE_TYPE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(SamplerStateType, GLenum, E_GRAPH_SAMPLER_STATE_TYPE, SamplerStateTypeTbl)

    //////////////////////////////////////////
    // ピクセルフォーマット

    void CParamValueConverterGLES2::ConvAbstractToTarget_PixelFormat(
        E_GRAPH_PIXEL_FMT fmt,
        GLenum& glFormat,
        GLenum& glType)
    {
        switch (fmt)
        {
        case E_GRAPH_PIXEL_FMT_RGBA8:
            glFormat = GL_RGBA;
            glType = GL_UNSIGNED_BYTE;
            break;
        case E_GRAPH_PIXEL_FMT_RGBA4:
            glFormat = GL_RGBA;
            glType = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
        case E_GRAPH_PIXEL_FMT_A8:
            glFormat = GL_ALPHA;
            glType = GL_UNSIGNED_BYTE;
            break;
        case E_GRAPH_PIXEL_FMT_RGBX8:
            glFormat = GL_RGB;
            glType = GL_UNSIGNED_BYTE;
            break;
        case E_GRAPH_PIXEL_FMT_BGRA8:
        case E_GRAPH_PIXEL_FMT_RGB10A2:
        case E_GRAPH_PIXEL_FMT_R32F:
        case E_GRAPH_PIXEL_FMT_RGBA16F:
        case E_GRAPH_PIXEL_FMT_RGBA32F:
        case E_GRAPH_PIXEL_FMT_DXT1:
        case E_GRAPH_PIXEL_FMT_DXT3:
        case E_GRAPH_PIXEL_FMT_DXT5:
        case E_GRAPH_PIXEL_FMT_D24S8:
        default:
            // Not Supported
            IZ_ASSERT(IZ_FALSE);
            break;
        }
    }

    //////////////////////////////////////////
    // プリミティブタイプ

    namespace {
        GLenum PrimTypeTbl[] = {
            GL_POINTS,
            GL_LINES,
            GL_LINE_STRIP,
            GL_TRIANGLES,
            GL_TRIANGLE_STRIP,
            GL_TRIANGLE_FAN,
        };
        IZ_C_ASSERT(COUNTOF(PrimTypeTbl) == E_GRAPH_PRIM_TYPE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(PrimType, GLenum, E_GRAPH_PRIM_TYPE, PrimTypeTbl)

    //////////////////////////////////////////
    // キューブフェイス

    namespace {
        GLenum CubeFaceTbl[] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        };
        IZ_C_ASSERT(COUNTOF(CubeFaceTbl) == E_GRAPH_CUBE_TEX_FACE_NUM);
    }   // namespace

    DEF_FUNC_CONV_PARAM_VAL(CubeFace, GLenum, E_GRAPH_CUBE_TEX_FACE, CubeFaceTbl);
}   // namespace graph
}   // namespace izanagi
