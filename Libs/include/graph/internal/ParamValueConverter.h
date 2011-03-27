#if !defined(__IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_H__)
#define __IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_H__

#include "../dx9/D3D9ParamValueConverter.h"

namespace izanagi {
	typedef CD3D9ParamValueConverter CTargetParamValueConverter;
}	// namespace izanagi

#define IZ_CONV_PARAM_TO_TARGET(p)	izanagi::CTargetParamValueConverter::ConvAbstractToTarget_##p
#define IZ_CONV_PARAM_TO_ABST(p)	izanagi::CTargetParamValueConverter::ConvTargetToAbstract_##p

#define IZ_GET_TARGET_VAL(p, v)		IZ_CONV_PARAM_TO_TARGET(p)(v)
#define IZ_GET_ABST_VAL(p, v)		IZ_CONV_PARAM_TO_ABST(p)(v)

#define IZ_GET_TARGET_FILL_MODE(v)			IZ_GET_TARGET_VAL(FillMode, v)
#define IZ_GET_TARGET_BLEND_OP(v)			IZ_GET_TARGET_VAL(BlendOp, v)
#define IZ_GET_TARGET_BLEND(v)				IZ_GET_TARGET_VAL(Blend, v)
#define IZ_GET_TARGET_CULL(v)				IZ_GET_TARGET_VAL(Cull, v)
#define IZ_GET_TARGET_CMP(v)				IZ_GET_TARGET_VAL(Cmp, v)
#define IZ_GET_TARGET_TEX_ADDR(v)			IZ_GET_TARGET_VAL(TexAddr, v)
#define IZ_GET_TARGET_TEX_FILTER(v)			IZ_GET_TARGET_VAL(TexFilter, v)
#define IZ_GET_TARGET_SAMPLER_STATE_TYPE(v)	IZ_GET_TARGET_VAL(SamplerStateType, v)
#define IZ_GET_TARGET_PIXEL_FMT(v)			IZ_GET_TARGET_VAL(PixelFormat, v)
#define IZ_GET_TARGET_IDX_BUF_FMT(v)		IZ_GET_TARGET_VAL(IdxBufFmt, v)
#define IZ_GET_TARGET_PRIM_TYPE(v)			IZ_GET_TARGET_VAL(PrimType, v)
#define IZ_GET_TARGET_CUBE_FACE(v)			IZ_GET_TARGET_VAL(CubeFace, v)
#define IZ_GET_TARGET_VTX_DECL_TYPE(v)		IZ_GET_TARGET_VAL(VtxDeclType, v)
#define IZ_GET_TARGET_VTX_DECL_USAGE(v)		IZ_GET_TARGET_VAL(VtxDeclUsage, v)

#define IZ_GET_ABST_PIXEL_FMT(v)			IZ_GET_ABST_VAL(PixelFormat, v)

#define IZ_GET_ABST_FILL_MODE(v)			IZ_GET_ABST_VAL(FillMode, v)
#define IZ_GET_ABST_BLEND_OP(v)				IZ_GET_ABST_VAL(BlendOp, v)
#define IZ_GET_ABST_BLEND(v)				IZ_GET_ABST_VAL(Blend, v)
#define IZ_GET_ABST_CULL(v)					IZ_GET_ABST_VAL(Cull, v)
#define IZ_GET_ABST_CMP(v)					IZ_GET_ABST_VAL(Cmp, v)
#define IZ_GET_ABST_TEX_ADDR(v)				IZ_GET_ABST_VAL(TexAddr, v)
#define IZ_GET_ABST_TEX_FILTER(v)			IZ_GET_ABST_VAL(TexFilter, v)
#define IZ_GET_ABST_SAMPLER_STATE_TYPE(v)	IZ_GET_ABST_VAL(SamplerStateType, v)
#define IZ_GET_ABST_PIXEL_FMT(v)			IZ_GET_ABST_VAL(PixelFormat, v)
#define IZ_GET_ABST_IDX_BUF_FMT(v)			IZ_GET_ABST_VAL(IdxBufFmt, v)
#define IZ_GET_ABST_PRIM_TYPE(v)			IZ_GET_ABST_VAL(PrimType, v)
#define IZ_GET_ABST_CUBE_FACE(v)			IZ_GET_ABST_VAL(CubeFace, v)
#define IZ_GET_ABST_VTX_DECL_TYPE(v)		IZ_GET_ABST_VAL(VtxDeclType, v)
#define IZ_GET_ABST_VTX_DECL_USAGE(v)		IZ_GET_ABST_VAL(VtxDeclUsage, v)

#endif	// #if !defined(__IZANAGI_GRAPH_PARAM_VALUE_CONVERTER_H__)
