#if !defined(__PHONG_SHADER_H__)
#define __PHONG_SHADER_H__

#include "izSceneGraph.h"
#include "ShaderBase.h"

// フォンシェーダ
class CPhongShader : public CShaderBase {
	friend class CShaderBase;

protected:
	inline CPhongShader();
	~CPhongShader() {}

	CPhongShader(const CPhongShader& rhs);
	const CPhongShader& operator=(const CPhongShader& rhs);

protected:
	// シェーダごとの初期化
	IZ_BOOL InitInternal(izanagi::CGraphicsDevice* pDevice);

	inline void InternalRelease();

public:
	inline void SetCameraPos(const izanagi::SVector& vec);

	inline void SetLight(
		const izanagi::SParallelLightParam& sParallel,
		const izanagi::SAmbientLightParam& sAmbient);

	inline void SetMaterial(const izanagi::SMaterialParam& sMaterial);

protected:
	enum {
		HANDLE_CAMERA_POS = 0,

		HANDLE_MATERIAL_DIFFUSE,
		HANDLE_MATERIAL_AMBIENT,
		HANDLE_MATERIAL_SPECULAR,

		HANDLE_LIGHT_DIR,
		HANDLE_LIGHT_COLOR,
		HANDLE_LIGHT_AMBIENT,

		HANDLE_NUM,
	};

	D3DXHANDLE m_hHandles[HANDLE_NUM];
};

// inline ********************************

// コンストラクタ
CPhongShader::CPhongShader()
{
	memset(m_hHandles, 0, sizeof(m_hHandles));
}

// 開放
void CPhongShader::InternalRelease()
{
	delete this;
	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// カメラ位置セット
void CPhongShader::SetCameraPos(const izanagi::SVector& vec)
{
	if (m_hHandles[HANDLE_CAMERA_POS] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_CAMERA_POS],
			(D3DXVECTOR4*)&vec);
	}
}

// ライトパラメータセット
void CPhongShader::SetLight(
	const izanagi::SParallelLightParam& sParallel,
	const izanagi::SAmbientLightParam& sAmbient)
{
	// ベタだけど・・・
	if (m_hHandles[HANDLE_LIGHT_DIR] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_DIR],
			(D3DXVECTOR4*)&sParallel.vDir);
	}
	if (m_hHandles[HANDLE_LIGHT_COLOR] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_COLOR],
			(D3DXVECTOR4*)&sParallel.color);
	}
	if (m_hHandles[HANDLE_LIGHT_AMBIENT] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_LIGHT_AMBIENT],
			(D3DXVECTOR4*)&sAmbient.color);
	}
}

// マテリアルパラメータセット
void CPhongShader::SetMaterial(const izanagi::SMaterialParam& sMaterial)
{
	// ベタだけど・・・
	if (m_hHandles[HANDLE_MATERIAL_DIFFUSE] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_MATERIAL_DIFFUSE],
			(D3DXVECTOR4*)&sMaterial.vDiffuse);
	}
	if (m_hHandles[HANDLE_MATERIAL_AMBIENT] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_MATERIAL_AMBIENT],
			(D3DXVECTOR4*)&sMaterial.vAmbient);
	}
	if (m_hHandles[HANDLE_MATERIAL_SPECULAR] != IZ_NULL) {
		m_pEffect->SetVector(
			m_hHandles[HANDLE_MATERIAL_SPECULAR],
			(D3DXVECTOR4*)&sMaterial.vSpecular);
	}
}

#endif	// #if !defined(__PHONG_SHADER_H__)
