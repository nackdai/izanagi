#if !defined(__CROSS_MAP_PROXY_H__)
#define __CROSS_MAP_PROXY_H__

#include <vector>
#include "CubeMapProxy.h"

/** クロスマッププロキシ
 */
class CCrossMapProxy : public CCubeMapProxy {
public:
    static IZ_UINT GetWidthPerFace(IZ_UINT width)
    {
        IZ_ASSERT(width % 3 == 0);
        return width / 3;
    }
    static IZ_UINT GetHeightPerFace(IZ_UINT height)
    {
        IZ_ASSERT(height % 4 == 0);
        return height / 4;
    }

public:
	CCrossMapProxy(
		izanagi::tool::CTextureLite* tex,
		EnvMapType type);
	virtual ~CCrossMapProxy();

public:
	/** XYから反射ベクトルを取得.
	 */
	virtual void getRef(
		IZ_UINT x, IZ_UINT y,
		izanagi::math::SVector& ref,
		izanagi::graph::E_GRAPH_CUBE_TEX_FACE face = izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM);

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
    izanagi::tool::CTextureLite* m_Tex;

    IZ_UINT m_Width;
    IZ_UINT m_Height;

    IZ_UINT m_WidthPerFace;
    IZ_UINT m_HeightPerFace;
};

#endif	// #if !defined(__CROSS_MAP_PROXY_H__)
