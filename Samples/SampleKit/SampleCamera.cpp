#include "SampleCamera.h"

using namespace izanagi;
using namespace sample;

void CSampleCamera::Dolly(float fDistScale)
{
    const izanagi::SCameraParam& param = GetParam();

    // 視点と注視点の距離
    float fLength = izanagi::math::SVector4::Length2(
        param.pos,
        param.ref);

    // 視点から注視点への方向
    izanagi::math::CVector4 vDir;
    izanagi::math::SVector4::Sub(
        vDir,
        param.ref,
        param.pos);
    izanagi::math::SVector4::Div(vDir, vDir, fLength);

    // スケーリング
    // 注視点までの距離に応じる
    fDistScale = fDistScale * fLength * 0.01f;
    izanagi::math::SVector4::Scale(vDir, vDir, fDistScale);

    izanagi::math::SVector4 pos;

    // 新しい視点
    izanagi::math::SVector4::Add(
        pos,
        GetPos(),
        vDir);

    SetPos(pos);
}

namespace {
    IZ_FLOAT _ProjectionToSphere(
        IZ_FLOAT radius,
        IZ_FLOAT x,
        IZ_FLOAT y)
    {
        IZ_FLOAT z = 0.0f;
        IZ_FLOAT dist = ::sqrtf(x * x + y * y);

        // r * 1/√2 の点で双曲線と接する内側と外側

        if (dist < radius * 0.70710678118654752440f) {
            // 内側

            // NOTE
            // r * r = x * x + y * y + z * z
            // <=> z * z = r * r - (x * x + y * y)
            z = ::sqrtf(radius * radius - dist * dist);
        }
        else {
            // 外側
            IZ_FLOAT t = radius * radius * 0.5f;
            z = t / dist;
        }

        return z;
    }
}

void CSampleCamera::Rotate(
    const izanagi::CFloatPoint& pt1,
    const izanagi::CFloatPoint& pt2)
{
    static const IZ_FLOAT radius = 0.8f;

    const izanagi::SCameraParam& param = GetParam();

    // スクリーン上の２点からトラックボール上の点を計算する
    // GLUTと同じ方法

    izanagi::math::CVector4 v1(
        pt1.x, pt1.y,
        _ProjectionToSphere(radius, pt1.x, pt1.y),
        0.0f);
    izanagi::math::SVector4::Normalize(v1, v1);

    izanagi::math::CVector4 v2(
        pt2.x, pt2.y,
        _ProjectionToSphere(radius, pt2.x, pt2.y),
        0.0f);
    izanagi::math::SVector4::Normalize(v2, v2);

    // 回転軸
    izanagi::math::SVector4 axis; 
    izanagi::math::SVector4::Cross(axis, v1, v2);
    izanagi::math::SVector4::Normalize(axis, axis);

    // カメラの回転状態に合わせて軸も回転
    izanagi::math::SMatrix44::Apply(axis, axis, GetTransform());

    // 回転の角度
    // NOTE
    // V1・V2 = |V1||V2|cosθ = cosθ (|V1| = |V2| = 1)
    // θ = acos(cosθ)
    // => θ = acos(cosθ) = acos(V1・V2)
    IZ_FLOAT theta = ::acos(izanagi::math::SVector4::Dot(v1, v2));

    // 回転
    izanagi::CVectorCamera::RotatePos(axis, theta);
}

void CSampleCamera::Move(float fOffsetX, float fOffsetY)
{
    const izanagi::SCameraParam& param = GetParam();

    // 移動ベクトル
    izanagi::math::SVector4 vOffset;
    //izanagi::math::SVector4::Set(vOffset, fOffsetX, 0.0f, fOffsetY, 0.0f);
    izanagi::math::SVector4::Set(vOffset, fOffsetX, fOffsetY, 0.0f, 0.0f);

    // カメラの回転を考慮する
    izanagi::math::SVector4 vDir;
    izanagi::math::SVector4::Sub(vDir, param.ref, param.pos);
    izanagi::math::SVector4::Normalize(vDir, vDir);
    vDir.y = 0.0f;

    izanagi::math::SMatrix44 mRot;
    izanagi::math::SMatrix44::SetUnit(mRot);
    izanagi::math::SMatrix44::GetRotMatrixFromVector(mRot, vDir);

    izanagi::math::SMatrix44::Apply(vOffset, vOffset, mRot);

    izanagi::math::CVector4 pos(param.pos);
    izanagi::math::CVector4 at(param.ref);

    izanagi::math::SVector4::Add(pos, pos, vOffset);
    SetPos(pos);

    izanagi::math::SVector4::Add(at, at, vOffset);
    SetAt(at);
}
