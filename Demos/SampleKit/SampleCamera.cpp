#include "SampleCamera.h"

using namespace izanagi;
using namespace sample;

void CSampleCamera::Init(
    const izanagi::math::CVector& vPos,
    const izanagi::math::CVector& vRef,
    const izanagi::math::CVector& vUp,
    IZ_FLOAT fNear, IZ_FLOAT fFar,
    IZ_FLOAT fFov,
    IZ_FLOAT fAspect)
{
    izanagi::CVectorCamera::Init(
        vPos,
        vRef,
        fNear, fFar,
        fFov,
        fAspect);
}

void CSampleCamera::Update()
{
    izanagi::CVectorCamera::Update();
}

void CSampleCamera::Dolly(float fDistScale)
{
    const izanagi::SCameraParam& param = GetParam();

    // 視点と注視点の距離
    float fLength = izanagi::math::SVector::Length2(
        param.pos,
        param.ref);

    // 視点から注視点への方向
    izanagi::math::CVector vDir;
    izanagi::math::SVector::Sub(
        vDir,
        param.ref,
        param.pos);
    izanagi::math::SVector::Div(vDir, vDir, fLength);

    // スケーリング
    // 注視点までの距離に応じる
    fDistScale = fDistScale * fLength * 0.01f;
    izanagi::math::SVector::Scale(vDir, vDir, fDistScale);

    izanagi::math::SVector pos;

    // 新しい視点
    izanagi::math::SVector::Add(
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

    izanagi::math::CVector v1(
        pt1.x, pt1.y,
        _ProjectionToSphere(radius, pt1.x, pt1.y));
    izanagi::math::SVector::Normalize(v1, v1);

    izanagi::math::CVector v2(
        pt2.x, pt2.y,
        _ProjectionToSphere(radius, pt2.x, pt2.y));
    izanagi::math::SVector::Normalize(v2, v2);

    // 回転軸
    izanagi::math::SVector axis; 
    izanagi::math::SVector::Cross(axis, v1, v2);
    izanagi::math::SVector::Normalize(axis, axis);

    // カメラの回転状態に合わせて軸も回転
    izanagi::math::SMatrix::Apply(axis, axis, GetTransform());

    // 回転の角度
    // NOTE
    // V1・V2 = |V1||V2|cosθ = cosθ (|V1| = |V2| = 1)
    // θ = acos(cosθ)
    // => θ = acos(cosθ) = acos(V1・V2)
    IZ_FLOAT theta = ::acos(izanagi::math::SVector::Dot(v1, v2));

    // 回転
    izanagi::CVectorCamera::Rotate(axis, theta);
}

void CSampleCamera::Move(float fOffsetX, float fOffsetY)
{
    const izanagi::SCameraParam& param = GetParam();

    // 移動ベクトル
    izanagi::math::SVector vOffset;
    izanagi::math::SVector::Set(vOffset, fOffsetX, 0.0f, fOffsetY, 0.0f);

    // カメラの回転を考慮する
    izanagi::math::SVector vDir;
    izanagi::math::SVector::Sub(vDir, param.ref, param.pos);
    izanagi::math::SVector::Normalize(vDir, vDir);
    vDir.y = 0.0f;

    izanagi::math::SMatrix mRot;
    izanagi::math::SMatrix::SetUnit(mRot);
    izanagi::math::SMatrix::GetRotMatrixFromVector(mRot, vDir);

    izanagi::math::SMatrix::Apply(vOffset, vOffset, mRot);

    izanagi::math::SVector pos = param.pos;
    izanagi::math::SVector::Add(pos, param.pos, vOffset);

    SetPos(pos);
}
