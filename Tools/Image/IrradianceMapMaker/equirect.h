#pragma once

#include "izMath.h"

class EquirectTexture {
public:
    EquirectTexture() {}
    EquirectTexture(int width, int height, float* data)
    {
        init(width, height, data);
    }
    ~EquirectTexture() {}

public:
    void init(int width, int height, float* data)
    {
        m_width = width;
        m_height = height;
        m_data = data;

        m_counts.resize(width * height);
    }

    int width() const
    {
        return m_width;
    }
    int height() const
    {
        return m_height;
    }
    float* data()
    {
        return m_data;
    }

    izanagi::math::SVector3 read(const izanagi::math::SVector3& dir) const
    {
        auto uv = convertDirToUV(dir);
        return read(uv.x, uv.y);
    }

    izanagi::math::SVector3 read(float u, float v) const
    {
        auto pos = computeIndexFromUV(u, v);

        izanagi::math::SVector3 color;
        color.Set(
            m_data[pos + 0],
            m_data[pos + 1],
            m_data[pos + 2]);

        return std::move(color);
    }

    void write(
        const izanagi::math::SVector3& clr,
        const izanagi::math::SVector3& dir)
    {
        auto uv = convertDirToUV(dir);
        write(clr, uv.x, uv.y);
    }

    void write(
        const izanagi::math::SVector3& clr,
        float u, float v)
    {
        auto pos = computeIndexFromUV(u, v);

        int idx = pos / 3;
        int cnt = m_counts[idx];

        if (cnt == 0) {
            m_data[pos + 0] = clr.x;
            m_data[pos + 1] = clr.y;
            m_data[pos + 2] = clr.z;
        }
        else {
            m_data[pos + 0] = (m_data[pos + 0] * cnt + clr.x) / (float)(cnt + 1);
            m_data[pos + 1] = (m_data[pos + 1] * cnt + clr.y) / (float)(cnt + 1);
            m_data[pos + 2] = (m_data[pos + 2] * cnt + clr.z) / (float)(cnt + 1);
        }

        m_counts[idx] += 1;
    }

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

    int computeIndexFromUV(float u, float v) const
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
    std::vector<int> m_counts;
};
