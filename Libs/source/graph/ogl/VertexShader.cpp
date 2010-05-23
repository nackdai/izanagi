#include "graph/dx9/VertexShader.h"

using namespace uranus;

// インスタンス作成
CVertexShader* CVertexShader::CreateVertexShader(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pProgram)
{
	CVertexShader* pInstance = UN_NULL;
	UN_UINT8* pBuf = UN_NULL;
	UN_BOOL result = UN_TRUE;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexShader));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CVertexShader;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	//UN_C_ASSERT(sizeof(DWORD) == sizeof(UN_DWORD));

	// シェーダ作成
	D3D_DEVICE* pRawDevice = pDevice->GetRawInterface();
	HRESULT hr = pRawDevice->CreateVertexShader(
					(const DWORD*)pProgram,
					&pInstance->m_pVS);
	result = SUCCEEDED(hr);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// シェーダ定数テーブル取得
	hr = D3DXGetShaderConstantTable(
			(const DWORD*)pProgram,
			&pInstance->m_pConstTable);
	result = SUCCEEDED(hr);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	SAFE_REPLACE(pInstance->m_pDevice, pDevice);

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}
