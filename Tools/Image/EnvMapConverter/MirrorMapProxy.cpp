#include "MirrorMapProxy.h"

CMirrorMapProxy::CMirrorMapProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
: CTexProxy(tex, type)
{
	m_Tex = tex;

	m_Pitch = 0;
	m_Data = IZ_NULL;

	IZ_UINT width = m_Tex->GetWidth();
	IZ_UINT height = m_Tex->GetHeight();

	m_DivW = 1.0f / (width - 1);
	m_DivH = 1.0f / (height - 1);
}

CMirrorMapProxy::~CMirrorMapProxy()
{
	if (m_Data) {
		m_Tex->Unlock(0);
		m_Data = IZ_NULL;
	}
}

// 反射ベクトルからUVを取得.
void CMirrorMapProxy::getUVFromRef(
	const izanagi::SVector& ref,
	IZ_FLOAT& u, IZ_FLOAT& v)
{
	// NOTE
	// R = 2 * (N・I) * N - I
	// I = (0, 0, 1)
	// N・I = N.z
	//
	// R.z = 2 * N.z * N.z - 1 = 2 * pow(N.z, 2) - 1
	// <-> N.z = sqrt((R.z + 1) / 2)
	// 
	// R.x = 2 * N.z * N.x - 0 
	// <-> N.x = R.x / (2 * N.z) = R.x / sqrt(2 * (R.z + 1))
	//
	// R.y = 2 * N.z * N.y - 0
	// <-> N.y = R.y / (2 * N.z) = R.y / sqrt(2 * (R.z + 1))
	

	IZ_FLOAT div = 1.0f / ::sqrtf(2.0f * (1.0f + ref.z));

	u = ref.x * div;
	v = ref.y * div;

	u = (u + 1.0f) * 0.5f;
	v = (v + 1.0f) * 0.5f;
}

// XYから反射ベクトルを取得.
IZ_BOOL CMirrorMapProxy::getRef(
	IZ_UINT x, IZ_UINT y,
	izanagi::SVector& ref)
{
	// [-1:1]に変換
	IZ_FLOAT u = 2.0f * x * m_DivW - 1.0f;
	IZ_FLOAT v = 2.0f * y * m_DivH - 1.0f;

	IZ_BOOL isValid = (::sqrtf(u * u + v * v) <= 1.0f);

	if (isValid) {
		izanagi::SVector nml;
		{
			nml.x = u;
			nml.y = v;
			nml.z = ::sqrtf(1.0f- u * u - v * v);
		}

		// NOTE
		// R = 2 * (N・I) * N - I
		// I = (0, 0, 1)
		// N・I = N.z
		//
		// R.x = 2 * N.z * N.x - 0
		// R.y = 2 * N.z * N.y - 0
		// R.z = 2 * N.z * N.z - 1

		ref.x = 2.0f * nml.z * nml.x;
		ref.y = 2.0f * nml.z * nml.y;
		ref.z = 2.0f * nml.z * nml.z - 1.0f;

		// 念のため
		izanagi::SVector::Normalize(ref, ref);
	}

	return isValid;
}

// UVから色を取得.
void CMirrorMapProxy::getColor(
	IZ_FLOAT u, IZ_FLOAT v,
	SFloatColor& color)
{
	IZ_UINT x = static_cast<IZ_UINT>(u * (m_Tex->GetWidth() - 1));
	IZ_UINT y = static_cast<IZ_UINT>(v * (m_Tex->GetHeight() - 1));

	if (m_Data == IZ_NULL) {
		IZ_ASSERT(m_Tex);
		m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
	}

	IZ_BOOL isFloat = izanagi::CGraphUtil::IsFloatPixelFormat(m_Tex->GetPixelFormat());
	IZ_UINT bpp = izanagi::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());

	IZ_UINT8* data = m_Data + m_Pitch * y + x * bpp;

	if (isFloat) {
		IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

		color.r = d[0];
		color.g = d[1];
		color.b = d[2];
		color.a = d[3];
	}
	else {
		color.r = NormalizeColor(data[0]);
		color.g = NormalizeColor(data[1]);
		color.b = NormalizeColor(data[2]);
		color.a = NormalizeColor(data[3]);
	}
}

// 指定位置に描きこみ.
void CMirrorMapProxy::putColor(
	IZ_UINT x, IZ_UINT y,
	const SFloatColor& color)
{
	if (m_Data == IZ_NULL) {
		IZ_ASSERT(m_Tex);
		m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
	}

	IZ_BOOL isFloat = izanagi::CGraphUtil::IsFloatPixelFormat(m_Tex->GetPixelFormat());
	IZ_UINT bpp = izanagi::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());

	IZ_UINT8* data = m_Data + m_Pitch * y + x * bpp;

	if (isFloat) {
		IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

		d[0] = color.r;
		d[1] = color.g;
		d[2] = color.b;
		d[3] = color.a;
	}
	else {
		data[0] = color.getAsUint8_R();
		data[1] = color.getAsUint8_G();
		data[2] = color.getAsUint8_B();
		data[3] = color.getAsUint8_A();
	}
}

// XYが正しい位置かどうか.
IZ_BOOL CMirrorMapProxy::isValid(IZ_UINT x, IZ_UINT y) const
{
	// 半径
	IZ_UINT r = m_Tex->GetWidth() >> 1;

	// 中心原点にする
	x -= r;
	y -= r;

	return ((x * x + y * y) <= r * r);
}
