#if !defined(__BASIC_SHADER_H__)
#define __BASIC_SHADER_H__

#include "ShaderBase.h"

// 環境マップボックスシェーダ
class CBasicShader : public CShaderBase {
	friend class CShaderBase;

protected:
	CBasicShader() {}
	~CBasicShader() {}

	CBasicShader(const CBasicShader& rhs);
	const CBasicShader& operator =(const CBasicShader& rhs);

protected:
	inline void InternalRelease();

	// シェーダごとの初期化
	inline IZ_BOOL InitInternal(izanagi::CGraphicsDevice* pDevice);
};

// inline ********************************

// 開放
void CBasicShader::InternalRelease()
{
	delete this;
	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// シェーダごとの初期化
IZ_BOOL CBasicShader::InitInternal(izanagi::CGraphicsDevice* pDevice)
{
	// 共通パラメータのハンドルを取得
	GetCommonParameterHandles();
	return IZ_TRUE;
}

#endif	// #if !defined(__BASIC_SHADER_H__)
