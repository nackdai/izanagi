#include <stdafx.h>
#include <vector>
#include "ShaderBase.h"

// コンストラクタ
CShaderBase::CShaderBase()
{
	m_pAllocator = IZ_NULL;
	m_pEffect = IZ_NULL;
	m_nPassCount = 0;
	memset(m_CommonHandles, 0, sizeof(m_CommonHandles));
}

// デストラクタ
CShaderBase::~CShaderBase()
{
	SAFE_RELEASE(m_pEffect);
}

// シェーダ読み込み
IZ_BOOL CShaderBase::ReadShader(
	izanagi::CGraphicsDevice* pDevice,
	LPCSTR lpszPathName)
{
	FILE* fp;
	fopen_s(&fp, lpszPathName, "rb");
	if (fp == NULL) {
		IZ_ASSERT(IZ_FALSE);
		return IZ_FALSE;
	}

	// ファイルサイズ取得
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);

	// データ読み込み
	std::vector<BYTE> tvBuf(size);
	fseek(fp, 0, SEEK_SET);
	fread(&tvBuf[0], size, sizeof(BYTE), fp);

	fclose(fp);

	SAFE_RELEASE(m_pEffect);

	LPD3DXBUFFER pErr = IZ_NULL;

	HRESULT hr = D3DXCreateEffect(
					pDevice->GetRawInterface(),
					&tvBuf[0],
					size,
					NULL,
					NULL,
					0,
					NULL,
					&m_pEffect,
					&pErr);
	IZ_BOOL ret = SUCCEEDED(hr);

	if (!ret) {
		IZ_PRINTF("%s\n", (char*)pErr->GetBufferPointer());
		IZ_ASSERT(IZ_FALSE);
	}

	SAFE_RELEASE(pErr);

	return ret;
}

// シェーダごとの初期化
IZ_BOOL CShaderBase::InitInternal(izanagi::CGraphicsDevice* pDevice)
{
	return IZ_TRUE;
}

// 共通パラメータのハンドルを取得
IZ_BOOL CShaderBase::GetCommonParameterHandles()
{
	static const char* PARAM_NAME[] = {
		"g_mL2W",
		"g_mW2C",
	};
	C_ASSERT(COUNTOF(PARAM_NAME) == HANDLE_COMMON_NUM);

	IZ_BOOL ret = GetParameterByName(
					HANDLE_COMMON_NUM,
					m_CommonHandles,
					PARAM_NAME);
	IZ_ASSERT(ret);

	return ret;
}

// シェーダ定数ハンドルをまとめて取得
// 名前で取得
IZ_BOOL CShaderBase::GetParameterByName(
	IZ_INT num,
	D3DXHANDLE* pHandles,
	LPCSTR* pNames)
{
	IZ_ASSERT(m_pEffect != IZ_NULL);

	IZ_BOOL ret = IZ_TRUE;

	for (IZ_INT i = 0; i < num; i++) {
		pHandles[i] = m_pEffect->GetParameterByName(NULL, pNames[i]);
		if (pHandles[i] == NULL) {
			ret = IZ_FALSE;
			IZ_ASSERT(IZ_FALSE);
			break;
		}
	}

	return ret;
}

// シェーダ定数ハンドルをまとめて取得
// セマンティックで取得
IZ_BOOL CShaderBase::GetParameterBySemantic(
	IZ_INT num,
	D3DXHANDLE* pHandles,
	LPCSTR* pSemantics)
{
	ASSERT(m_pEffect != NULL);

	IZ_BOOL ret = IZ_TRUE;

	for (IZ_INT i = 0; i < num; i++) {
		pHandles[i] = m_pEffect->GetParameterBySemantic(NULL, pSemantics[i]);
		if (pHandles[i] == NULL) {
			ret = IZ_FALSE;
			IZ_ASSERT(IZ_FALSE);
			break;
		}
	}

	return ret;
}
