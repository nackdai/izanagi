#include "TextureUtil.h"
#include "izToolKit.h"

#if 0
namespace {
	// format
	BOOL _SetValueFormat(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		static struct {
			IZ_PCSTR strFmt;
			izanagi::E_GRAPH_PIXEL_FMT fmt;
		} tbl[] = {
			{"A8R8G8B8",      izanagi::E_GRAPH_PIXEL_FMT_RGBA8},
			{"A16B16G16R16F", izanagi::E_GRAPH_PIXEL_FMT_RGBA16F},
			{"A32B32G32R32F", izanagi::E_GRAPH_PIXEL_FMT_RGBA32F},
			{"R32F",          izanagi::E_GRAPH_PIXEL_FMT_R32F},
			{"A2R10G10B10",   izanagi::E_GRAPH_PIXEL_FMT_RGB10A2},
			{"A8",            izanagi::E_GRAPH_PIXEL_FMT_A8},
		};

		IZ_PCSTR fmt = ::cgGetStringAnnotationValue(ann);
		VRETURN(fmt != NULL);

		for (IZ_UINT i = 0; i < COUNTOF(tbl); i++) {
			if (izanagi::izanagi_tk::izanagi::izanagi_tk::CString::CmpStr(fmt, tbl[i].strFmt)) {
				sAnn.fmt = tbl[i].fmt;
				break;
			}
		}

		return TRUE;
	}

	// Dimensions
	BOOL _SetValueDimensions(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		INT num;
		const float* p = cgGetFloatAnnotationValues(ann, &num);
		if (num == 2) {
			// 必ず２なければいけない
			sAnn.Dimensions[0] = p[0];
			sAnn.Dimensions[1] = p[1];
		}
		return (num == 2);
	}

	// ViewportRatio
	BOOL _SetValueViewportRatio(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		INT num;
		const float* p = cgGetFloatAnnotationValues(ann, &num);
		if (num == 2) {
			// 必ず２なければいけない
			sAnn.ViewportRatio[0] = p[0];
			sAnn.ViewportRatio[1] = p[1];
		}
		return (num == 2);
	}

	// IsRenderTarget
	BOOL _SetValueIsRenderTarget(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		int num;
		const CGbool* p = cgGetBoolAnnotationValues(ann, &num);
		if (p != NULL) {
			sAnn.isRenderTarget = *p;
		}
		return (p != NULL);
	}

	// IsDynamic
	BOOL _SetValueIsDynamic(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		int num;
		const CGbool* p = cgGetBoolAnnotationValues(ann, &num);
		if (p != NULL) {
			sAnn.isDynamic = *p;
		}
		return (p != NULL);
	}

	// RscType
	BOOL _SetValueRscType(izanagi::S_SHD_TEXTURE_ANN& sAnn, CGannotation ann)
	{
		static struct {
			IZ_PCSTR strFmt;
			izanagi::E_GRAPH_RSC_TYPE type;
		} tbl[] = {
			{"STATIC", izanagi::E_GRAPH_RSC_TYPE_STATIC},
			{"DYNAMIC", izanagi::E_GRAPH_RSC_TYPE_DYNAMIC},
			{"STATIC_DYNAMIC", izanagi::E_GRAPH_RSC_TYPE_STATIC_DYNAMIC},
		};

		IZ_PCSTR fmt = ::cgGetStringAnnotationValue(ann);
		VRETURN(fmt != NULL);

		for (IZ_UINT i = 0; i < COUNTOF(tbl); i++) {
			if (izanagi::izanagi_tk::izanagi::izanagi_tk::CString::CmpStr(fmt, tbl[i].strFmt)) {
				sAnn.typeRsc = tbl[i].type;
				break;
			}
		}

		return TRUE;
	}

	// 各種アノテーションをセット
	BOOL _SetAnnValue(
		izanagi::S_SHD_TEXTURE_ANN& sAnn, 
		CGannotation ann)
	{
		static struct {
			const char* Name;
			BOOL (*Func)(izanagi::S_SHD_TEXTURE_ANN&, CGannotation);
		} AnnTable[] = {
			{"format", _SetValueFormat},					// ピクセルフォーマット
			{"Dimensions", _SetValueDimensions},			// サイズ
			{"ViewportRatio", _SetValueViewportRatio},		// 比率
			{"IsRenderTarget", _SetValueIsRenderTarget},	// レンダーターゲットか
			{"IsDynamic", _SetValueIsDynamic},				// 動的か
			{"RscType", _SetValueRscType},
		};

		BOOL ret = TRUE;

		izanagi::izanagi_tk::CString str(cgGetAnnotationName(ann));

		for (UINT i = 0; i < COUNTOF(AnnTable); i++) {
			if (str == AnnTable[i].Name) {
				ret = (*AnnTable[i].Func)(sAnn, ann);
				break;
			}
		}

		IZ_ASSERT(ret);
		return ret;
	}
}	// namespace

BOOL CTextureUtil::SetAnnValue(
	izanagi::S_SHD_TEXTURE_ANN& sAnn, 
	::CGparameter param)
{
	CGannotation ann = ::cgGetFirstParameterAnnotation(param);
	while (ann != NULL) {
		VRETURN(_SetAnnValue(sAnn, ann));
		ann = ::cgGetNextAnnotation(ann);
	}

	return TRUE;
}
#endif
