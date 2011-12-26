#if !defined(__TEX_PROXY_H__)
#define __TEX_PROXY_H__

#include "izToolKit.h"
#include "izMath.h"
#include "EnvMapConverterDefs.h"

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

public:
	// テクスチャプロキシ作成
	static CTexProxy* CreateTexProxy(
		std::vector<izanagi::izanagi_tk::CTextureLite*>& tex,
		EnvMapType type);

	// テクスチャプロキシ破棄
	static void DeleteTexProxy(CTexProxy*& tex);

public:
	CTexProxy(EnvMapType type);
	virtual ~CTexProxy();

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
		izanagi::E_GRAPH_CUBE_TEX_FACE face = izanagi::E_GRAPH_CUBE_TEX_FACE_NUM) = 0;

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
