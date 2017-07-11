// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include <vector>
#include <atomic>
#include "izToolKit.h"
#include "izMath.h"

#include "xorshift.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

namespace {
    inline void _DispUsage()
    {
    }
}

#ifdef VGOTO
#undef VGOTO
#endif

#define VGOTO(b)    if (!(b)) { IZ_ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

class EquirectTexture {
public:
    EquirectTexture(int width, int height, float* data)
        : m_width(width), m_height(height), m_data(data)
    {}
    ~EquirectTexture() {}

public:
    izanagi::math::SVector3 read(const izanagi::math::SVector3& nml);
    izanagi::math::SVector3 read(float u, float v);

    void write(
        const izanagi::math::SVector3& clr,
        const izanagi::math::SVector3& dir);
    void write(
        const izanagi::math::SVector3& clr,
        float u, float v);

    static izanagi::math::SVector3 convertDirToUV(const izanagi::math::SVector3& dir)
    {
        auto temp = ::atan2(dir.x, dir.z);
        auto r = izanagi::math::SVector3::Length(dir);

        // Account for discontinuity
        auto phi = ((temp >= 0) ? temp : (temp + 2 * IZ_MATH_PI));
        auto theta = ::acos(dir.y / r);

        // Map to [0,1]x[0,1] range and reverse Y axis
        auto u = phi / (2 * IZ_MATH_PI);
        auto v = theta / IZ_MATH_PI;

        izanagi::math::CVector3 uv(u, v, 0);

        return std::move(uv);
    }

    static izanagi::math::SVector3 convertUVToDir(float u, float v)
    {
        // u = phi / 2PI
        // => phi = 2PI * u;
        auto phi = 2 * IZ_MATH_PI * u;

        // v = theta / PI
        // => theta = v * PI;
        auto theta = v * IZ_MATH_PI;

        izanagi::math::CVector3 dir;

        dir.y = ::cos(theta);

        auto xz = ::sqrt(1 - dir.y * dir.y);    // sin(theta)

        dir.x = xz * ::sin(phi);
        dir.z = xz * ::cos(phi);

        // 念のため...
        dir.Normalize();

        return std::move(dir);
    }

    int computeIndexFromUV(float u, float v)
    {
        int x = izanagi::math::CMath::Clamp<int>(u * m_width, 0, m_width - 1);
        int y = izanagi::math::CMath::Clamp<int>(v * m_height, 0, m_height - 1);

        int idx = y * m_width + x;
        idx *= 3;

        return idx;
    }

private:
    int m_width{ 0 };
    int m_height{ 0 };
    float* m_data{ nullptr };
};

izanagi::math::SVector3 EquirectTexture::read(const izanagi::math::SVector3& dir)
{
    auto uv = convertDirToUV(dir);
    return read(uv.x, uv.y);
}

izanagi::math::SVector3 EquirectTexture::read(float u, float v)
{
    auto pos = computeIndexFromUV(u, v);

    izanagi::math::SVector3 color;
    color.Set(
        m_data[pos + 0],
        m_data[pos + 1],
        m_data[pos + 2]);

    return std::move(color);
}

void EquirectTexture::write(
    const izanagi::math::SVector3& clr,
    const izanagi::math::SVector3& dir)
{
    auto uv = convertDirToUV(dir);
    write(clr, uv.x, uv.y);
}

void EquirectTexture::write(
    const izanagi::math::SVector3& clr,
    float u, float v)
{
    auto pos = computeIndexFromUV(u, v);

    m_data[pos + 0] = clr.x;
    m_data[pos + 1] = clr.y;
    m_data[pos + 2] = clr.z;
}

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

izanagi::math::SVector3 computeRadiance(
    XorShift& sampler,
    EquirectTexture& in,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    // NOTE
    // http://www.codinglabs.net/article_physically_based_rendering.aspx

    izanagi::math::CVector3 conv;

    // normalの方向を基準とした正規直交基底(w, u, v)を作る.
    // この基底に対する半球内で次のレイを飛ばす.

    // コサイン項を使った重点的サンプリング.
    const float r1 = 2 * IZ_MATH_PI * sampler.nextSample();
    const float r2 = sampler.nextSample();
    const float r2s = sqrt(r2);

    const float x = ::cosf(r1) * r2s;
    const float y = ::sinf(r1) * r2s;
    const float z = ::sqrtf(1.0f - r2);

    izanagi::math::CVector3 wo = t * x + b * y + n * z;
    wo.Normalize();

    // テクスチャからサンプリング.
    izanagi::math::CVector3 bsdf(in.read(wo));
    bsdf /= IZ_MATH_PI;

    auto c = izanagi::math::SVector3::Dot(n, wo);
    c = ::fabsf(c);

    auto pdf = c / IZ_MATH_PI;

    auto ret = bsdf * c / pdf;

    return std::move(ret);
}

inline izanagi::math::SVector3 cross(const izanagi::math::SVector3& v1, const izanagi::math::SVector3& v2)
{
    izanagi::math::SVector3 ret;
    ret.Set(
        v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
        v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
        v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);

    return std::move(ret);
}

inline bool isValid(float f)
{
    bool b = isnan(f);
    if (!b) {
        b = isinf(f);
    }

    return !b;
}

inline bool isInvalid(const izanagi::math::SVector3& v)
{
    bool b0 = !isValid(v.x);
    bool b1 = !isValid(v.y);
    bool b2 = !isValid(v.z);

    return b0 || b1 || b2;
}

inline bool isInvalidColor(const izanagi::math::SVector3& v)
{
    bool b = isInvalid(v);
    if (!b) {
        if (v.x < 0 || v.y < 0 || v.z < 0) {
            b = true;
        }
    }

    return b;
}

int main(int argc, char* argv[])
{
    int nRetCode = 0;

    int width, height, comp;
#if 1
    //auto data = stbi_loadf("studio015.hdr", &width, &height, &comp, 0);
    auto data = stbi_loadf("harbor.hdr", &width, &height, &comp, 0);

    EquirectTexture in(width, height, data);
#else
    auto data = stbi_load("input.png", &width, &height, &comp, 0);
    float* f = (float*)malloc(width * height * 3 * sizeof(float));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            idx *= 3;

            f[idx + 0] = data[idx + 0] / (float)255;
            f[idx + 1] = data[idx + 1] / (float)255;
            f[idx + 2] = data[idx + 2] / (float)255;
        }
    }
    EquirectTexture in(width, height, f);
#endif

    int bytes = sizeof(float) * 3 * width * height;
    float* outdata = (float*)malloc(bytes);

    EquirectTexture out(width, height, outdata);

    std::atomic<int> count = 0;

    static const int samples = 100;

#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        int cur = count.fetch_add(1);
        if ((cur % 10) == 0) {
           IZ_PRINTF("%.3f %%\n", cur / (float)height * 100);
        }

        izanagi::math::CVector3 conv;

        for (int x = 0; x < width; x++) {
            float u = x / (float)(width - 1);
            float v = y / (float)(height - 1);

            auto n = EquirectTexture::convertUVToDir(u, v);

            auto t = getOrthoVector(n);

            auto b = cross(n, t);

            int count = 0;

            for (int i = 0; i < samples; i++) {
                XorShift sampler((y * height * 4 + x * 4) * samples + i + 1);

                auto clr = computeRadiance(sampler, in, n, t, b);

                if (isInvalidColor(clr)) {
                    IZ_PRINTF("Invalid(%d/%d[%d])\n", x, y, i);
                    continue;
                }

                conv += clr;
                count++;
            }

            conv /= (float)count;

            out.write(conv, u, v);
        }
    }

    stbi_write_hdr("test.hdr", width, height, 3, outdata);

    return nRetCode;
}
