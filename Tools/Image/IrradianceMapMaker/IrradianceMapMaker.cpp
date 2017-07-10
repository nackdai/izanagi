// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include <vector>
#include <atomic>
#include "izToolKit.h"
#include "izMath.h"

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

    izanagi::math::SVector3 convertDirToUV(const izanagi::math::SVector3& dir)
    {
        auto temp = ::atan2(dir.x, dir.z);
        auto r = izanagi::math::SVector3::Length(dir);

        // Account for discontinuity
        auto phi = ((temp >= 0) ? temp : (temp + 2 * IZ_MATH_PI));
        auto theta = ::acos(dir.y / r);

        // Map to [0,1]x[0,1] range and reverse Y axis
        auto u = phi / (2 * IZ_MATH_PI);
        //auto v = 1 - theta / IZ_MATH_PI;
        auto v = theta / IZ_MATH_PI;

        izanagi::math::CVector3 uv(u, v, 0);

        return std::move(uv);
    }

    izanagi::math::SVector3 convertUVToDir(float u, float v)
    {
        // u = phi / 2PI
        // => phi = 2PI * u;
        auto phi = 2 * IZ_MATH_PI * u;

        // v = 1 - theta / PI
        // => theta = (1 - v) * PI;
        //auto theta = (1 - v) * IZ_MATH_PI;
        auto theta = v * IZ_MATH_PI;

        izanagi::math::CVector3 dir;

        dir.y = ::cos(theta);

        auto xz = ::sqrt(1 - dir.y * dir.y);

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
    EquirectTexture& in,
    const izanagi::math::CVector3& n,
    const izanagi::math::CVector3& t,
    const izanagi::math::CVector3& b)
{
    // NOTE
    // http://www.codinglabs.net/article_physically_based_rendering.aspx

    static const int step = 10;

    static const float phi_step = IZ_MATH_PI2 / step;
    static const float theta_step = IZ_MATH_PI1_2 / step;

    int count = 0;

    izanagi::math::CVector3 conv;

    // normalの方向を基準とした正規直交基底(w, u, v)を作る.
    // この基底に対する半球内で次のレイを飛ばす.

    for (float phi = 0.0f; phi < IZ_MATH_PI2; phi += phi_step) {
        for (float theta = 0.0f; theta < IZ_MATH_PI1_2; theta += theta_step) {
            float x = ::sin(theta) * ::sin(phi);
            float y = ::sin(theta) * ::cos(phi);
            float z = ::cos(theta);

            izanagi::math::CVector3 dir = t * x + b * y + n * z;
            dir.Normalize();

            // テクスチャからサンプリング.
            izanagi::math::CVector3 c(in.read(dir));

            conv += c * ::cos(theta) * ::sin(theta);

            count++;
        }
    }

    // divide by samplings.
    conv = IZ_MATH_PI * conv / (float)count;
    
    return std::move(conv);
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

int main(int argc, char* argv[])
{
    int nRetCode = 0;

    int width, height, comp;
    auto data = stbi_loadf("studio015.hdr", &width, &height, &comp, 0);

    EquirectTexture in(width, height, data);

    int bytes = sizeof(float) * 3 * width * height;
    float* outdata = (float*)malloc(bytes);

    EquirectTexture out(width, height, outdata);

    std::atomic<int> count = 0;

//#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        int cur = count.fetch_add(1);
        if ((cur % 10) == 0) {
            IZ_PRINTF("%.3f %%\n", cur / (float)height * 100);
        }

        for (int x = 0; x < width; x++) {
            float u = x / (float)(width - 1);
            float v = y / (float)(height - 1);

            auto n = in.convertUVToDir(u, v);

            auto t = getOrthoVector(n);

            auto b = cross(n, t);

            //auto clr = in.read(n);
            auto clr = computeRadiance(in, n, t, b);

            out.write(clr, n);
        }
    }

    stbi_write_hdr("test.hdr", width, height, 3, outdata);

    return nRetCode;
}
