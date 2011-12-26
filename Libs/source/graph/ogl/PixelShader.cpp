#include "graph/dx9/PixelShader.h"

using namespace izanagi;

// インスタンス作成
CPixelShader* CPixelShader::CreatePixelShader(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pProgram)
{
	CPixelShader* pInstance = IZ_NULL;
	IZ_UINT8* pBuf = IZ_NULL;
	IZ_BOOL result = IZ_TRUE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPixelShader));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CPixelShader;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	//IZ_C_ASSERT(sizeof(DWORD) == sizeof(IZ_DWORD));

	// シェーダ作成
	D3D_DEVICE* pRawDevice = pDevice->GetRawInterface();
	HRESULT hr = pRawDevice->CreatePixelShader(
					(const DWORD*)pProgram,
					&pInstance->m_pPS);
	result = SUCCEEDED(hr);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// シェーダ定数テーブル取得
	hr = D3DXGetShaderConstantTable(
			(const DWORD*)pProgram,
			&pInstance->m_pConstTable);
	result = SUCCEEDED(hr);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	SAFE_REPLACE(pInstance->m_pDevice, pDevice);

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}
