#include "izStd.h"
#include "izToolKit.h"
#include "izImageLib.h"

#include "ImageReaderImpl.h"

extern "C"
{
IZ_API IZ_BOOL izInitImageLib(void* hWnd)
{
	izanagi::tool::CGraphicsDeviceLite* device = izanagi::InitGraphicsDevice(hWnd);
	
	if (device != IZ_NULL) {
		// DirectXテクスチャ読み込み処理をセット
		izanagi::tool::CImageReader::GetInstance().SetReadFunc(izanagi::Read);
	}

	return (device != IZ_NULL);
}

IZ_API void izReleaseImageLib()
{
	izanagi::ReleaseGraphicsDevice();
}

IZ_API void* izReadImage(const char* path)
{
	izanagi::tool::CGraphicsDeviceLite* device = izanagi::GetGraphicsDevice();

	// TODO
	izanagi::tool::CIMGBody* body = izanagi::tool::CImageReader::GetInstance().Read(path, izanagi::E_GRAPH_TEX_TYPE_PLANE);
	return body;
}

IZ_API void izReleaseImage(void* p)
{
	IZ_ASSERT(p != IZ_NULL);
	izanagi::tool::CIMGBody* body =  reinterpret_cast<izanagi::tool::CIMGBody*>(p);
	if (body != IZ_NULL) {
		izanagi::tool::CImageReader::GetInstance().Delete(body);
	}
}

IZ_API IZ_UINT izGetTexNum(void* p)
{
	IZ_ASSERT(p != IZ_NULL);
	izanagi::tool::CIMGBody* body =  reinterpret_cast<izanagi::tool::CIMGBody*>(p);
	IZ_UINT ret = body->GetTexNum();
	return ret;
}

IZ_API void* izGetTexture(void* p, IZ_UINT texIdx)
{
	VRETURN_NULL(p != IZ_NULL);

	izanagi::tool::CIMGBody* body =  reinterpret_cast<izanagi::tool::CIMGBody*>(p);
	VRETURN_NULL(texIdx < body->GetTexNum());

	izanagi::tool::CIMGTexture* tex = body->GetTexture(texIdx);
	return tex;
}

IZ_API IZ_UINT izGetImageNum(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		IZ_UINT ret = tex->GetImageNum();
		return ret;
	}
	return 0;
}

IZ_API IZ_UINT izGetMipMapNum(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		// NOTE
		// どのイメージでもミップマップ数は同じ
		size_t ret = tex->GetImage(0).size();
		return (IZ_UINT)ret;
	}
	return 0;
}

IZ_API void* izGetImage(void* p, IZ_UINT imgIdx, IZ_UINT level)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		VRETURN_VAL(imgIdx < GetImageNum(p), 0);
		VRETURN_VAL(level < GetMipMapNum(p), 0);
		return tex->GetImage(imgIdx)[level];
	}
	return IZ_NULL;
}

IZ_API IZ_UINT izGetWidth(void* p)
{
	VRETURN_VAL(p != IZ_NULL, 0);

	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);

	IZ_UINT ret = img->GetWidth();
	return ret;
}

IZ_API IZ_UINT izGetHeight(void* p)
{
	izanagi::tool::CIMGImage* img = reinterpret_cast<izanagi::tool::CIMGImage*>(p);

	IZ_UINT ret = img->GetHeight();
	return ret;
}

static const char* format[] = {
	"RGBA8",
	"BGRA8",

	"RGBA4",
	"RGB10A2",
	"A8",
	
	"R32F",
	"RGBA16F",
	"RGBA32F",

	"DXT1",
	"DXT3",
	"DXT5",

	"RGBX8",
	"D24S8",
};

static const char* address[] = {
	"WRAP",
	"MIRROR",
	"CLAMP",
	"BORDER",
	"MIRRORONCE",
};

static const char* filter[] = {
	"NONE",				// filtering disabled (valid for mip filter only)
	"POINT",			// nearest
	"LINEAR",			// linear interpolation
	"ANISOTROPIC",		// anisotropic
	"PYRAMIDALQUAD",	// 4-sample tent
	"GAUSSIANQUAD",
};

static const char* GetString(
	const char* strings[],
	IZ_UINT type)
{
	VRETURN_NULL(type < COUNTOF(strings));
	return strings[type];
}

IZ_API const char* izGetImagePixelFormat(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		izanagi::E_GRAPH_PIXEL_FMT fmt = tex->GetTexInfo().fmt;
		VRETURN_NULL(fmt < COUNTOF(format));
		return format[fmt];
	}
	return IZ_NULL;
}

IZ_API const char* izGetAddressU(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().addressU);
		return ret;
	}
	return IZ_NULL;
}

IZ_API const char* izGetAddressV(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().addressV);
		return ret;
	}
	return IZ_NULL;
}

IZ_API const char* izGetMinFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().minFilter);
		return ret;
	}
	return IZ_NULL;
}

IZ_API const char* izGetMagFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().magFilter);
		return ret;
	}
	return IZ_NULL;
}

IZ_API const char* izGetMipFilger(void* p)
{
	izanagi::tool::CIMGTexture* tex = reinterpret_cast<izanagi::tool::CIMGTexture*>(p);
	if (tex != IZ_NULL) {
		const char* ret = GetString(address, tex->GetTexInfo().mipFilter);
		return ret;
	}
	return IZ_NULL;
}

IZ_API IZ_BOOL izConvertFormat(void* p, IZ_UINT fmt)
{
	// TODO
	return IZ_TRUE;
}

IZ_API void izSetAddressU(void* p, IZ_UINT addr)
{
}

IZ_API void izSetAddressV(void* p, IZ_UINT addr)
{
}

IZ_API void izSetMinFilter(void* p, IZ_UINT filter)
{
}

IZ_API void SetMagFilter(void* p, IZ_UINT filter)
{
}

IZ_API void izSetMipFilter(void* p, IZ_UINT filter)
{
}

}
