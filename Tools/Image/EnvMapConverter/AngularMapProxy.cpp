#include "AngularMapProxy.h"

// NOTE
// Mirrorとちがって、Angularは全方位をカバーするので
// 単純にθ、φに落とし込めない
//
// 全方向に向く反射ベクトルがあり、
// そのZ値は、z = cosα と考えることができる
// （αは全方位をカバーする値でθ、φから算出できるはずのもの）
//
// Angularは全方位を線形に描画にしているので、αはAngularMapのU,V位置に比例する
// つまり、r = sqrt(U * U + V * V) に比例する
// α = r * π となる

CAngularMapProxy::CAngularMapProxy(
	izanagi::izanagi_tk::CTextureLite* tex,
	EnvMapType type)
: CMirrorMapProxy(tex, type)
{
}

CAngularMapProxy::~CAngularMapProxy()
{
}

// 反射ベクトルからUVを取得.
void CAngularMapProxy::getUVFromRef(
	const izanagi::SVector& ref,
	IZ_FLOAT& u, IZ_FLOAT& v)
{
	if (izanagi::CMath::IsNearyEqual(ref.z, 1.0f)) {
		// zが手前に向いているときは中心
		u = 0.0f;
		v = 0.0f;
	}
	else {
		// z = cosα
		// [-π:π]
		IZ_FLOAT alpha = ::acosf(ref.z);

		IZ_FLOAT r = alpha / IZ_MATH_PI;

		// sin(alpha)を計算
		IZ_FLOAT sin = ::sqrtf(1.0f - ref.z * ref.z);

		// NOTE
		// ref.x = sin * u / r <-> u = ref.x * r / sin
		// ref.y = sin * v / r <-> v = ref.y * r / sin

		u = ref.x * r / (sin + IZ_MATH_EPSILON);
		v = ref.y * r / (sin + IZ_MATH_EPSILON);

		u = izanagi::CMath::Clamp(u, -1.0f, 1.0f);
		v = izanagi::CMath::Clamp(v, -1.0f, 1.0f);
	}

	u = (u + 1.0f) * 0.5f;
	v = (v + 1.0f) * 0.5f;
}

// XYから反射ベクトルを取得.
void CAngularMapProxy::getRef(
	IZ_UINT x, IZ_UINT y,
	izanagi::SVector& ref,
	izanagi::E_GRAPH_CUBE_TEX_FACE face/*= izanagi::E_GRAPH_CUBE_TEX_FACE_NUM*/)
{
	// [-1:1]に変換
	IZ_FLOAT u = 2.0f * x * m_DivW - 1.0f;
	IZ_FLOAT v = 2.0f * y * m_DivH - 1.0f;

	IZ_FLOAT r = ::sqrtf(u * u + v * v);

	if (izanagi::CMath::IsNearyEqualZero(r)) {
		// 中心位置の場合は正面を向く
		ref.x = 0.0f;
		ref.y = 0.0f;
		ref.z = 1.0f;
	}
	else {
		// α = r * π
		IZ_FLOAT alpha = r * IZ_MATH_PI;

		// そうは言っても正負の判定はしないといけない
		alpha = (u < 0.0f ? -alpha : alpha);

		ref.z = ::cosf(alpha);

		// sin(alpha)を計算
		IZ_FLOAT sin = ::sqrtf(1.0f - ref.z * ref.z);

		ref.x = sin * u / r;
		ref.y = sin * v / r;
	}
}
