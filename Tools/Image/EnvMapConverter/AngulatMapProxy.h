#if !defined(__TEX_PROXY_H__)
#define __TEX_PROXY_H__

#include "izToolKit.h"
#include "izMath.h"
#include "EnvMapConverterDefs.h"

/** 反射パラメータ
 */
struct SReflectionParam {
	izanagi::SVector ref;
	izanagi::SVector nml;
	IZ_FLOAT theta;
	IZ_FLOAT phi;

	// thetaを0.0f - 1.0fに正規化
	IZ_FLOAT NormalizeTheta()
	{
		// 0 - 2π
		IZ_FLOAT ret = theta / IZ_MATH_PI2;
		return ret;
	}

	// phiを0.0f - 1.0fに正規化
	IZ_FLOAT NormalizePhi()
	{
		// -π/2 - π/2
		IZ_FLOAT ret = phi / IZ_MATH_PI1_2;
		ret = (-ret + 1.0f) * 0.5f;
		return ret;
	}
};

/** テクスチャプロキシ
 */
class CTexProxy {
public:
	// テクスチャプロキシ作成
	static CTexProxy* CreateTexProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type);

	// テクスチャプロキシ破棄
	static void DeleteTexProxy(CTexProxy*& tex);

public:
	CTexProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type);
	virtual ~CTexProxy();

public:
	// データ取得
	void GetAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT ret[4]);
	virtual void GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4]) = 0;

	// データセット
	void SetAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT data[4]);

	// データ加算
	void AddAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT data[4]);

	// データ加工
	void ScaleAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT scale);

	// データ補間（水平方向）
	void InterpH(
		IZ_UINT y,
		IZ_UINT begin, IZ_UINT end);

	// データ補間（垂直方向）
	void InterpV(
		IZ_UINT x,
		IZ_UINT begin, IZ_UINT end);

	// floatフォーマットかどうか
	IZ_BOOL IsFloatFormat() const;

	// 反射ベクトル取得
	virtual IZ_BOOL GetReflection(
		IZ_INT x, IZ_INT y,
		SReflectionParam& refParam) = 0;

	// 有効な位置かどうか
	virtual IZ_BOOL IsValidPos(IZ_INT x, IZ_INT y) = 0;

protected:
	IZ_UINT8* GetData(IZ_UINT x, IZ_UINT y);

protected:
	izanagi::izanagi_tk::CTextureLite* m_Tex;
	EnvMapType m_Type;

	IZ_UINT m_Pitch;
	IZ_UINT8* m_Data;

	IZ_INT m_HalfW;
	IZ_INT m_HalfH;

	IZ_FLOAT m_DivW;
	IZ_FLOAT m_DivH;
};

/** テクスチャプロキシ for Spherical
 */
class CSphericalTexProxy : public CTexProxy {
public:
	CSphericalTexProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type)
	: CTexProxy(tex, type)
	{
	}

	virtual ~CSphericalTexProxy() {}

public:
	// データ取得
	virtual void GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4]);

	// 反射ベクトル取得
	virtual IZ_BOOL GetReflection(
		IZ_INT x, IZ_INT y,
		SReflectionParam& refParam);

	// 有効な位置かどうか
	virtual IZ_BOOL IsValidPos(IZ_INT x, IZ_INT y);
};

/** テクスチャプロキシ for Angular
 */
class CAngularTexProxy : public CTexProxy {
public:
	CAngularTexProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type)
	: CTexProxy(tex, type)
	{
	}

	virtual ~CAngularTexProxy() {}

public:
	// データ取得
	virtual void GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4]);

	// 反射ベクトル取得
	virtual IZ_BOOL GetReflection(
		IZ_INT x, IZ_INT y,
		SReflectionParam& refParam);

	// 有効な位置かどうか
	virtual IZ_BOOL IsValidPos(IZ_INT x, IZ_INT y);
};

// TODO
/** テクスチャプロキシ for Cross
 */
class CCrossTexProxy : public CTexProxy {
public:
	CCrossTexProxy(
		izanagi::izanagi_tk::CTextureLite* tex,
		EnvMapType type)
	: CTexProxy(tex, type)
	{
	}

	virtual ~CCrossTexProxy() {}

public:
	// データ取得
	virtual void GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4]);

	// 反射ベクトル取得
	virtual IZ_BOOL GetReflection(
		IZ_INT x, IZ_INT y,
		SReflectionParam& refParam);

	// 有効な位置かどうか
	virtual IZ_BOOL IsValidPos(IZ_INT x, IZ_INT y);
};

#endif	// #if !defined(__TEX_PROXY_H__)
