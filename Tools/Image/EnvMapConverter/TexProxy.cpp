#include "TexProxy.h"

#include "MirrorMapProxy.h"
#include "LatLongMapProxy.h"
#include "AngularMapProxy.h"
#include "CubeMapProxy.h"
#include "CrossMapProxy.h"

IZ_FLOAT CTexProxy::NormalizeColor(IZ_UINT8 c)
{
	static const IZ_FLOAT div = 1.0f / 255.0f;
	return c * div;
}

namespace {
	enum {
		CLR_POS_SELF,
		CLR_POS_RIGHT,
		CLR_POS_BOTTOM,
		CLR_POS_BOTTOM_LIGHT,

		CLR_POS_NUM,
	};
}

// バイリニアフィルタ計算
void CTexProxy::getBiLinearColor(
	IZ_BOOL isFloat,
	IZ_UINT width, IZ_UINT height,
	IZ_UINT pitch, IZ_UINT bpp,
	const IZ_UINT8* data,
	IZ_FLOAT u, IZ_FLOAT v,
	SFloatColor& color)
{
	IZ_FLOAT x = u * (width - 1);
	IZ_FLOAT y = v * (height - 1);

	IZ_UINT iX0 = static_cast<IZ_UINT>(x);
	IZ_UINT iY0 = static_cast<IZ_UINT>(y);

	IZ_FLOAT fracX = x - iX0;
	IZ_FLOAT fracY = y - iY0;

	IZ_UINT iX1 = min(iX0 + 1, width - 1);
	IZ_UINT iY1 = min(iY0 + 1, height - 1);

	// 周囲の色を取得する
	SFloatColor dst[CLR_POS_NUM];

	CTexProxy::getColor(
		isFloat,
		iX0, iY0,
		pitch, bpp,
		data,
		dst[CLR_POS_SELF]);

	CTexProxy::getColor(
		isFloat,
		iX1, iY0,
		pitch, bpp,
		data,
		dst[CLR_POS_RIGHT]);

	CTexProxy::getColor(
		isFloat,
		iX0, iY1,
		pitch, bpp,
		data,
		dst[CLR_POS_BOTTOM]);

	CTexProxy::getColor(
		isFloat,
		iX1, iY1,
		pitch, bpp,
		data,
		dst[CLR_POS_BOTTOM_LIGHT]);

	// ウエイト
	IZ_FLOAT weight[CLR_POS_NUM] = {
		(1.0f - fracX) * (1.0f - fracY),
		fracX * (1.0f - fracY),
		(1.0f - fracX) * fracY,
		fracX * fracY,
	};

	color.r = 0.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 0.0f;

	for (IZ_UINT i = 0; i < CLR_POS_NUM; i++) {
		color.r += dst[i].r * weight[i];
		color.g += dst[i].g * weight[i];
		color.b += dst[i].b * weight[i];
		color.a += dst[i].a * weight[i];
	}
}

// テクスチャプロキシ作成
CTexProxy* CTexProxy::CreateTexProxy(
	std::vector<izanagi::tool::CTextureLite*>& tex,
	EnvMapType type)
{
	IZ_ASSERT(tex.size() > 0);

	CTexProxy* ret = IZ_NULL;

	switch (type) {
	case EnvMapTypeMirror:
		ret = new CMirrorMapProxy(tex[0], type);
		break;
	case EnvMapTypeLatLong:
		ret = new CLatLongMapProxy(tex[0], type);
		break;
	case EnvMapTypeAngular:
		ret = new CAngularMapProxy(tex[0], type);
		break;
	case EnvMapTypeCube:
		ret = new CCubeMapProxy(tex, type);
		break;
    case EnvMapTypeCross:
        ret = new CCrossMapProxy(tex[0], type);
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}

	return ret;
}

// テクスチャプロキシ破棄
void CTexProxy::DeleteTexProxy(CTexProxy*& tex)
{
	if (tex != IZ_NULL) {
		delete tex;
		tex = IZ_NULL;
	}
}

CTexProxy::CTexProxy(EnvMapType type)
{
	m_Type = type;

	m_IsFloat = IZ_FALSE;
	m_Bpp = 1;
}

CTexProxy::~CTexProxy()
{
}
