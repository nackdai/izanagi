#include "graph/ogl/OGLParamValueConverter.h"
#include "graph/ogl/glext.h"

using namespace uranus;

#define DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst)	target CParamValueConverter::ConvAbstractToTarget_##p(abst v)
#define DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst)	abst CParamValueConverter::ConvTargetToAbstract_##p(target v)

#define GET_TARGET_TO_ABSTRACT(tbl, v, abst)\
	UN_INT ret = -1;\
	for (UN_UINT i = 0; i < COUNTOF(tbl); i++) {\
		if (tbl[i] == v) {\
			ret = i;\
			break;\
		}\
	}\
	UN_ASSERT(ret > 0);\
	return static_cast<abst>(ret)

#define DEF_FUNC_CONV_PARAM_VAL(p, target, abst, tbl)\
	DEF_FUNC_ABSTRACT_TO_TARGET(p, target, abst) { UN_ASSERT(v < COUNTOF(tbl)); return tbl[v]; }\
	DEF_FUNC_TARGET_TO_ABSTRACT(p, target, abst) { GET_TARGET_TO_ABSTRACT(tbl, v, abst); }

//////////////////////////////////////////
// フィルモード

namespace {
	GLenum FillModeTbl[] = {
		GL_POINT,
		GL_LINE,
		GL_FILL,
	};
	UN_C_ASSERT(COUNTOF(FillModeTbl) == E_GRAPH_FILL_MODE_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(FillMode, GLenum, E_GRAPH_FILL_MODE, FillModeTbl)

//////////////////////////////////////////
// ブレンド方法

namespace {
	GLenum BlendOpTbl[] = {
		GL_FUNC_ADD,
		GL_FUNC_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT,
		0,	// D3DBLENDOP_MIN
		0,	// D3DBLENDOP_MAX
	};
	UN_C_ASSERT(COUNTOF(BlendOpTbl) == E_GRAPH_BLEND_OP_NUM);
}	// namespace

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
		0,	// D3DBLEND_BOTHSRCALPHA
		0,	// D3DBLEND_BOTHINVSRCALPHA
	};
	UN_C_ASSERT(COUNTOF(BlendTbl) == E_GRAPH_BLEND_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(Blend, GLenum, E_GRAPH_BLEND, BlendTbl)

//////////////////////////////////////////
// カリングモード

namespace {
	GLenum CullTbl[] = {
		GL_FRONT_AND_BACK,
		GL_FRONT,
		GL_BACK,
	};
	UN_C_ASSERT(COUNTOF(CullTbl) == E_GRAPH_CULL_NUM);
}	// namespace

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
	UN_C_ASSERT(COUNTOF(CmpTbl) == E_GRAPH_CMP_FUNC_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(Cmp, GLenum, E_GRAPH_CMP_FUNC, CmpTbl)

//////////////////////////////////////////
// テクスチャアドレッシング

namespace {
	GLenum TexAddrTbl[] = {
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP,
		GL_CLAMP_TO_BORDER,
		0,	// D3DTADDRESS_MIRRORONCE
	};
	UN_C_ASSERT(COUNTOF(TexAddrTbl) == E_GRAPH_TEX_ADDRESS_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(TexAddr, GLenum, E_GRAPH_TEX_ADDRESS, TexAddrTbl)

//////////////////////////////////////////
// テクスチャフィルタ

namespace {
	GLenum TexFilterTbl[] = {
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
		D3DTEXF_PYRAMIDALQUAD,
		D3DTEXF_GAUSSIANQUAD,
	};
	UN_C_ASSERT(COUNTOF(TexFilterTbl) == E_GRAPH_TEX_FILTER_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(TexFilter, D3DTEXTUREFILTERTYPE, E_GRAPH_TEX_FILTER, TexFilterTbl)

//////////////////////////////////////////
// サンプラーステートタイプ

namespace {
	GLenum SamplerStateTypeTbl[] = {
		GL_TEXTURE_WRAP_S,	// D3DSAMP_ADDRESSU
		GL_TEXTURE_WRAP_T,	// D3DSAMP_ADDRESSV
		0,	// D3DSAMP_ADDRESSW,
		GL_TEXTURE_MAG_FILTER,	// D3DSAMP_MAGFILTER
		GL_TEXTURE_MIN_FILTER,	// D3DSAMP_MINFILTER
		0,	// D3DSAMP_MIPFILTER,
	};
	UN_C_ASSERT(COUNTOF(SamplerStateTypeTbl) == E_GRAPH_SAMPLER_STATE_TYPE_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(SamplerStateType, GLenum, E_GRAPH_SAMPLER_STATE_TYPE, SamplerStateTypeTbl)

//////////////////////////////////////////
// ピクセルフォーマット

namespace {
	UN_UINT PixelFormatTbl[] = {
		UN_OGL_PIXEL_FMT(GL_RGBA, GL_UNSIGNED_BYTE),	// D3DFMT_A8R8G8B8,
		0,	// D3DFMT_A8B8G8R8,
		UN_OGL_PIXEL_FMT(GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4),	// D3DFMT_A4R4G4B4,
		UN_OGL_PIXEL_FMT(GL_RGBA, GL_UNSIGNED_INT_10_10_10_2),	// D3DFMT_A2R10G10B10,
		UN_OGL_PIXEL_FMT(GL_ALPHA, GL_UNSIGNED_BYTE),			// D3DFMT_A8,
		UN_OGL_PIXEL_FMT(GL_RED, GL_FLOAT),						// D3DFMT_R32F,
		UN_OGL_PIXEL_FMT(GL_RGBA, GL_UNSIGNED_SHORT),			// D3DFMT_A16B16G16R16F,
		UN_OGL_PIXEL_FMT(GL_RGBA, GL_FLOAT),					// D3DFMT_A32B32G32R32F,
		UN_OGL_PIXEL_FMT(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE),	// D3DFMT_DXT1	
		UN_OGL_PIXEL_FMT(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_UNSIGNED_BYTE),	// D3DFMT_DXT3
		UN_OGL_PIXEL_FMT(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_UNSIGNED_BYTE),	// D3DFMT_DXT5
	};
	UN_C_ASSERT(COUNTOF(PixelFormatTbl) == E_GRAPH_PIXEL_FMT_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(PixelFormat, UN_UINT, E_GRAPH_PIXEL_FMT, PixelFormatTbl)

//////////////////////////////////////////
// インデックスバッファフォーマット

// Not used...
// Dummy

namespace {
	GLenum IdxBufFmtTbl[] = {
		GL_UNSIGNED_SHORT,	// D3DFMT_INDEX16
		GL_UNSIGNED_INT,	// D3DFMT_INDEX32
	};
	UN_C_ASSERT(COUNTOF(IdxBufFmtTbl) == E_GRAPH_INDEX_BUFFER_FMT_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(IdxBufFmt, GLenum, E_GRAPH_INDEX_BUFFER_FMT, IdxBufFmtTbl)

//////////////////////////////////////////
// プリミティブタイプ

namespace {
	GLenum PrimTypeTbl[] = {
		GL_POINTS,			// D3DPT_POINTLIST,
		GL_LINES,			// D3DPT_LINELIST,
		GL_LINE_STRIP,		// D3DPT_LINESTRIP,
		GL_TRIANGLES,		// D3DPT_TRIANGLELIST,
		GL_TRIANGLE_STRIP,	// D3DPT_TRIANGLESTRIP,
		GL_TRIANGLE_FAN,	// D3DPT_TRIANGLEFAN,
	};
	UN_C_ASSERT(COUNTOF(PrimTypeTbl) == E_GRAPH_PRIM_TYPE_NUM);
}	// namespace

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
	UN_C_ASSERT(COUNTOF(CubeFaceTbl) == E_GRAPH_CUBE_TEX_FACE_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(CubeFace, GLenum, E_GRAPH_CUBE_TEX_FACE, CubeFaceTbl);

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
	UN_C_ASSERT(COUNTOF(VtxDeclTypeTbl) == E_GRAPH_VTX_DECL_TYPE_NUM);
}	// namespace

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
	UN_C_ASSERT(COUNTOF(VtxDeclUsageTbl) == E_GRAPH_VTX_DECL_USAGE_NUM);
}	// namespace

DEF_FUNC_CONV_PARAM_VAL(VtxDeclUsage, D3DDECLUSAGE, E_GRAPH_VTX_DECL_USAGE, VtxDeclUsageTbl);
