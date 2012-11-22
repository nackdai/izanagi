#if !defined(__SHADER_BASE_H__)
#define __SHADER_BASE_H__

#include "izStd.h"
#include "izGraph.h"
#include "izMath.h"

// シェーダベース
class CShaderBase : public izanagi::CObject {
public:
	template <class _T>
	static _T* CreateShader(
		izanagi::IMemoryAllocator* pAllocator,
		izanagi::graph::CGraphicsDevice* pDevice,
		LPCSTR lpszPathName);

protected:
	CShaderBase();
	virtual ~CShaderBase();

	CShaderBase(const CShaderBase& rhs);
	const CShaderBase& operator =(const CShaderBase& rhs);

protected:
	// シェーダ読み込み
	IZ_BOOL ReadShader(
		izanagi::graph::CGraphicsDevice* pDevice,
		LPCSTR lpszPathName);

	// シェーダごとの初期化
	virtual IZ_BOOL InitInternal(izanagi::graph::CGraphicsDevice* pDevice);

	// 共通パラメータのハンドルを取得
	BOOL GetCommonParameterHandles();

protected:
	// シェーダ定数ハンドルをまとめて取得

	// 名前で取得
	IZ_BOOL GetParameterByName(
		IZ_INT num,
		D3DXHANDLE* pHandles,
		LPCSTR* pNames);

	// セマンティックで取得
	IZ_BOOL GetParameterBySemantic(
		IZ_INT num,
		D3DXHANDLE* pHandles,
		LPCSTR* pSemantics);

public:
	inline LPD3DXEFFECT GetEffect();
	inline IZ_UINT GetPassCount() const;

	inline IZ_BOOL Begin(IZ_DWORD flags);
	inline IZ_BOOL End();

	inline IZ_BOOL BeginPass(IZ_UINT nPass);
	inline IZ_BOOL EndPass();

public:
	inline void SetL2W(const izanagi::SMatrix& mat);
	inline void SetW2C(const izanagi::SMatrix& mat);

public:
	inline void OnLostDevice();
	inline IZ_BOOL Reset(
		izanagi::graph::CGraphicsDevice* pDevice,
		LPCSTR lpszPathName);

protected:
	enum {
		HANDLE_COMMON_MTX_L2W = 0,
		HANDLE_COMMON_MTX_W2C,

		HANDLE_COMMON_NUM,
	};

protected:
	izanagi::IMemoryAllocator* m_pAllocator;

	// 共通ハンドル
	D3DXHANDLE m_CommonHandles[HANDLE_COMMON_NUM];

	LPD3DXEFFECT m_pEffect;
	IZ_UINT m_nPassCount;
};

// **********************************

template <class _T>
_T* CShaderBase::CreateShader(
	izanagi::IMemoryAllocator* pAllocator,
	izanagi::graph::CGraphicsDevice* pDevice,
	LPCSTR lpszPathName)
{
	IZ_UINT8* pBuf = IZ_NULL;
	_T* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(_T));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	pInstance = new(pBuf) _T;
	{
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();
	}

	// シェーダ読み込み
	result = pInstance->ReadShader(pDevice, lpszPathName);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// シェーダごとの初期化
	result = pInstance->InitInternal(pDevice);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

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

// inline ***************************

// エフェクト本体取得
LPD3DXEFFECT CShaderBase::GetEffect()
{
	return m_pEffect;
}

// パス数取得
IZ_UINT CShaderBase::GetPassCount() const
{
	return m_nPassCount;
}

// シェーダ開始
IZ_BOOL CShaderBase::Begin(IZ_DWORD flags)
{
	IZ_ASSERT(m_pEffect != NULL);
	HRESULT hr = m_pEffect->Begin(&m_nPassCount, flags);
	return SUCCEEDED(hr);
}

// シェーダ終了
IZ_BOOL CShaderBase::End()
{
	IZ_ASSERT(m_pEffect != NULL);
	HRESULT hr = m_pEffect->End();
	return SUCCEEDED(hr);
}

// パス開始
IZ_BOOL CShaderBase::BeginPass(UINT nPass)
{
	IZ_ASSERT(m_pEffect != NULL);
	IZ_ASSERT(nPass < m_nPassCount);
	HRESULT hr = m_pEffect->BeginPass(nPass);
	return SUCCEEDED(hr);
}

// パス終了
IZ_BOOL CShaderBase::EndPass()
{
	IZ_ASSERT(m_pEffect != NULL);
	HRESULT hr = m_pEffect->EndPass();
	return SUCCEEDED(hr);
}

// L2Wマトリクスセット
void CShaderBase::SetL2W(const izanagi::SMatrix& mat)
{
	if (m_CommonHandles[HANDLE_COMMON_MTX_L2W] != NULL) {
		// ちとマズイがメンドイので・・・
		m_pEffect->SetMatrix(
			m_CommonHandles[HANDLE_COMMON_MTX_L2W],
			(D3DXMATRIX*)&mat);
	}
}

// W2Cマトリクスセット
void CShaderBase::SetW2C(const izanagi::SMatrix& mat)
{
	if (m_CommonHandles[HANDLE_COMMON_MTX_W2C] != NULL) {
		// ちとマズイがメンドイので・・・
		m_pEffect->SetMatrix(
			m_CommonHandles[HANDLE_COMMON_MTX_W2C],
			(D3DXMATRIX*)&mat);
	}
}

void CShaderBase::OnLostDevice()
{
	SAFE_RELEASE(m_pEffect);
}

IZ_BOOL CShaderBase::Reset(
	izanagi::graph::CGraphicsDevice* pDevice,
	LPCSTR lpszPathName)
{
	IZ_BOOL ret = ReadShader(pDevice, lpszPathName);
	if (ret) {
		ret = InitInternal(pDevice);
	}
	return ret;
}

#endif	// #if !defined(__SHADER_BASE_H__)
