#if !defined(__HEMI_LIGHT_SHADER_H__)
#define __HEMI_LIGHT_SHADER_H__

#include "izSceneGraph.h"
#include "ShaderBase.h"

// 半球ライトシェーダー
class CHemiLightShader : public CShaderBase {
	friend class CShaderBase;

protected:
	inline CHemiLightShader();
	~CHemiLightShader() {}

	CHemiLightShader(const CHemiLightShader& rhs);
	const CHemiLightShader& operator=(const CHemiLightShader& rhs);

protected:
	// シェーダごとの初期化
	IZ_BOOL InitInternal(izanagi::CGraphicsDevice* pDevice);

	inline void InternalRelease();

public:
	inline void SetCameraPos(const izanagi::SVector& vec);

	inline void SetLight(const izanagi::SHemisphereLightParam& sParam);

	inline void SetMaterial(const izanagi::SMaterialParam& sMaterial);

protected:
	enum {
		HANDLE_CAMERA_POS = 0,

		HANDLE_MATERIAL_DIFFUSE,

		HANDLE_LIGHT_AXIS,
		HANDLE_LIGHT_UP_COLOR,
		HANDLE_LIGHT_DOWN_COLOR,

		HANDLE_NUM,
	};

	D3DXHANDLE m_hHandles[HANDLE_NUM];
};

// inline ********************************

// コンストラクタ
CHemiLightShader::CHemiLightShader()
{
	memset(m_hHandles, 0, sizeof(m_hHandles));
}

// 開放
void CHemiLightShader::InternalRelease()
{
	delete this;
	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// カメラ位置セット
void CHemiLightShader::SetCameraPos(const izanagi::SVector& vec)
{
	if (m_hHandles[HANDLE_CAMERA_POS] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_CAMERA_POS],
			(D3DXVECTOR4*)&vec);
	}
}

// ライトパラメータセット
void CHemiLightShader::SetLight(const izanagi::SHemisphereLightParam& sParam)
{
	// ベタだけど・・・
	if (m_hHandles[HANDLE_LIGHT_AXIS] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_AXIS],
			(D3DXVECTOR4*)&sParam.vAxis);
	}
	if (m_hHandles[HANDLE_LIGHT_UP_COLOR] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_UP_COLOR],
			(D3DXVECTOR4*)&sParam.upColor);
	}
	if (m_hHandles[HANDLE_LIGHT_DOWN_COLOR] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_DOWN_COLOR],
			(D3DXVECTOR4*)&sParam.downColor);
	}
}

// マテリアルパラメータセット
void CHemiLightShader::SetMaterial(const izanagi::SMaterialParam& sMaterial)
{
	// ベタだけど・・・
	if (m_hHandles[HANDLE_MATERIAL_DIFFUSE] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_MATERIAL_DIFFUSE],
			(D3DXVECTOR4*)&sMaterial.vDiffuse);
	}
}

#endif	// #if !defined(__HEMI_LIGHT_SHADER_H__)
