#if !defined(__LAT_LONG_MAP_PROXY_H__)
#define __LAT_LONG_MAP_PROXY_H__

#include "TexProxy.h"

/** Latitude-Longitudeマッププロキシ
 */
class CLatLongMapProxy : public CTexProxy {
public:
	CLatLongMapProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type);
	virtual ~CLatLongMapProxy();

public:
	/** 反射ベクトルからUVを取得.
	 */
	virtual void getUVFromRef(
		const izanagi::SVector& ref,
		IZ_FLOAT& u, IZ_FLOAT& v);

	/** XYから反射ベクトルを取得.
	 */
	virtual IZ_BOOL getRef(
		IZ_UINT x, IZ_UINT y,
		izanagi::SVector& ref);

	/** UVから色を取得.
	 */
	virtual void getColor(
		IZ_FLOAT u, IZ_FLOAT v,
		SFloatColor& color);

	/** 指定位置に描きこみ.
	 */
	virtual void putColor(
		IZ_UINT x, IZ_UINT y,
		const SFloatColor& color);

	/** XYが正しい位置かどうか.
	 */
	virtual IZ_BOOL isValid(IZ_UINT x, IZ_UINT y) const;

protected:
	izanagi::izanagi_tk::CTextureLite* m_Tex;

	IZ_UINT m_Pitch;
	IZ_UINT8* m_Data;

	IZ_FLOAT m_DivW;
	IZ_FLOAT m_DivH;
};

#endif	// #if !defined(__LAT_LONG_MAP_PROXY_H__)
