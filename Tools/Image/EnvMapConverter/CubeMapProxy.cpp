#include "CubeMapProxy.h"

CCubeMapProxy::CCubeMapProxy(
	std::vector<izanagi::izanagi_tk::CTextureLite*>& tex,
	EnvMapType type)
: CTexProxy(type)
{
	IZ_ASSERT(tex.size() == izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);

	for (size_t i = 0; i < tex.size(); i++) {
		m_Tex.push_back(tex[i]);
	}

	m_Pitch = 0;
	m_Data = IZ_NULL;

	IZ_UINT width = m_Tex[0]->GetWidth();
	IZ_UINT height = m_Tex[0]->GetHeight();

	m_DivW = 1.0f / (width - 1);
	m_DivH = 1.0f / (height - 1);

	m_CurFace = izanagi::E_GRAPH_CUBE_TEX_FACE_NUM;
	m_PrevFace = izanagi::E_GRAPH_CUBE_TEX_FACE_NUM;
	m_IsChangedFace = IZ_FALSE;

	m_IsFloat = izanagi::CGraphUtil::IsFloatPixelFormat(m_Tex[0]->GetPixelFormat());
	m_Bpp = izanagi::CGraphUtil::GetBPP(m_Tex[0]->GetPixelFormat());
}

CCubeMapProxy::~CCubeMapProxy()
{
	if (m_Data) {
		IZ_ASSERT(m_CurFace != izanagi::E_GRAPH_CUBE_TEX_FACE_NUM);

		m_Tex[m_CurFace]->Unlock(0);
		m_Data = IZ_NULL;
	}
}

// 反射ベクトルからUVを取得.
void CCubeMapProxy::getUVFromRef(
	const izanagi::SVector& ref,
	IZ_FLOAT& u, IZ_FLOAT& v)
{
	// 一番大きい値から面を決める
	IZ_FLOAT x = izanagi::CMath::Absf(ref.x);
	IZ_FLOAT y = izanagi::CMath::Absf(ref.y);
	IZ_FLOAT z = izanagi::CMath::Absf(ref.z);

	IZ_FLOAT maxVal = max(x, max(y, z));

	izanagi::E_GRAPH_CUBE_TEX_FACE face = izanagi::E_GRAPH_CUBE_TEX_FACE_X_P;

	if (maxVal == x) {
		// X面
		face = (ref.x > 0.0f
				? izanagi::E_GRAPH_CUBE_TEX_FACE_X_P
				: izanagi::E_GRAPH_CUBE_TEX_FACE_X_N);
	}
	else if (maxVal == y) {
		// Y面
		face = (ref.y > 0.0f
				? izanagi::E_GRAPH_CUBE_TEX_FACE_Y_N
				: izanagi::E_GRAPH_CUBE_TEX_FACE_Y_P);
	}
	else {
		// Z面
		face = (ref.z > 0.0f
				? izanagi::E_GRAPH_CUBE_TEX_FACE_Z_P
				: izanagi::E_GRAPH_CUBE_TEX_FACE_Z_N);
	}

	// [-1:1]
	switch (face) {
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_P:	// +X
		{
			u = -ref.z / ref.x;
			v = ref.y / ref.x;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_N:	// -X
		{
			u = -ref.z / ref.x;
			v = -ref.y / ref.x;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_P:	// +Y
		{
			u = -ref.x / ref.y;
			v = -ref.z / ref.y;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_N:	// -Y
		{
			u = ref.x / ref.y;
			v = ref.z / ref.y;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_P:	// +Z
		{
			u = ref.x / ref.z;
			v = ref.y / ref.z;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_N:	// -Z
		{
			u = ref.x / ref.z;
			v = -ref.y / ref.z;
		}
		break;
	}

	u = izanagi::CMath::Clamp(u, -1.0f, 1.0f);
	v = izanagi::CMath::Clamp(v, -1.0f, 1.0f);

	ChangeFace(face);

	// [0:1]
	u = (u + 1.0f) * 0.5f;
	v = (v + 1.0f) * 0.5f;
}

// XYから反射ベクトルを取得.
void CCubeMapProxy::getRef(
	IZ_UINT x, IZ_UINT y,
	izanagi::SVector& ref,
	izanagi::E_GRAPH_CUBE_TEX_FACE face/*= izanagi::E_GRAPH_CUBE_TEX_FACE_NUM*/)
{
	// [-1:1]に変換
	IZ_FLOAT u = 2.0f * x * m_DivW - 1.0f;
	IZ_FLOAT v = 2.0f * y * m_DivH - 1.0f;

	switch (face) {
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_P:	// +X
		{
			ref.x = 1.0f;
			ref.y = v;
			ref.z = -u;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_N:	// -X
		{
			ref.x = -1.0f;
			ref.y = v;
			ref.z = u;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_P:	// +Y
		{
			ref.x = u;
			ref.y = -1.0f;
			ref.z = v;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_N:	// -Y
		{
			ref.x = u;
			ref.y = 1.0f;
			ref.z = -v;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_P:	// +Z
		{
			ref.x = u;
			ref.y = v;
			ref.z = 1.0f;
		}
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_N:	// -Z
		{
			ref.x = -u;
			ref.y = v;
			ref.z = -1.0f;
		}
		break;
	}

	izanagi::SVector::Normalize(ref, ref);

	ChangeFace(face);
}

// UVから色を取得.
void CCubeMapProxy::getColor(
	IZ_FLOAT u, IZ_FLOAT v,
	SFloatColor& color)
{
	IZ_UINT x = static_cast<IZ_UINT>(u * (m_Tex[0]->GetWidth() - 1));
	IZ_UINT y = static_cast<IZ_UINT>(v * (m_Tex[0]->GetHeight() - 1));

	if (m_Data == IZ_NULL) {
		m_Pitch = m_Tex[m_CurFace]->Lock(0, (void**)&m_Data);
	}
	else if (m_IsChangedFace) {
		m_Tex[m_PrevFace]->Unlock(0);
		m_Pitch = m_Tex[m_CurFace]->Lock(0, (void**)&m_Data);
	}

	IZ_UINT8* data = m_Data + m_Pitch * y + x * m_Bpp;

	if (m_IsFloat) {
		IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

		color.r = d[0];
		color.g = d[1];
		color.b = d[2];
		color.a = d[3];
	}
	else {
		color.r = NormalizeColor(data[0]);
		color.g = NormalizeColor(data[1]);
		color.b = NormalizeColor(data[2]);
		color.a = NormalizeColor(data[3]);
	}
}

// 指定位置に描きこみ.
void CCubeMapProxy::putColor(
	IZ_UINT x, IZ_UINT y,
	const SFloatColor& color)
{
	if (m_Data == IZ_NULL) {
		m_Pitch = m_Tex[m_CurFace]->Lock(0, (void**)&m_Data);
	}
	else if (m_IsChangedFace) {
		m_Tex[m_PrevFace]->Unlock(0);
		m_Pitch = m_Tex[m_CurFace]->Lock(0, (void**)&m_Data);
	}

	IZ_UINT8* data = m_Data + m_Pitch * y + x * m_Bpp;

	if (m_IsFloat) {
		IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

		d[0] = color.r;
		d[1] = color.g;
		d[2] = color.b;
		d[3] = color.a;
	}
	else {
		data[0] = color.getAsUint8_R();
		data[1] = color.getAsUint8_G();
		data[2] = color.getAsUint8_B();
		data[3] = color.getAsUint8_A();
	}
}

// XYが正しい位置かどうか.
IZ_BOOL CCubeMapProxy::isValid(IZ_UINT x, IZ_UINT y) const
{
	return true;
}

// 参照する面の変更
void CCubeMapProxy::ChangeFace(izanagi::E_GRAPH_CUBE_TEX_FACE face)
{
	m_IsChangedFace = (m_CurFace != face);
	m_PrevFace = m_CurFace;
	m_CurFace = face;
}
