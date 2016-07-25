#include "LatLongMapProxy.h"

// NOTE
// -π <= φ <= π
// -π/2 <= θ <= π/2

// NOTE
// 0.0f <= u,v <= 1.0f

// NOTE
// x = cosθsinφ
// y = sinθ
// z = cosθcosφ

// NOTE
// latitude = 緯度 / longitude = 経度

// NOTE
//      |-
//  ----+----> φ
//      |+
//      θ

CLatLongMapProxy::CLatLongMapProxy(
    izanagi::tool::CTextureLite* tex,
    EnvMapType type)
: CTexProxy(type)
{
    m_Tex = tex;

    m_Pitch = 0;
    m_Data = IZ_NULL;

    IZ_UINT width = m_Tex->GetWidth();
    IZ_UINT height = m_Tex->GetHeight();

    m_DivW = 1.0f / (width - 1);
    m_DivH = 1.0f / (height - 1);

    m_IsFloat = izanagi::graph::CGraphUtil::IsFloatPixelFormat(m_Tex->GetPixelFormat());
    m_Bpp = izanagi::graph::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());
}

CLatLongMapProxy::~CLatLongMapProxy()
{
    if (m_Data) {
        m_Tex->Unlock(0);
        m_Data = IZ_NULL;
    }
}

// 反射ベクトルからUVを取得.
void CLatLongMapProxy::getUVFromRef(
    const izanagi::math::SVector4& ref,
    IZ_FLOAT& u, IZ_FLOAT& v)
{
    // y = sin(θ) -> θ=asin(y)
    // θ : [-pi/2, pi/2]
    IZ_FLOAT latitude = ::asinf(ref.y);

    // Normalize [-pi/2, pi/2] -> [-1, 1]
    v = latitude / IZ_MATH_PI1_2;

    IZ_FLOAT cosLat = ::cosf(latitude);

    // z = cos(θ)cos(φ) -> cos(φ) = z / cos(θ) -> φ = acos(z / cos(θ))
    IZ_FLOAT cosLong = ref.z / (cosLat + 0.00001f);
    cosLong = izanagi::math::CMath::Clamp(cosLong, -1.0f, 1.0f);

    IZ_FLOAT longitude = ::acosf(cosLong);
    
    // NOTE
    // acosf -> 範囲[0, +π].
    // φ : [-π, π]
    longitude = (ref.x < 0.0f ? -longitude : longitude);

    // Normalize [-π, π] -> [-1, 1]
    u = longitude / IZ_MATH_PI;

    // Normalize [-1, 1] -> [0, 1]
    u = (u + 1.0f) * 0.5f;
    v = (v + 1.0f) * 0.5f;
}

// XYから反射ベクトルを取得.
void CLatLongMapProxy::getRef(
    IZ_UINT x, IZ_UINT y,
    izanagi::math::SVector4& ref,
    izanagi::graph::E_GRAPH_CUBE_TEX_FACE face/*= izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM*/)
{
    // [-1:1]
    IZ_FLOAT u = 2.0f * (x * m_DivW) - 1.0f;
    IZ_FLOAT v = 2.0f * (y * m_DivH) - 1.0f;

    u = u * IZ_MATH_PI;     // longitude
    v = v * IZ_MATH_PI1_2;  // latitude

    IZ_FLOAT sinLat = ::sinf(v);
    IZ_FLOAT cosLat = ::cosf(v);
    IZ_FLOAT sinLong = ::sinf(u);
    IZ_FLOAT cosLong = ::cosf(u);

    ref.x = cosLat * sinLong;
    ref.y = sinLat;
    ref.z = cosLat * cosLong;

    ref.x = izanagi::math::CMath::Clamp(ref.x, -1.0f, 1.0f);
    ref.y = izanagi::math::CMath::Clamp(ref.y, -1.0f, 1.0f);
    ref.z = izanagi::math::CMath::Clamp(ref.z, -1.0f, 1.0f);

    izanagi::math::SVector4::Normalize(ref, ref);
}

// UVから色を取得.
void CLatLongMapProxy::getColor(
    IZ_FLOAT u, IZ_FLOAT v,
    SFloatColor& color)
{
    IZ_UINT x = static_cast<IZ_UINT>(u * (m_Tex->GetWidth() - 1));
    IZ_UINT y = static_cast<IZ_UINT>(v * (m_Tex->GetHeight() - 1));

    if (m_Data == IZ_NULL) {
        IZ_ASSERT(m_Tex);
        m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
    }

#if 0
    IZ_UINT8* data = m_Data + m_Pitch * y + x * m_Bpp;

    if (m_IsFloat) {
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
#else
#ifdef ENABLE_BILINEAR
    CTexProxy::getBiLinearColor(
        m_IsFloat,
        m_Tex->GetWidth(), m_Tex->GetHeight(),
        m_Pitch, m_Bpp,
        m_Data,
        u, v,
        color);
#else
    CTexProxy::getColor(
        m_IsFloat,
        x, y,
        m_Pitch, m_Bpp,
        m_Data,
        color);
#endif
#endif
}

// 指定位置に描きこみ.
void CLatLongMapProxy::putColor(
    IZ_UINT x, IZ_UINT y,
    const SFloatColor& color)
{
    if (m_Data == IZ_NULL) {
        IZ_ASSERT(m_Tex);
        m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
    }

#if 0
    IZ_UINT8* data = m_Data + m_Pitch * y + x * m_Bpp;

    if (m_IsFloat) {
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
#else
    CTexProxy::putColor(
        m_IsFloat,
        x, y,
        m_Pitch, m_Bpp,
        color,
        m_Data);
#endif
}

// XYが正しい位置かどうか.
IZ_BOOL CLatLongMapProxy::isValid(IZ_UINT x, IZ_UINT y) const
{
    return IZ_TRUE;
}
