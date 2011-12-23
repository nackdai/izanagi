#include "TexProxy.h"

#include "MirrorMapProxy.h"
#include "LatLongMapProxy.h"

IZ_FLOAT CTexProxy::NormalizeColor(IZ_UINT8 c)
{
	static const IZ_FLOAT div = 1.0f / 255.0f;
	return c * div;
}

// テクスチャプロキシ作成
CTexProxy* CTexProxy::CreateTexProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
{
	CTexProxy* ret = IZ_NULL;

	switch (type) {
	case EnvMapTypeMirror:
		ret = new CMirrorMapProxy(tex, type);
		break;
	case EnvMapTypeLatLong:
		ret = new CLatLongMapProxy(tex, type);
		break;
	case EnvMapTypeAngular:
	case EnvMapTypeCube:
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

CTexProxy::CTexProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
{
	m_Type = type;
}

CTexProxy::~CTexProxy()
{
}
