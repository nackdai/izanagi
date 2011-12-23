#include "TexProxy.h"

// テクスチャプロキシ作成
CTexProxy* CTexProxy::CreateTexProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
{
	CTexProxy* ret = IZ_NULL;

	switch (type) {
	case EnvMapTypeShperical:
		ret = new CSphericalTexProxy(tex, type);
		break;
	case EnvMapTypeAngular:
		ret = new CAngularTexProxy(tex, type);
		break;
	case EnvMapTypeCross:
		ret = new CCrossTexProxy(tex, type);
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

CTexProxy::CTexProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
{
	m_Tex = tex;
	m_Type = type;
	m_Pitch = 0;
	m_Data = IZ_NULL;

	IZ_UINT width = tex->GetWidth();
	IZ_UINT height = tex->GetHeight();

	m_HalfW = width >> 1;
	m_HalfH = height >> 1;

	m_DivW = 1.0f / width;
	m_DivH = 1.0f / height;
}

CTexProxy::~CTexProxy()
{
	if (m_Data) {
		IZ_ASSERT(m_Tex);
		m_Tex->Unlock(0);
	}
}

// データ取得
void CTexProxy::GetAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT ret[4])
{
	static const IZ_FLOAT div = 1.0f / 255.0f;

	IZ_UINT8* src = GetData(x, y);

	if (IsFloatFormat()) {
		IZ_FLOAT* p = (IZ_FLOAT*)src;

		ret[0] = p[0];
		ret[1] = p[1];
		ret[2] = p[2];
		ret[3] = p[3];
	}
	else {
		ret[0] = src[0] * div;
		ret[1] = src[1] * div;
		ret[2] = src[2] * div;
		ret[3] = src[3] * div;
	}
}

// データセット
void CTexProxy::SetAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT data[4])
{
	IZ_UINT8* dst = GetData(x, y);

	if (IsFloatFormat()) {
		IZ_FLOAT* p = (IZ_FLOAT*)dst;

		p[0] = data[0];
		p[1] = data[1];
		p[2] = data[2];
		p[3] = data[3];
	}
	else {
		dst[0] = (IZ_UINT8)izanagi::CMath::Clamp(data[0] * 255.0f, 0.0f, 255.0f);
		dst[0] = (IZ_UINT8)izanagi::CMath::Clamp(data[1] * 255.0f, 0.0f, 255.0f);
		dst[0] = (IZ_UINT8)izanagi::CMath::Clamp(data[2] * 255.0f, 0.0f, 255.0f);
		dst[0] = (IZ_UINT8)izanagi::CMath::Clamp(data[3] * 255.0f, 0.0f, 255.0f);
	}
}

// データ加算
void CTexProxy::AddAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT data[4])
{
	IZ_UINT8* dst = GetData(x, y);

	if (IsFloatFormat()) {
		IZ_FLOAT* p = (IZ_FLOAT*)dst;

		p[0] += data[0];
		p[1] += data[1];
		p[2] += data[2];
		p[3] += data[3];
	}
	else {
		dst[0] += (IZ_UINT8)izanagi::CMath::Clamp(data[0] * 255.0f, 0.0f, 255.0f);
		dst[0] += (IZ_UINT8)izanagi::CMath::Clamp(data[1] * 255.0f, 0.0f, 255.0f);
		dst[0] += (IZ_UINT8)izanagi::CMath::Clamp(data[2] * 255.0f, 0.0f, 255.0f);
		dst[0] += (IZ_UINT8)izanagi::CMath::Clamp(data[3] * 255.0f, 0.0f, 255.0f);
	}
}

// データ加工
void CTexProxy::ScaleAt(IZ_UINT x, IZ_UINT y, IZ_FLOAT scale)
{
	IZ_ASSERT((0.0f <= scale) && (scale <= 1.0f));

	if (scale == 1.0f) {
		// 何もしない
		return;
	}

	IZ_UINT8* data = GetData(x, y);

	if (IsFloatFormat()) {
		IZ_FLOAT* p = (IZ_FLOAT*)data;

		p[0] *= scale;
		p[1] *= scale;
		p[2] *= scale;
		p[3] *= scale;
	}
	else {
		data[0] = (IZ_UINT8)(data[0] * scale);
		data[1] = (IZ_UINT8)(data[1] * scale);
		data[2] = (IZ_UINT8)(data[2] * scale);
		data[3] = (IZ_UINT8)(data[3] * scale);
	}
}

namespace {
	template <typename _T>
	void _Interp(
		IZ_UINT begin, IZ_UINT end,
		IZ_UINT8* beginData, IZ_UINT8* endData,
		IZ_UINT stride)
	{
		_T* b = reinterpret_cast<_T*>(beginData);
		_T* e = reinterpret_cast<_T*>(endData);

		for (IZ_UINT i = 1; i < end - begin - 1; i++) {
			_T* dst = (_T*)(beginData + stride * i);

			IZ_FLOAT f = i / (IZ_FLOAT)(end - begin);

			dst[0] = static_cast<_T>(b[0] * (1.0f - f) + e[0] * f);
			dst[1] = static_cast<_T>(b[1] * (1.0f - f) + e[1] * f);
			dst[2] = static_cast<_T>(b[2] * (1.0f - f) + e[2] * f);
			dst[3] = static_cast<_T>(b[3] * (1.0f - f) + e[3] * f);
		}
	}
}

// データ補間（水平方向）
void CTexProxy::InterpH(
	IZ_UINT y,
	IZ_UINT begin, IZ_UINT end)
{
	if (begin == end) {
		// 何もしない
		return;
	}

	IZ_ASSERT(begin < end);

	IZ_UINT8* beginData = GetData(begin, y);
	IZ_UINT8* endData = GetData(end, y);

	IZ_UINT bpp = izanagi::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());

	if (IsFloatFormat()) {
		_Interp<IZ_FLOAT>(
			begin, end,
			beginData, endData,
			bpp);
	}
	else {
		_Interp<IZ_UINT8>(
			begin, end,
			beginData, endData,
			bpp);
	}
}

// データ補間（垂直方向）
void CTexProxy::InterpV(
	IZ_UINT x,
	IZ_UINT begin, IZ_UINT end)
{
	if (begin == end) {
		// 何もしない
		return;
	}

	IZ_ASSERT(begin < end);

	IZ_UINT8* beginData = GetData(x, begin);
	IZ_UINT8* endData = GetData(x, end);

	if (IsFloatFormat()) {
		_Interp<IZ_FLOAT>(
			begin, end,
			beginData, endData,
			m_Pitch);
	}
	else {
		_Interp<IZ_UINT8>(
			begin, end,
			beginData, endData,
			m_Pitch);
	}
}

// floatフォーマットかどうか
IZ_BOOL CTexProxy::IsFloatFormat() const
{
	IZ_BOOL ret = izanagi::CGraphUtil::IsFloatPixelFormat(m_Tex->GetPixelFormat());
	return ret;
}

IZ_UINT8* CTexProxy::GetData(IZ_UINT x, IZ_UINT y)
{
	if (m_Data == IZ_NULL) {
		m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
	}

	IZ_ASSERT(x < m_Tex->GetWidth());
	IZ_ASSERT(y < m_Tex->GetHeight());

	IZ_UINT bpp = izanagi::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());
	IZ_UINT8* src = m_Data + m_Pitch * y + x * bpp;

	return src;
}

///////////////////////////////////////////////////

static const izanagi::CVector e(0.0f, 0.0f, 1.0f);

// データ取得
void CSphericalTexProxy::GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4])
{
	IZ_FLOAT normX = (refParam.nml.x + 1.0f) * 0.5f;
	IZ_FLOAT normY = (-refParam.nml.y + 1.0f) * 0.5f;

	IZ_UINT x = min(normX * m_Tex->GetWidth(), m_Tex->GetWidth() - 1);
	IZ_UINT y = min(normY * m_Tex->GetHeight(), m_Tex->GetHeight() - 1);

	CTexProxy::GetAt(x, y, ret);
}

// 反射ベクトル取得
IZ_BOOL CSphericalTexProxy::GetReflection(
	IZ_INT x, IZ_INT y,
	SReflectionParam& refParam)
{
	IZ_ASSERT(m_Type == EnvMapTypeShperical);

	IZ_BOOL ret = IZ_FALSE;

	izanagi::SVector::SetZero(refParam.ref);

	IZ_FLOAT normX = (x - m_HalfW) * m_DivW * 2.0f;
	IZ_FLOAT normY = -(y - m_HalfH) * m_DivH * 2.0f;

	if (::sqrtf(normX * normX + normY * normY) <= 1.0f) {
		// 法線を計算する

		// X方向
		// 0 - 2π
		refParam.theta = IZ_MATH_PI + normX * IZ_MATH_PI;

		// Y方向
		// -π/2 - π/2
		refParam.phi = IZ_MATH_PI1_2 * normY;

		refParam.nml.x = izanagi::CMath::CosF(refParam.phi) * izanagi::CMath::SinF(refParam.theta);
		refParam.nml.y = izanagi::CMath::SinF(refParam.phi);
		refParam.nml.z = izanagi::CMath::CosF(refParam.phi) * izanagi::CMath::CosF(refParam.theta);

		// 念のため
		izanagi::SVector::Normalize(refParam.nml, refParam.nml);

		IZ_FLOAT dot = izanagi::SVector::Dot(refParam.nml, e);

		// NOTE
		// 反射ベクトル
		// f = -e + 2 * n * (e・n)

		refParam.ref.x = -e.x + 2.0f * refParam.nml.x * dot;
		refParam.ref.y = -e.y + 2.0f * refParam.nml.y * dot;
		refParam.ref.z = -e.z + 2.0f * refParam.nml.z * dot;

		// 念のため
		izanagi::SVector::Normalize(refParam.ref, refParam.ref);

		ret = IZ_TRUE;
	}

	return ret;
}

// 有効な位置かどうか
IZ_BOOL CSphericalTexProxy::IsValidPos(IZ_INT x, IZ_INT y)
{
	IZ_FLOAT normX = (x - m_HalfW) * m_DivW * 2.0f;
	IZ_FLOAT normY = -(y - m_HalfH) * m_DivH * 2.0f;

	if (::sqrtf(normX * normX + normY * normY) <= 1.0f) {
		return IZ_TRUE;
	}
	
	return IZ_FALSE;
}

///////////////////////////////////////////////////

// データ取得
void CAngularTexProxy::GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4])
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
}

// 反射ベクトル取得
IZ_BOOL CAngularTexProxy::GetReflection(
	IZ_INT x, IZ_INT y,
	SReflectionParam& refParam)
{
	IZ_ASSERT(m_Type == EnvMapTypeAngular);

	izanagi::SVector::SetZero(refParam.ref);

	IZ_FLOAT normX = x * m_DivW;
	IZ_FLOAT normY = -(y - m_HalfH) * m_DivH;

	// 法線を計算する

	// X方向
	// 0 - 2π
	refParam.theta = IZ_MATH_PI2 - (1.0f - normX) * IZ_MATH_PI2;

	// Y方向
	// -π/2 - π/2
	refParam.phi = IZ_MATH_PI * normY;

	refParam.nml.x = izanagi::CMath::CosF(refParam.phi) * izanagi::CMath::SinF(refParam.theta);
	refParam.nml.y = izanagi::CMath::SinF(refParam.phi);
	refParam.nml.z = izanagi::CMath::CosF(refParam.phi) * izanagi::CMath::CosF(refParam.theta);

	// 念のため
	izanagi::SVector::Normalize(refParam.nml, refParam.nml);

	IZ_FLOAT dot = izanagi::SVector::Dot(refParam.nml, e);

	// NOTE
	// 反射ベクトル
	// f = -e + 2 * n * (e・n)

	refParam.ref.x = -e.x + 2.0f * refParam.nml.x * dot;
	refParam.ref.y = -e.y + 2.0f * refParam.nml.y * dot;
	refParam.ref.z = -e.z + 2.0f * refParam.nml.z * dot;

	return IZ_TRUE;
}

// 有効な位置かどうか
IZ_BOOL CAngularTexProxy::IsValidPos(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

///////////////////////////////////////////////////

// データ取得
void CCrossTexProxy::GetAt(const SReflectionParam& refParam, IZ_FLOAT ret[4])
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
}

// 反射ベクトル取得
IZ_BOOL CCrossTexProxy::GetReflection(
	IZ_INT x, IZ_INT y,
	SReflectionParam& refParam)
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
	return IZ_FALSE;
}

// 有効な位置かどうか
IZ_BOOL CCrossTexProxy::IsValidPos(IZ_INT x, IZ_INT y)
{
	// TODO
	return IZ_TRUE;
}
