#if !defined(__ENV_BOX_SHADER_H__)
#define __ENV_BOX_SHADER_H__

#include "ShaderBase.h"

// 環境マップボックスシェーダ
class CEnvBoxShader : public CShaderBase {
	friend class CShaderBase;

protected:
	CEnvBoxShader() {}
	~CEnvBoxShader() {}

	CEnvBoxShader(const CEnvBoxShader& rhs);
	const CEnvBoxShader& operator =(const CEnvBoxShader& rhs);

protected:
	inline void InternalRelease();

	// シェーダごとの初期化
	inline IZ_BOOL InitInternal(izanagi::graph::CGraphicsDevice* pDevice);
};

// inline ********************************

// 開放
void CEnvBoxShader::InternalRelease()
{
	delete this;
	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// シェーダごとの初期化
IZ_BOOL CEnvBoxShader::InitInternal(izanagi::graph::CGraphicsDevice* pDevice)
{
	// 共通パラメータのハンドルを取得
	GetCommonParameterHandles();
	return IZ_TRUE;
}

#endif	// #if !defined(__ENV_BOX_SHADER_H__)
