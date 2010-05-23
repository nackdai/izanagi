#if !defined(__URANUS_GRAPH_PARAM_VALUE_CONVERTER_H__)
#define __URANUS_GRAPH_PARAM_VALUE_CONVERTER_H__

#include "../dx9/D3D9ParamValueConverter.h"

namespace uranus {
	typedef CD3D9ParamValueConverter CTargetParamValueConverter;
}	// namespace uranus

#define UN_GET_TARGET_VAL(p, v)		uranus::CTargetParamValueConverter::ConvAbstractToTarget_##p(v)
#define UN_GET_ABST_VAL(p, v)		uranus::CTargetParamValueConverter::ConvTargetToAbstract_##p(v)

#define UN_GET_TARGET_FILL_MODE(v)			UN_GET_TARGET_VAL(FillMode, v)
#define UN_GET_TARGET_BLEND_OP(v)			UN_GET_TARGET_VAL(BlendOp, v)
#define UN_GET_TARGET_BLEND(v)				UN_GET_TARGET_VAL(Blend, v)
#define UN_GET_TARGET_CULL(v)				UN_GET_TARGET_VAL(Cull, v)
#define UN_GET_TARGET_CMP(v)				UN_GET_TARGET_VAL(Cmp, v)
#define UN_GET_TARGET_TEX_ADDR(v)			UN_GET_TARGET_VAL(TexAddr, v)
#define UN_GET_TARGET_TEX_FILTER(v)			UN_GET_TARGET_VAL(TexFilter, v)
#define UN_GET_TARGET_SAMPLER_STATE_TYPE(v)	UN_GET_TARGET_VAL(SamplerStateType, v)
#define UN_GET_TARGET_PIXEL_FMT(v)			UN_GET_TARGET_VAL(PixelFormat, v)
#define UN_GET_TARGET_IDX_BUF_FMT(v)		UN_GET_TARGET_VAL(IdxBufFmt, v)
#define UN_GET_TARGET_PRIM_TYPE(v)			UN_GET_TARGET_VAL(PrimType, v)
#define UN_GET_TARGET_CUBE_FACE(v)			UN_GET_TARGET_VAL(CubeFace, v)
#define UN_GET_TARGET_VTX_DECL_TYPE(v)		UN_GET_TARGET_VAL(VtxDeclType, v)
#define UN_GET_TARGET_VTX_DECL_USAGE(v)		UN_GET_TARGET_VAL(VtxDeclUsage, v)

#define UN_GET_ABST_PIXEL_FMT(v)			UN_GET_ABST_VAL(PixelFormat, v)

#endif	// #if !defined(__URANUS_GRAPH_PARAM_VALUE_CONVERTER_H__)
