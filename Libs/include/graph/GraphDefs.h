#if !defined(__IZANAGI_GRAPH_GRAPH_DEFS_H__)
#define __IZANAGI_GRAPH_GRAPH_DEFS_H__

#include "izDefs.h"
#include "izStd.h"

#ifdef __IZ_DX9__
#include "izD3DDefs.h"
#elif __IZ_GLES2__
#include "graph/gles2/izGLES2Defs.h"
#endif

typedef void* TEX_HANDLE;

namespace izanagi
{
namespace graph
{
    enum {
        TEX_STAGE_NUM = 8,
        MAX_MRT_NUM = 2,
    };

    /**
    * レンダーステート
    */
    enum E_GRAPH_RENDER_STATE {
        E_GRAPH_RS_ZWRITEENABLE = 0,
        E_GRAPH_RS_ZENABLE,
        E_GRAPH_RS_ZFUNC,

        E_GRAPH_RS_ALPHATESTENABLE,
        E_GRAPH_RS_ALPHAREF,
        E_GRAPH_RS_ALPHAFUNC,
        E_GRAPH_RS_ALPHABLENDENABLE,
        E_GRAPH_RS_BLENDMETHOD,
        
        E_GRAPH_RS_FILLMODE,
        E_GRAPH_RS_CULLMODE,
        
        E_GRAPH_RS_COLORWRITEENABLE_RGB,
        E_GRAPH_RS_COLORWRITEENABLE_A,

        E_GRAPH_RS_SCISSORTESTENABLE,

        E_GRAPH_RS_NUM,
        E_GRAPH_RS_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * フィルモード
    */
    enum E_GRAPH_FILL_MODE {
        E_GRAPH_FILL_MODE_POINT = 0,
        E_GRAPH_FILL_MODE_WIREFRAME,
        E_GRAPH_FILL_MODE_SOLID,

        E_GRAPH_FILL_MODE_NUM,
        E_GRAPH_FILL_MODE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * ブレンド方法
    */
    enum E_GRAPH_BLEND_OP {
        E_GRAPH_BLEND_OP_ADD = 0,
        E_GRAPH_BLEND_OP_SUBTRACT,
        E_GRAPH_BLEND_OP_REVSUBTRACT,
        E_GRAPH_BLEND_OP_MIN,
        E_GRAPH_BLEND_OP_MAX,

        E_GRAPH_BLEND_OP_NUM,
        E_GRAPH_BLEND_OP_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * ブレンド係数
    */
    enum E_GRAPH_BLEND {
        E_GRAPH_BLEND_ZERO = 0,
        E_GRAPH_BLEND_ONE,
        E_GRAPH_BLEND_SRCCOLOR,
        E_GRAPH_BLEND_INVSRCCOLOR,
        E_GRAPH_BLEND_SRCALPHA,
        E_GRAPH_BLEND_INVSRCALPHA,
        E_GRAPH_BLEND_DESTALPHA,
        E_GRAPH_BLEND_INVDESTALPHA,
        E_GRAPH_BLEND_DESTCOLOR,
        E_GRAPH_BLEND_INVDESTCOLOR,
        E_GRAPH_BLEND_SRCALPHASAT,
        E_GRAPH_BLEND_BOTHSRCALPHA,
        E_GRAPH_BLEND_BOTHINVSRCALPHA,

        E_GRAPH_BLEND_NUM,
        E_GRAPH_BLEND_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * カリングモード
    */
    enum E_GRAPH_CULL {
        E_GRAPH_CULL_NONE = 0,
        E_GRAPH_CULL_CW,
        E_GRAPH_CULL_CCW,

        E_GRAPH_CULL_NUM,

        // NOTE
        // izanagiでは左手座標系なので
        // カリングの標準はCounterClockWiseにする
        E_GRAPH_CULL_DEFAULT = E_GRAPH_CULL_CCW,

        E_GRAPH_CULL_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * 比較処理
    */
    enum E_GRAPH_CMP_FUNC {
        E_GRAPH_CMP_FUNC_NEVER = 0,
        E_GRAPH_CMP_FUNC_LESS,
        E_GRAPH_CMP_FUNC_EQUAL,
        E_GRAPH_CMP_FUNC_LESSEQUAL,
        E_GRAPH_CMP_FUNC_GREATER,
        E_GRAPH_CMP_FUNC_NOTEQUAL,
        E_GRAPH_CMP_FUNC_GREATEREQUAL,
        E_GRAPH_CMP_FUNC_ALWAYS,

        E_GRAPH_CMP_FUNC_NUM,
        E_GRAPH_CMP_FUNC_FORCE_INT32 = 0x7ffffff,
    };

    /**
    * テクスチャアドレッシング
    */
    enum E_GRAPH_TEX_ADDRESS {
        E_GRAPH_TEX_ADDRESS_WRAP = 0,
        E_GRAPH_TEX_ADDRESS_MIRROR,
        E_GRAPH_TEX_ADDRESS_CLAMP,
        E_GRAPH_TEX_ADDRESS_BORDER,
        E_GRAPH_TEX_ADDRESS_MIRRORONCE,

        E_GRAPH_TEX_ADDRESS_NUM,
        E_GRAPH_TEX_ADDRESS_FORCE_INT32 = 0x7ffffff,
    };

    /**
    * テクスチャフィルタ
    */
    enum E_GRAPH_TEX_FILTER {
        E_GRAPH_TEX_FILTER_NONE = 0,        // filtering disabled (valid for mip filter only)
        E_GRAPH_TEX_FILTER_POINT,           // nearest
        E_GRAPH_TEX_FILTER_LINEAR,          // linear interpolation
        E_GRAPH_TEX_FILTER_ANISOTROPIC,     // anisotropic
        E_GRAPH_TEX_FILTER_PYRAMIDALQUAD,   // 4-sample tent
        E_GRAPH_TEX_FILTER_GAUSSIANQUAD,    // 4-sample gaussian

        E_GRAPH_TEX_FILTER_NUM,
        E_GRAPH_TEX_FILTER_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * サンプラーステートタイプ
    */
    enum E_GRAPH_SAMPLER_STATE_TYPE {
        E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU = 0,
        E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
        E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSW,

        E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
        E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
        E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,

        E_GRAPH_SAMPLER_STATE_TYPE_NUM,
        E_GRAPH_SAMPLER_STATE_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    inline IZ_BOOL IsSamplerStateTypeAddr(E_GRAPH_SAMPLER_STATE_TYPE n)
    {
        return ((E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU <= n) && (n <= E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSW));
    }
    inline IZ_BOOL IsSamplerStateTypeFilter(E_GRAPH_SAMPLER_STATE_TYPE n)
    {
        return ((E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER <= n) && (n <= E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER));
    }

    /**
    * アルファブレンド処理
    */
    #define IZ_GRAPH_ALPHA_BLEND_VAL(op, src, dst)  (((op & 0xff) << 16) | ((src & 0xff) << 8) | (dst & 0xff))
    #define IZ_GRAPH_GET_ALPHA_BLEND_OP(val)        ((val >> 16) & 0xff);
    #define IZ_GRAPH_GET_ALPHA_BLEND_SRC(val)       ((val >> 8) & 0xff);
    #define IZ_GRAPH_GET_ALPHA_BLEND_DST(val)       (val & 0xff)
    enum E_GRAPH_ALPHA_BLEND {
        // 通常のアルファブレンド
        E_GRAPH_ALPHA_BLEND_NORMAL   = IZ_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_INVSRCALPHA),

        // 加算
        E_GRAPH_ALPHA_BLEND_CsAs_Cd  = IZ_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_ONE),

        E_GRAPH_ALPHA_BLEND_Cs_Cd    = IZ_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_ADD,         E_GRAPH_BLEND_ONE,      E_GRAPH_BLEND_ONE),
        E_GRAPH_ALPHA_BLEND_NCsAs_Cd = IZ_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_REVSUBTRACT, E_GRAPH_BLEND_SRCALPHA, E_GRAPH_BLEND_ONE),
        E_GRAPH_ALPHA_BLEND_NCs_Cd   = IZ_GRAPH_ALPHA_BLEND_VAL(E_GRAPH_BLEND_OP_REVSUBTRACT, E_GRAPH_BLEND_ONE,      E_GRAPH_BLEND_ONE),

        E_GRAPH_ALPHA_BLEND_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * クリアフラグ
    */
    enum E_GRAPH_CLEAR_FLAG {
        E_GRAPH_CLEAR_FLAG_COLOR   = 1 << 0,
        E_GRAPH_CLEAR_FLAG_DEPTH   = 1 << 1,
        E_GRAPH_CLEAR_FLAG_STENCIL = 1 << 2,

        E_GRAPH_CLEAR_FLAG_ALL = E_GRAPH_CLEAR_FLAG_COLOR | E_GRAPH_CLEAR_FLAG_DEPTH | E_GRAPH_CLEAR_FLAG_STENCIL,

        E_GRAPH_CLEAR_FLAG_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * シーン終了フラグ
    */
    enum E_GRAPH_END_SCENE_FLAG {
        E_GRAPH_END_SCENE_FLAG_RT_0 = 1 << 0,           // レンダーターゲット０
        E_GRAPH_END_SCENE_FLAG_RT_1 = 1 << 1,           // レンダーターゲット１

        E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL = 1 << 2,  // 深度・ステンシル

        E_GRAPH_END_SCENE_FLAG_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * ピクセルフォーマット
    */
    enum E_GRAPH_PIXEL_FMT {
        E_GRAPH_PIXEL_FMT_RGBA8 = 0,
        E_GRAPH_PIXEL_FMT_BGRA8,

        E_GRAPH_PIXEL_FMT_RGBA4,
        E_GRAPH_PIXEL_FMT_RGB10A2,
        E_GRAPH_PIXEL_FMT_A8,
        
        E_GRAPH_PIXEL_FMT_R32F,
        E_GRAPH_PIXEL_FMT_RGBA16F,
        E_GRAPH_PIXEL_FMT_RGBA32F,

        E_GRAPH_PIXEL_FMT_DXT1,
        E_GRAPH_PIXEL_FMT_DXT3,
        E_GRAPH_PIXEL_FMT_DXT5,

        E_GRAPH_PIXEL_FMT_RGBX8,
        E_GRAPH_PIXEL_FMT_D24S8,

        E_GRAPH_PIXEL_FMT_NUM,
        E_GRAPH_PIXEL_FMT_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * インデックスバッファフォーマット
    */
    enum E_GRAPH_INDEX_BUFFER_FMT {
        E_GRAPH_INDEX_BUFFER_FMT_INDEX16 = 0,
        E_GRAPH_INDEX_BUFFER_FMT_INDEX32,

        E_GRAPH_INDEX_BUFFER_FMT_NUM,
        E_GRAPH_INDEX_BUFFER_FMT_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * テクスチャタイプ
    */
    enum E_GRAPH_TEX_TYPE {
        E_GRAPH_TEX_TYPE_PLANE = 0, // 通常
        E_GRAPH_TEX_TYPE_CUBE,      // キューブ
        E_GRAPH_TEX_TYPE_VOLUME,    // ボリューム

        E_GRAPH_TEX_TYPE_NUM,
        E_GRAPH_TEX_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * キューブテクスチャ面
    */
    enum E_GRAPH_CUBE_TEX_FACE {
        E_GRAPH_CUBE_TEX_FACE_X_P = 0,  // +X
        E_GRAPH_CUBE_TEX_FACE_X_N,      // -X
        E_GRAPH_CUBE_TEX_FACE_Y_P,      // +Y
        E_GRAPH_CUBE_TEX_FACE_Y_N,      // -Y
        E_GRAPH_CUBE_TEX_FACE_Z_P,      // +Z
        E_GRAPH_CUBE_TEX_FACE_Z_N,      // -Z

        E_GRAPH_CUBE_TEX_FACE_NUM,
        E_GRAPH_CUBE_TEX_FACE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * 2D描画設定
    */
    enum E_GRAPH_2D_RENDER_OP {
        E_GRAPH_2D_RENDER_OP_VTX = 0,       // Dst = Vtx
        E_GRAPH_2D_RENDER_OP_TEX,           // Dst = Tex
        E_GRAPH_2D_RENDER_OP_MODULATE,      // Dst = Vtx x Tex
        E_GRAPH_2D_RENDER_OP_MODULATE_2X,   // Dst = (Vtx x Tex) << 1
        E_GRAPH_2D_RENDER_OP_MODULATE_4X,   // Dst = (Vtx x Tex) << 2
        E_GRAPH_2D_RENDER_OP_ADD,           // Dst = Vtx + Tex

        E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA,    // Dst = Vtx x Tex.a
        E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA,      // Dst = Vtx x (Tex.rgb, 1.0)

        E_GRAPH_2D_RENDER_OP_NUM,
        E_GRAPH_2D_RENDER_OP_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * プリミティブタイプ
    */
    enum E_GRAPH_PRIM_TYPE {
        E_GRAPH_PRIM_TYPE_POINTLIST,
        E_GRAPH_PRIM_TYPE_LINELIST,
        E_GRAPH_PRIM_TYPE_LINESTRIP,
        E_GRAPH_PRIM_TYPE_TRIANGLELIST,
        E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
        E_GRAPH_PRIM_TYPE_TRIANGLEFAN,

        E_GRAPH_PRIM_TYPE_NUM,
        E_GRAPH_PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * リソースタイプ
    */
    enum E_GRAPH_RSC_USAGE {
        E_GRAPH_RSC_USAGE_STATIC  = 1 << 0,
        E_GRAPH_RSC_USAGE_DYNAMIC = 1 << 1,
        E_GRAPH_RSC_USAGE_STATIC_DYNAMIC = E_GRAPH_RSC_USAGE_DYNAMIC | E_GRAPH_RSC_USAGE_STATIC,

        E_GRAPH_RSC_USAGE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * 頂点宣言のデータ型
    */
    enum E_GRAPH_VTX_DECL_TYPE {
        E_GRAPH_VTX_DECL_TYPE_FLOAT1 =  0,  // 1D float expanded to (value, 0., 0., 1.)
        E_GRAPH_VTX_DECL_TYPE_FLOAT2,       // 2D float expanded to (value, value, 0., 1.)
        E_GRAPH_VTX_DECL_TYPE_FLOAT3,       // 3D float expanded to (value, value, value, 1.)
        E_GRAPH_VTX_DECL_TYPE_FLOAT4,       // 4D float
        E_GRAPH_VTX_DECL_TYPE_COLOR,        // 4D packed unsigned bytes mapped to 0. to 1. range. Input is in format (ARGB) expanded to (R, G, B, A)
        E_GRAPH_VTX_DECL_TYPE_UBYTE4,       // 4D unsigned byte
        E_GRAPH_VTX_DECL_TYPE_SHORT2,       // 2D signed short expanded to (value, value, 0., 1.)
        E_GRAPH_VTX_DECL_TYPE_SHORT4,       // 4D signed short

        // The following types are valid only with vertex shaders >= 2.0

        E_GRAPH_VTX_DECL_TYPE_UBYTE4N,      // Each of 4 bytes is normalized by dividing to 255.0
        E_GRAPH_VTX_DECL_TYPE_SHORT2N,      // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
        E_GRAPH_VTX_DECL_TYPE_SHORT4N,      // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
        E_GRAPH_VTX_DECL_TYPE_USHORT2N,     // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
        E_GRAPH_VTX_DECL_TYPE_USHORT4N,     // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
        E_GRAPH_VTX_DECL_TYPE_UDEC3,        // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
        E_GRAPH_VTX_DECL_TYPE_DEC3N,        // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
        E_GRAPH_VTX_DECL_TYPE_FLOAT16_2,    // Two 16-bit floating point values, expanded to (value, value, 0, 1)
        E_GRAPH_VTX_DECL_TYPE_FLOAT16_4,    // Four 16-bit floating point values
        E_GRAPH_VTX_DECL_TYPE_UNUSED,       // When the type field in a decl is unused.

        E_GRAPH_VTX_DECL_TYPE_NUM,
        E_GRAPH_VTX_DECL_TYPE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * 頂点データの使い方
    */
    enum E_GRAPH_VTX_DECL_USAGE {
        E_GRAPH_VTX_DECL_USAGE_POSITION = 0,    // 頂点位置
        E_GRAPH_VTX_DECL_USAGE_BLENDWEIGHT,     // ブレンディングの重み
        E_GRAPH_VTX_DECL_USAGE_BLENDINDICES,    // ブレンディング インデックス 
        E_GRAPH_VTX_DECL_USAGE_NORMAL,          // 頂点法線
        E_GRAPH_VTX_DECL_USAGE_PSIZE,           // ポイント サイズ 
        E_GRAPH_VTX_DECL_USAGE_TEXCOORD,        // テクスチャ座標
        E_GRAPH_VTX_DECL_USAGE_TANGENT,         // 頂点の接線
        E_GRAPH_VTX_DECL_USAGE_BINORMAL,        // 頂点の従法線
        E_GRAPH_VTX_DECL_USAGE_TESSFACTOR,
        E_GRAPH_VTX_DECL_USAGE_POSITIONT,       // 座標変換された位置座標
        E_GRAPH_VTX_DECL_USAGE_COLOR,           // 頂点カラー
        E_GRAPH_VTX_DECL_USAGE_FOG,             // フォグ
        E_GRAPH_VTX_DECL_USAGE_DEPTH,
        E_GRAPH_VTX_DECL_USAGE_SAMPLE,

        E_GRAPH_VTX_DECL_USAGE_NUM,
        E_GRAPH_VTX_DECL_USAGE_FORCE_INT32 = 0x7fffffff,
    };

    /**
    * ビューポート
    */
    struct SViewport {
        // Viewport Top left
        IZ_UINT x;
        IZ_UINT y;

        // Viewport Dimensions
        IZ_UINT width;
        IZ_UINT height;

        // Min/max of clip Volume
        IZ_FLOAT minZ;
        IZ_FLOAT maxZ;
    };

    /**
    * サンプラステート
    */
    struct S_SAMPLER_STATE {
        // フィルタ
        E_GRAPH_TEX_FILTER minFilter;
        E_GRAPH_TEX_FILTER magFilter;
        E_GRAPH_TEX_FILTER mipFilter;

        // アドレッシング
        E_GRAPH_TEX_ADDRESS addressU;
        E_GRAPH_TEX_ADDRESS addressV;

        // コンストラクタ
        S_SAMPLER_STATE()
        {
            minFilter = E_GRAPH_TEX_FILTER_LINEAR;
            magFilter = E_GRAPH_TEX_FILTER_LINEAR;
            mipFilter = E_GRAPH_TEX_FILTER_LINEAR;

            addressU = E_GRAPH_TEX_ADDRESS_CLAMP;
            addressV = E_GRAPH_TEX_ADDRESS_CLAMP;
        }
    };

    ////////////////////////////////////////////
    
    /**
    * リセット用コールバック関数型
    */
    typedef IZ_BOOL (*GraphicsDeviceResetCallBack)();

    /**
    * デバイスロスト用コールバック関数型
    */
    typedef void (*GraphicsDeviceLostDeviceCallBack)();
}   // namespace graph
}   // namespace izanagi;

#endif  // #if !defined(__IZANAGI_GRAPH_GRAPH_DEFS_H__)
