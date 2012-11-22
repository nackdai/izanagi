#if !defined(__ANGULAR_MAP_PROXY_H__)
#define __ANGULAR_MAP_PROXY_H__

#include "MirrorMapProxy.h"

/** アンギュラーマッププロキシ
 */
class CAngularMapProxy : public CMirrorMapProxy {
public:
	CAngularMapProxy(
		izanagi::tool::CTextureLite* tex,
		EnvMapType type);
	virtual ~CAngularMapProxy();

public:
	/** 反射ベクトルからUVを取得.
	 */
	virtual void getUVFromRef(
		const izanagi::SVector& ref,
		IZ_FLOAT& u, IZ_FLOAT& v);

	/** XYから反射ベクトルを取得.
	 */
	virtual void getRef(
		IZ_UINT x, IZ_UINT y,
		izanagi::SVector& ref,
		izanagi::graph::E_GRAPH_CUBE_TEX_FACE face = izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM);
};

#endif	// #if !defined(__ANGULAR_MAP_PROXY_H__)
