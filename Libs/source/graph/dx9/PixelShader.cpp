#include "graph/dx9/PixelShader.h"

using namespace uranus;

// �C���X�^���X�쐬
CPixelShader* CPixelShader::CreatePixelShader(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pProgram)
{
	CPixelShader* pInstance = UN_NULL;
	UN_UINT8* pBuf = UN_NULL;
	UN_BOOL result = UN_TRUE;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPixelShader));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new(pBuf) CPixelShader;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	//UN_C_ASSERT(sizeof(DWORD) == sizeof(UN_DWORD));

	// �V�F�[�_�쐬
	D3D_DEVICE* pRawDevice = pDevice->GetRawInterface();
	HRESULT hr = pRawDevice->CreatePixelShader(
					(const DWORD*)pProgram,
					&pInstance->m_pPS);
	result = SUCCEEDED(hr);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �V�F�[�_�萔�e�[�u���擾
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
