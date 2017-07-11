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

inline izanagi::math::SVector3 computeGGXRadiance(
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

    auto r1 = sampler.nextSample();
    auto r2 = sampler.nextSample();

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

    auto N = n;
    auto R = N;
    auto V = R;

    auto L = 2.0f * izanagi::math::SVector3::Dot(V, H) * H - V;
    L.Normalize();

    float NdotL = std::max<float>(izanagi::math::SVector3::Dot(N, L), 0.0f);

    izanagi::math::CVector3 clr;

    if (NdotL > 0.0f) {
        clr = in.read(L);
        clr *= NdotL;

        weight += NdotL;
    }

    return std::move(clr);
}