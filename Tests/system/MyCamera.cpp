#include <stdafx.h>
#include "MyCamera.h"

CMyCamera CMyCamera::s_cInstance;

void CMyCamera::Init(
	const izanagi::CVector& vPos,
	const izanagi::CVector& vRef,
	const izanagi::CVector& vUp,
	IZ_FLOAT fNear, IZ_FLOAT fFar,
	IZ_FLOAT fFov,
	IZ_FLOAT fAspect)
{
	m_cCamera.Init(
		vPos, vRef, vUp,
		fNear, fFar,
		fFov,
		fAspect);

	// 注視点から視点への方向
	izanagi::CVector vDir;
	izanagi::SVector::Sub(
		vDir,
		m_cCamera.GetParam().pos,
		m_cCamera.GetParam().ref);

	izanagi::SVector::Normalize(vDir, vDir);

	m_fPitch = -asinf(vDir.y);
	m_fYaw = (vDir.x == 0.0f ? IZ_MATH_PI : IZ_MATH_PI + atan2f(vDir.z, vDir.x));
	m_fRoll = 0.0f;
}

void CMyCamera::Dolly(float fDistScale)
{
	// 視点と注視点の距離
	float fLength = izanagi::SVector::Length2(m_cCamera.GetParam().pos, m_cCamera.GetParam().ref);

	// 視点から注視点への方向
	izanagi::CVector vDir;
	izanagi::SVector::Sub(
		vDir,
		m_cCamera.GetParam().ref,
		m_cCamera.GetParam().pos);
	izanagi::SVector::Div(vDir, vDir, fLength);

	// スケーリング
	// 注視点までの距離に応じる
	fDistScale = fDistScale * fLength * 0.01f;
	izanagi::SVector::Scale(vDir, vDir, fDistScale);

	izanagi::SVector pos = m_cCamera.GetParam().pos;

	// 新しい視点
	izanagi::SVector::Add(
		pos,
		m_cCamera.GetParam().pos,
		vDir);

	m_cCamera.SetPos(pos);
}

#if 1
void CMyCamera::Rotate(float fLatitude, float fLongitude)
{
	// 注視点から視点への方向
	izanagi::CVector vDir;
	izanagi::SVector::Sub(
		vDir,
		m_cCamera.GetParam().pos,
		m_cCamera.GetParam().ref);

	// 半径
	float fRadius = izanagi::SVector::Length(vDir);

	m_fPitch += fLatitude;
	m_fYaw -= fLongitude;

	// ロール回転はしない
	float fRoll = 0.0f;

	float cosY = cosf(m_fYaw);
	float sinY = sinf(m_fYaw);
	float cosP = cosf(m_fPitch);
	float sinP = sinf(m_fPitch);
	float cosR = cosf(m_fRoll);
	float sinR = sinf(m_fRoll);

	izanagi::SVector vFwd;
	vFwd.x = cosP * sinY;
	vFwd.y = sinP;
	vFwd.z = cosP * cosY;
	vFwd.w = 0.0f;

	izanagi::SVector::Normalize(vFwd, vFwd);
	izanagi::SVector::ScaleXYZ(vFwd, vFwd, -fRadius);

	izanagi::SVector pos = m_cCamera.GetParam().pos; 

	izanagi::SVector::AddXYZ(pos, m_cCamera.GetParam().ref, vFwd);

	m_cCamera.SetPos(pos);

#if 0
	m_cCamera.up.x = -cosY * sinR - sinY * sinP * cosR;
	m_cCamera.up.y = cosP * cosR;
	m_cCamera.up.z = sinY * sinR - sinP * cosR * cosY;
#endif
}
#else
void CMyCamera::Rotate(float fLatitude, float fLongitude)
{
	// 注視点から視点への方向
	izanagi::CVector vDir;
	izanagi::SVector::Sub(
		vDir,
		m_cCamera.pos,
		m_cCamera.ref);

	// 半径
	float fRadius = SVector::Length(vDir);

	izanagi::SVector::Normalize(vDir, vDir);

	// 緯度
	float fTheta = MATH_PID2 - asinf(vDir.v[1]);	// PI / 2 - arcsin(y)

	// 経度
	float fPhi = 0.0f;

	if (izanagi::IsNearyEqualZero(fTheta) || izanagi::IsNearyEqual(fTheta, MATH_PI)) {
		// sin(fTheta) = 0.0f になるので
		fPhi = 0.0f;
	}
	else {
		fPhi = atan2f(vDir.v[2], vDir.v[0]);	// arctan(z/x)
	}

	fTheta += fLatitude;
	fPhi += fLongitude;

#if 0
	fTheta = izanagi::Clamp<float>(fTheta, 0.0f, MATH_PI);
#else
	// ジンバルロック対策
	fTheta = izanagi::Clamp<float>(fTheta, 0.0f + MATH_PI / 180.0f, MATH_PI - MATH_PI / 180.0f);
#endif
	
	if (fPhi < -MATH_PI2) {
		fPhi += MATH_PI2;
	}
	else if (fPhi > MATH_PI2) {
		fPhi -= MATH_PI2;
	}

	// ベタに・・・
	float x = fRadius * sinf(fTheta) * cosf(fPhi);
	float y = fRadius * cosf(fTheta);
	float z = fRadius * sinf(fTheta) * sinf(fPhi);

	// で、x、y、z は注視点からのオフセットなので・・・
	x += m_cCamera.ref.v[0];
	y += m_cCamera.ref.v[1];
	z += m_cCamera.ref.v[2];

	m_cCamera.pos.Set(x, y, z, 1.0f);
}
#endif

void CMyCamera::Move(float fOffsetX, float fOffsetY)
{
	// 移動ベクトル
	izanagi::SVector vOffset;
	izanagi::SVector::Set(vOffset, fOffsetX, 0.0f, fOffsetY, 0.0f);

	// カメラの回転を考慮する
	izanagi::SVector vDir;
	izanagi::SVector::Sub(vDir, m_cCamera.GetParam().ref, m_cCamera.GetParam().pos);
	izanagi::SVector::Normalize(vDir, vDir);
	vDir.y = 0.0f;

	izanagi::SMatrix mRot;
	izanagi::SetUnitMatrix(mRot);
	izanagi::GetRotMatrixFromVector(mRot, vDir);

	izanagi::ApplyMatrix(vOffset, vOffset, mRot);

	izanagi::SVector pos = m_cCamera.GetParam().pos;
	izanagi::SVector ref = m_cCamera.GetParam().ref;

	izanagi::SVector::Add(ref, m_cCamera.GetParam().ref, vOffset);
	izanagi::SVector::Add(pos, m_cCamera.GetParam().pos, vOffset);

	m_cCamera.SetPos(pos);
	m_cCamera.SetAt(ref);
}
