#if !defined(__TEX_PROXY_H__)
#define __TEX_PROXY_H__

#include "izToolKit.h"
#include "izMath.h"
#include "EnvMapConverterDefs.h"

#define ENABLE_BILINEAR

struct SFloatColor {
	IZ_FLOAT r;
	IZ_FLOAT g;
	IZ_FLOAT b;
	IZ_FLOAT a;

	IZ_UINT8 getAsUint8_R() const { return izanagi::CMath::Clamp<IZ_UINT>((IZ_UINT)(r * 255), 0, 255);	}
	IZ_UINT8 getAsUint8_G() const{ return izanagi::CMath::Clamp<IZ_UINT>((IZ_UINT)(g * 255), 0, 255);	}
	IZ_UINT8 getAsUint8_B() const{ return izanagi::CMath::Clamp<IZ_UINT>((IZ_UINT)(b * 255), 0, 255);	}
	IZ_UINT8 getAsUint8_A() const{ return izanagi::CMath::Clamp<IZ_UINT>((IZ_UINT)(a * 255), 0, 255);	}
};

/** テクスチャプロキシ
 */
class CTexProxy {
protected:
	static IZ_FLOAT NormalizeColor(IZ_UINT8 c);

	static void getColor(
		IZ_BOOL isFloat,
		IZ_UINT x, IZ_UINT y,
		IZ_UINT pitch, IZ_UINT bpp,
		const IZ_UINT8* src,
		SFloatColor& dst)
	{
		const IZ_UINT8* data = src + pitch * y + x * bpp;

		if (isFloat) {
			const IZ_FLOAT* d = reinterpret_cast<const IZ_FLOAT*>(data);

			dst.r = d[0];
			dst.r = d[1];
			dst.r = d[2];
			dst.r = d[3];
		}
		else {
			dst.r = NormalizeColor(data[0]);
			dst.g = NormalizeColor(data[1]);
			dst.b = NormalizeColor(data[2]);
			dst.a = NormalizeColor(data[3]);
		}
	}

	static void putColor(
		IZ_BOOL isFloat,
		IZ_UINT x, IZ_UINT y,
		IZ_UINT pitch, IZ_UINT bpp,
		const SFloatColor& src,
		IZ_UINT8* dst)
	{
		IZ_UINT8* data = dst + pitch * y + x * bpp;

		if (isFloat) {
			IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

			d[0] = src.r;
			d[1] = src.g;
			d[2] = src.b;
			d[3] = src.a;
		}
		else {
			data[0] = src.getAsUint8_R();
			data[1] = src.getAsUint8_G();
			data[2] = src.getAsUint8_B();
			data[3] = src.getAsUint8_A();
		}
	}

	// バイリニアフィルタ計算
	static void getBiLinearColor(
		IZ_BOOL isFloat,
		IZ_UINT width, IZ_UINT height,
		IZ_UINT pitch, IZ_UINT bpp,
		const IZ_UINT8* data,
		IZ_FLOAT u, IZ_FLOAT v,
		SFloatColor& color);

public:
	// テクスチャプロキシ作成
	static CTexProxy* CreateTexProxy(
		std::vector<izanagi::tool::CTextureLite*>& tex,
		EnvMapType type);

	// テクスチャプロキシ破棄
	static void DeleteTexProxy(CTexProxy*& tex);

public:
	CTexProxy(EnvMapType type);
	virtual ~CTexProxy();

    CTexProxy() {}

public:
	/** 反射ベクトルからUVを取得.
	 */
	virtual void getUVFromRef(
		const izanagi::SVector& ref,
		IZ_FLOAT& u, IZ_FLOAT& v) = 0;

	/** XYから反射ベクトルを取得.
	 */
	virtual void getRef(
		IZ_UINT x, IZ_UINT y,
		izanagi::SVector& ref,
		izanagi::graph::E_GRAPH_CUBE_TEX_FACE face = izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM) = 0;

	/** UVから色を取得.
	 */
	virtual void getColor(
		IZ_FLOAT u, IZ_FLOAT v,
		SFloatColor& color) = 0;

	/** 指定位置に描きこみ.
	 */
	virtual void putColor(
		IZ_UINT x, IZ_UINT y,
		const SFloatColor& color) = 0;

	/** XYが正しい位置かどうか.
	 */
	virtual IZ_BOOL isValid(IZ_UINT x, IZ_UINT y) const = 0;

protected:
	EnvMapType m_Type;

	IZ_BOOL m_IsFloat;
	IZ_UINT m_Bpp;
};

#endif	// #if !defined(__TEX_PROXY_H__)
