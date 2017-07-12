// ImageBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include <vector>
#include <atomic>
#include "izToolKit.h"
#include "izMath.h"

#include <cmdline.h>

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

struct Options {
    std::string input;
    std::string output_diffuse;
    std::string output_specular;
    std::string output_integ;
    int width{ 0 };
    int height{ 0 };
    int samples{ 100 };
    float roughness{ 0.5f };
};

bool parseOption(
    int argc, char* argv[],
    cmdline::parser& cmd,
    Options& opt)
{
    {
        cmd.add<string>("input", 'i', "input environment map", true);
        cmd.add<string>("output", 'o', "output filename base", true, "result");
        cmd.add<int>("width", 'w', "output map width", false);
        cmd.add<int>("height", 'h', "output map height", false);
        cmd.add<int>("samples", 's', "ray samples", false, opt.samples);
        cmd.add<float>("roughness", 'r', "material roughness", false, opt.roughness);
        cmd.add<string>("help", '?', "print usage", false);
    }

    bool isCmdOk = cmd.parse(argc, argv);

    if (argc == 1 || cmd.exist("help")){
        cerr << cmd.usage();
        return false;
    }

    if (!isCmdOk){
        cerr << cmd.error() << endl << cmd.usage();
        return false;
    }

    if (cmd.exist("input")) {
        opt.input = cmd.get<string>("input");
    }
    if (cmd.exist("output")) {
        auto output = cmd.get<string>("output");

        opt.output_diffuse = output + "_diff.hdr";
        opt.output_specular = output + "_spec.hdr";
        opt.output_integ = output + "_integ.hdr";
    }
    if (cmd.exist("width")) {
        opt.width = cmd.get<int>("width");
    }
    if (cmd.exist("height")) {
        opt.height = cmd.get<int>("height");
    }
    if (cmd.exist("samples")) {
        opt.samples = cmd.get<int>("samples");
        opt.samples = std::max<int>(1, opt.samples);
    }
    if (cmd.exist("roughness")) {
        opt.roughness = cmd.get<float>("roughness");
        opt.roughness = izanagi::math::CMath::Clamp<float>(opt.roughness, 0, 1);
    }

    return true;
}

float* loadImage(
    const char* input,
    int& width,
    int& height)
{
    int comp = 0;

    if (stbi_is_hdr(input)) {
        auto data = stbi_loadf(input, &width, &height, &comp, 0);
        return data;
    }
    else {
        auto data = stbi_load(input, &width, &height, &comp, 0);

        if (data) {
            auto bytes = sizeof(float) * (width * height * 3);
            float* fdata = (float*)malloc(bytes);

#pragma omp parallel for
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int idx = y * width + x;
                    idx *= 3;

                    fdata[idx + 0] = data[idx + 0] / (float)255;
                    fdata[idx + 1] = data[idx + 1] / (float)255;
                    fdata[idx + 2] = data[idx + 2] / (float)255;
                }
            }

            STBI_FREE(data);

            return fdata;
        }

        return nullptr;
    }
}

int main(int argc, char* argv[])
{
    cmdline::parser cmd;
    Options opt;

    if (!parseOption(argc, argv, cmd, opt)) {
        return 1;
    }

    int width, height;
    auto data = loadImage(opt.input.c_str(), width, height);

    if (!data) {
        return 1;
    }

    EquirectTexture in(width, height, data);

    if (opt.width == 0) {
        opt.width = width;
    }
    if (opt.height == 0) {
        opt.height = height;
    }

    int bytes = sizeof(float) * 3 * opt.width * opt.height;
    float* bufDiffuse = (float*)malloc(bytes);
    float* bufSpecular = (float*)malloc(bytes);

    EquirectTexture outDiff(opt.width, opt.height, bufDiffuse);
    EquirectTexture outSpec(opt.width, opt.height, bufSpecular);

    std::atomic<int> count = 0;

#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        int cur = count.fetch_add(1);
        if ((cur % 10) == 0) {
           IZ_PRINTF("%.3f %%\n", cur / (float)height * 100);
        }

        for (int x = 0; x < width; x++) {
            float u = (x + 0.5f) / (float)width;
            float v = (y + 0.5f) / (float)height;

            auto n = EquirectTexture::convertUVToDir(u, v);
            auto t = getOrthoVector(n);
            auto b = cross(n, t);

            float weight = 0.0f;

            izanagi::math::CVector3 convDiff;
            izanagi::math::CVector3 convSpec;

            for (int i = 0; i < opt.samples; i++) {
                XorShift sampler((y * height * 4 + x * 4) * opt.samples + i + 1);

                auto diffuse = computeLambertRadiance(weight, sampler, in, opt.roughness, n, t, b);
                auto specular = computeGGXPrefilter(weight, sampler, in, opt.roughness, n, t, b);

                if (isInvalidColor(diffuse)) {
                    //IZ_PRINTF("Invalid(%d/%d[%d])\n", x, y, i);
                    continue;
                }

                convDiff += diffuse;
                convSpec += specular;
            }

            convDiff /= weight;
            convSpec /= weight;

            outDiff.write(convDiff, u, v);
            outSpec.write(convSpec, u, v);
        }
    }

    stbi_write_hdr(opt.output_diffuse.c_str(), opt.width, opt.height, 3, bufDiffuse);
    stbi_write_hdr(opt.output_specular.c_str(), opt.width, opt.height, 3, bufSpecular);

    if (data) {
        free(data);
    }
    if (bufDiffuse) {
        free(bufDiffuse);
    }
    if (bufSpecular) {
        free(bufSpecular);
    }
    
    // Reset counter.
    count = 0;

    {
        // Fixed size.
        width = 256;
        height = 256;

        bytes = sizeof(float) * 3 * width * height;
        float* outdata = (float*)malloc(bytes);

#pragma omp parallel for
        for (int y = 0; y < height; y++) {
            int cur = count.fetch_add(1);
            if ((cur % 10) == 0) {
                IZ_PRINTF("%.3f %%\n", cur / (float)height * 100);
            }

            for (int x = 0; x < width; x++) {
                float u = (x + 0.5f) / (float)width;
                float v = (y + 0.5f) / (float)height;

                // Fixed normal.
                izanagi::math::CVector3 n(0, 0, 1);
                auto t = getOrthoVector(n);
                auto b = cross(n, t);

                izanagi::math::CVector3 integ;

                for (int i = 0; i < opt.samples; i++) {
                    XorShift sampler((y * height * 4 + x * 4) * opt.samples + i + 1);

                    auto clr = computeIntegrateBRDF(
                        u, v,
                        sampler,
                        n, t, b);

                    integ += clr;
                }

                integ /= (float)opt.samples;

                // Revert y axis.
                int idx = (height - 1 - y) * width + x;
                idx *= 3;

                outdata[idx + 0] = integ.x;
                outdata[idx + 1] = integ.y;
                outdata[idx + 2] = integ.z;
            }
        }

        stbi_write_hdr(opt.output_integ.c_str(), width, height, 3, outdata);
        free(outdata);
    }

    return 0;
}
