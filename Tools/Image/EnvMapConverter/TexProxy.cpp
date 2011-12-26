#include "TexProxy.h"

#include "MirrorMapProxy.h"
#include "LatLongMapProxy.h"
#include "AngularMapProxy.h"
#include "CubeMapProxy.h"

IZ_FLOAT CTexProxy::NormalizeColor(IZ_UINT8 c)
{
	static const IZ_FLOAT div = 1.0f / 255.0f;
	return c * div;
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
