#pragma once

#include "izMath.h"
#include "equirect.h"
#include "xorshift.h"

// 直行ベクトルを計算.
inline izanagi::math::SVector3 getOrthoVector(const izanagi::math::SVector3& n)
{
    izanagi::math::SVector3 p;

    // NOTE
    // dotを計算したときにゼロになるようなベクトル.
    // k は normalize 計算用.
    if (::abs(n.z) > 0.0f) {
        float k = ::sqrt(n.y * n.y + n.z * n.z);
        p.x = 0;
        p.y = -n.z / k;
        p.z = n.y / k;
    }
    else {
        float k = ::sqrt(n.x * n.x + n.y * n.y);
        p.x = n.y / k;
        p.y = -n.x / k;
        p.z = 0;
    }

    return std::move(p);
}

inline izanagi::math::SVector3 computeLambertRadiance(
    float& weight,
    XorShift& sampler,
    const EquirectTexture& in,
    float roughness,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    // NOTE
    // http://www.codinglabs.net/article_physically_based_rendering.aspx
    // https://learnopengl.com/#!PBR/IBL/Diffuse-irradiance

    // なぜか、うまくいかないので、レイトレの計算をそのまま持ってきた...

    izanagi::math::CVector3 conv;

    // Importance sampling.
    const float r1 = 2 * IZ_MATH_PI * sampler.nextSample();
    const float r2 = sampler.nextSample();
    const float r2s = sqrt(r2);

    const float x = ::cosf(r1) * r2s;
    const float y = ::sinf(r1) * r2s;
    const float z = ::sqrtf(1.0f - r2);

    izanagi::math::CVector3 wo = t * x + b * y + n * z;
    wo.Normalize();

    izanagi::math::CVector3 bsdf(in.read(wo));
    bsdf /= IZ_MATH_PI;

    auto c = izanagi::math::SVector3::Dot(n, wo);
    c = ::fabsf(c);

    auto pdf = c / IZ_MATH_PI;

    auto ret = bsdf * c / pdf;
    weight += 1.0f;

    return std::move(ret);
}

inline float computeGGXSmithG1(float roughness, const izanagi::math::SVector3& v, const izanagi::math::SVector3& n)
{
    // NOTE
    // http://computergraphics.stackexchange.com/questions/2489/correct-form-of-the-ggx-geometry-term
    // http://gregory-igehy.hatenadiary.com/entry/2015/02/26/154142

    float a = roughness;

    float costheta = ::abs(izanagi::math::SVector3::Dot(v, n));

    float sintheta = ::sqrt(1 - izanagi::math::CMath::Clamp<float>(costheta * costheta, 0, 1));
    float tan = costheta > 0 ? sintheta / costheta : 0;

    float denom = ::sqrt(1 + a * a * tan * tan);

    float ret = 2 / (1 + denom);

    return ret;
}

inline izanagi::math::SVector3 importanceSamplingGGX(
    float r1, float r2,
    float roughness,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    float a = roughness * roughness;

    float phi = 2.0 * IZ_MATH_PI * r1;
    float cosTheta = sqrt((1.0f - r2) / (1.0f + (a * a - 1.0f) * r2));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates
    izanagi::math::SVector3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    H = t * H.x + b * H.y + n * H.z;

    return std::move(H);
}

inline izanagi::math::SVector3 computeGGXPrefilter(
    float& weight,
    XorShift& sampler,
    const EquirectTexture& in,
    const float roughness,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    // NOTE
    // https://learnopengl.com/#!PBR/IBL/Specular-IBL
    // http://gregory-igehy.hatenadiary.com/entry/2015/02/26/154142
    // https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf

    auto r1 = sampler.nextSample();
    auto r2 = sampler.nextSample();

    auto H = importanceSamplingGGX(r1, r2, roughness, n, t, b);

    auto N = n;
    auto R = N;
    auto V = R;

    auto L = 2.0f * izanagi::math::SVector3::Dot(V, H) * H - V;
    L.Normalize();

    izanagi::math::CVector3 clr;

#if 1
    float NoL = std::max<float>(izanagi::math::SVector3::Dot(N, L), 0.0f);

    if (NoL > 0.0f) {
        clr = in.read(L);
        clr *= NoL;
        weight += NoL;

    }
#else
    float NoV = izanagi::math::SVector3::Dot(N, V);
    float NoL = izanagi::math::SVector3::Dot(N, L);
    float NoH = izanagi::math::SVector3::Dot(N, H);
    float VoH = izanagi::math::SVector3::Dot(V, H);

    // TODO
    izanagi::math::CVector3 SpecularColor(1, 1, 1);

    if (NoL > 0.0f) {
        clr = in.read(L);

        // G 項
        float G1 = computeGGXSmithG1(roughness, L, N);
        float G2 = computeGGXSmithG1(roughness, V, N);
        float G = G1 * G2;

        // F 項
        float Fc = pow(1 - VoH, 5);
        auto F = (1 - Fc) * SpecularColor + Fc;

        // pdf = (D * NoH) / VoH
        // L = ks * NoL * (DGF / (4 * NoL * NoV)) / pdf
        //   = ks * NoL * (DGF / (4 * NoL * NoV)) * (VoH / (D * NoH))
        //   = ks * (GF / NoV) * (VoH / NoH)

        clr = clr * (G * F / NoV) * (VoH / NoH);

        weight += 1.0f;
    }
#endif

    return std::move(clr);
}
inline float GeometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

inline float GeometrySmith(
    const izanagi::math::CVector3& N,
    const izanagi::math::CVector3& V,
    const izanagi::math::CVector3& L,
    const float roughness)
{
    float NdotV = std::max<float>(izanagi::math::SVector3::Dot(N, V), 0.0);
    float NdotL = std::max<float>(izanagi::math::SVector3::Dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

inline izanagi::math::SVector3 computeIntegrateBRDF(
    float u, float v,
    XorShift& sampler,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    // NOTE
    // https://learnopengl.com/#!PBR/IBL/Specular-IBL
    // https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf

    // NOTE
    // roughness(v)
    // |
    // |
    // |
    // +--------> NoV(u)

    const float NdotV = izanagi::math::CMath::Clamp<float>(u, 0, 1);
    const float roughness = izanagi::math::CMath::Clamp<float>(v, 0, 1);

    izanagi::math::CVector3 V;
    V.x = sqrt(1.0 - NdotV * NdotV);    // sin
    V.y = 0.0;
    V.z = NdotV;  // cos

    izanagi::math::CVector3 N(0.0, 0.0, 1.0);

    auto r1 = sampler.nextSample();
    auto r2 = sampler.nextSample();

    auto H = importanceSamplingGGX(r1, r2, roughness, n, t, b);

    auto L = 2.0f * izanagi::math::SVector3::Dot(V, H) * H - V;
    L.Normalize();

    float NdotL = std::max<float>(L.z, 0.0);
    float NdotH = std::max<float>(H.z, 0.0);
    float VdotH = std::max<float>(izanagi::math::SVector3::Dot(V, H), 0.0);

    float A = 0.0;
    float B = 0.0;

    if (NdotL > 0.0)
    {
        float G = GeometrySmith(N, V, L, roughness);
        float G_Vis = (G * VdotH) / (NdotH * NdotV + 0.0001f);
        float Fc = pow(1.0 - VdotH, 5.0);

        A += (1.0 - Fc) * G_Vis;
        B += Fc * G_Vis;
    }

    return izanagi::math::CVector3(A, B, 0);
}