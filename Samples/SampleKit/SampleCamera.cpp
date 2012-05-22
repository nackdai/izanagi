#include "SampleCamera.h"

using namespace izanagi;
using namespace sample;

void CSampleCamera::Init(
	const izanagi::CVector& vPos,
	const izanagi::CVector& vRef,
	const izanagi::CVector& vUp,
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
	float fLength = izanagi::SVector::Length2(
		param.pos,
		param.ref);

	// 視点から注視点への方向
	izanagi::CVector vDir;
	izanagi::SVector::Sub(
		vDir,
		param.ref,
		param.pos);
	izanagi::SVector::Div(vDir, vDir, fLength);

	// スケーリング
	// 注視点までの距離に応じる
	fDistScale = fDistScale * fLength * 0.01f;
	izanagi::SVector::Scale(vDir, vDir, fDistScale);

	izanagi::SVector pos;

	// 新しい視点
	izanagi::SVector::Add(
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

	izanagi::CVector v1(
		pt1.x, pt1.y,
		_ProjectionToSphere(radius, pt1.x, pt1.y));
	izanagi::SVector::Normalize(v1, v1);

	izanagi::CVector v2(
		pt2.x, pt2.y,
		_ProjectionToSphere(radius, pt2.x, pt2.y));
	izanagi::SVector::Normalize(v2, v2);

	// 回転軸
	izanagi::SVector axis; 
	izanagi::SVector::Cross(axis, v1, v2);
	izanagi::SVector::Normalize(axis, axis);

	// カメラの回転状態に合わせて軸も回転
	izanagi::SMatrix::Apply(axis, axis, GetTransform());

	// 回転の角度
	// NOTE
	// V1・V2 = |V1||V2|cosθ = cosθ (|V1| = |V2| = 1)
	// θ = acos(cosθ)
	// => θ = acos(cosθ) = acos(V1・V2)
	IZ_FLOAT theta = ::acos(izanagi::SVector::Dot(v1, v2));

	// 回転
	izanagi::CVectorCamera::Rotate(axis, theta);
}

void CSampleCamera::Move(float fOffsetX, float fOffsetY)
{
	const izanagi::SCameraParam& param = GetParam();

	// 移動ベクトル
	izanagi::SVector vOffset;
	izanagi::SVector::Set(vOffset, fOffsetX, 0.0f, fOffsetY, 0.0f);

	// カメラの回転を考慮する
	izanagi::SVector vDir;
	izanagi::SVector::Sub(vDir, param.ref, param.pos);
	izanagi::SVector::Normalize(vDir, vDir);
	vDir.y = 0.0f;

	izanagi::SMatrix mRot;
	izanagi::SMatrix::SetUnit(mRot);
	izanagi::SMatrix::GetRotMatrixFromVector(mRot, vDir);

	izanagi::SMatrix::Apply(vOffset, vOffset, mRot);

	izanagi::SVector pos = param.pos;
	izanagi::SVector::Add(pos, param.pos, vOffset);

	SetPos(pos);
}
