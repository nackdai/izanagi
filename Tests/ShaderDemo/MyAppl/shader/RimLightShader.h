#if !defined(__RIM_LIGHT_SHADER_H__)
#define __RIM_LIGHT_SHADER_H__

#include "izSceneGraph.h"
#include "PhongShader.h"

// RimLightシェーダ
class CRimLightShader : public CPhongShader {
	friend class CShaderBase;

protected:
	inline CRimLightShader();
	~CRimLightShader() {}

	CRimLightShader(const CRimLightShader& rhs);
	const CRimLightShader& operator=(const CRimLightShader& rhs);

protected:
	// シェーダごとの初期化
	IZ_BOOL InitInternal(izanagi::graph::CGraphicsDevice* pDevice);

public:
	inline void SetMaterial(const izanagi::SMaterialParam& sMaterial);

protected:
	D3DXHANDLE m_hRimLightParamHandle;
};

// inline ********************************

CRimLightShader::CRimLightShader()
{
	m_hRimLightParamHandle = IZ_NULL;
}

// マテリアルパラメータセット
void CRimLightShader::SetMaterial(const izanagi::SMaterialParam& sMaterial)
{
	CPhongShader::SetMaterial(sMaterial);

	m_pEffect->SetVector(
		m_hRimLightParamHandle,
		(D3DXVECTOR4*)&sMaterial.vRim);
}

#endif	// #if !defined(__RIM_LIGHT_SHADER_H__)
