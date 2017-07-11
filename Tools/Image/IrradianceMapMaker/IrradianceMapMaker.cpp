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
#include "equirect.h"
#include "radiance.h"

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

            float weight = 0.0f;

            for (int i = 0; i < samples; i++) {
                XorShift sampler((y * height * 4 + x * 4) * samples + i + 1);

                float roughness = 0.2f;
                auto clr = computeLambertRadiance(weight, sampler, in, roughness, n, t, b);
                //auto clr = computeGGXRadiance(weight, sampler, in, roughness, n, t, b);

                if (isInvalidColor(clr)) {
                    IZ_PRINTF("Invalid(%d/%d[%d])\n", x, y, i);
                    continue;
                }

                conv += clr;
            }

            conv /= weight;

            out.write(conv, u, v);
        }
    }

    stbi_write_hdr("test.hdr", width, height, 3, outdata);

    return nRetCode;
}
