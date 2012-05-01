#include "graph/dx9/FxShader.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace izanagi;

namespace {
	struct _SFuncCreateEffect {
		IZ_BOOL operator()(
			CGraphicsDevice* pDevice,
			const void* pProgram,
			IZ_UINT nProgramLen,
			D3D_EFFECT** pEffect)
		{
			HRESULT hr = D3DXCreateEffect(
							pDevice->GetRawInterface(),
							pProgram,
							nProgramLen,
							IZ_NULL,
							IZ_NULL,
							0,
							IZ_NULL,
							pEffect,
							IZ_NULL);
			IZ_BOOL ret = SUCCEEDED(hr);
			IZ_ASSERT(ret);
			return ret;
		}
	};

	struct _SFuncCreateEffectFromFile {
		IZ_BOOL operator()(
			CGraphicsDevice* pDevice,
			const char* pszFile,
			IZ_UINT nDummy,
			D3D_EFFECT** pEffect)
		{
			UNUSED_ALWAYS(pEffect);

			HRESULT hr = D3DXCreateEffectFromFile(
							pDevice->GetRawInterface(),
							pszFile,
							IZ_NULL,
							IZ_NULL,
							0,
							IZ_NULL,
							pEffect,
							IZ_NULL);
			IZ_BOOL ret = SUCCEEDED(hr);
			IZ_ASSERT(ret);
			return ret;
		}
	};

	struct _SFuncCreateEffectDummy {
		IZ_BOOL operator()(
			CGraphicsDevice* pDevice,
			const void* p,
			IZ_UINT n,
			D3D_EFFECT** pEffect)
		{
			return IZ_FALSE;
		}
	};
}	// namespace

/**
* Creates instance of CFxShader from binary data.
*/
CFxShader* CFxShader::CreateFxShader(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	const void* pProgram,
	IZ_UINT nProgramLen)
{
	CFxShader* ret = CreateFxShader(
						pAllocator,
						pDevice,
						pProgram,
						nProgramLen,
						_SFuncCreateEffect());
	return ret;
}

/**
* Creates instance of CFxShader from effect file.
*/
CFxShader* CFxShader::CreateFxShaderFromFile(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IZ_PCSTR pszFile)
{
	CFxShader* ret = CreateFxShader(
						pAllocator,
						pDevice,
						pszFile,
						0,
						_SFuncCreateEffectFromFile());
	return ret;
}

CFxShader* CFxShader::CreateFxShader(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	CFxShader* ret = CreateFxShader(
						pAllocator,
						pDevice,
						(void*)0,
						0,
						_SFuncCreateEffectDummy());
	return ret;
}

template <typename _T, typename _Func>
CFxShader* CFxShader::CreateFxShader(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	_T tInput,
	IZ_UINT nInputLen,
	_Func& func)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pDevice != IZ_NULL);

	CFxShader* pInstance = IZ_NULL;

	// メモリ確保
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CFxShader));
	IZ_BOOL result = (pBuf != IZ_NULL);
	VGOTO(result, __EXIT__);

	// インスタンス作成
	pInstance = new(pBuf) CFxShader;
	{
		pInstance->m_Allocator = pAllocator;
		pInstance->AddRef();

		SAFE_REPLACE(
			pInstance->m_pDevice,
			pDevice);

		// 本体作成
		result = func(
					pDevice,
					tInput,
					nInputLen,
					&pInstance->m_pBody);
		if (result) {
			HRESULT hr = pInstance->m_pBody->GetDesc(
							&pInstance->m_sDesc);
			result = SUCCEEDED(hr);
			VGOTO(result, __EXIT__);
		}
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

CFxShader::CFxShader()
{
	FILL_ZERO(this, sizeof(CFxShader));
}

CFxShader::~CFxShader()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pBody);
}

void CFxShader::InternalRelease()
{
	delete this;
	if (m_Allocator != IZ_NULL) {
		FREE(m_Allocator, this);
	}
}

IZ_BOOL CFxShader::SetProgram(
	const void* pProgram,
	IZ_UINT nProgramLen)
{
	IZ_ASSERT(pProgram != IZ_NULL);
	IZ_ASSERT(nProgramLen > 0);
	IZ_ASSERT(m_pDevice != IZ_NULL);

	if (m_pBody != IZ_NULL) {
		SAFE_RELEASE(m_pBody);
	}
	
	HRESULT hr = D3DXCreateEffect(
					m_pDevice->GetRawInterface(),
					pProgram,
					nProgramLen,
					IZ_NULL,
					IZ_NULL,
					0,
					IZ_NULL,
					&m_pBody,
					IZ_NULL);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);
	return ret;
}

/**
* Retruns num of techniques.
*/
IZ_UINT CFxShader::GetTechniqueNum()
{
	return m_sDesc.Techniques;
}

IZ_BOOL CFxShader::SetTechnique(IZ_UINT nTechIdx)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(nTechIdx < GetTechniqueNum());

	SHADER_PARAM_HANDLE hTech = m_pBody->GetTechnique(nTechIdx);
	IZ_ASSERT(hTech != IZ_NULL);

	HRESULT hr = m_pBody->SetTechnique(hTech);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

IZ_BOOL CFxShader::Begin(IZ_UINT* pPassNum)
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	HRESULT hr = m_pBody->Begin(
					&m_nCurTechPassNum,
					D3DXFX_DONOTSAVESAMPLERSTATE);

	if (pPassNum != IZ_NULL) {
		*pPassNum = m_nCurTechPassNum;
	}

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

IZ_BOOL CFxShader::End()
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	m_nCurTechPassNum = 0;
	HRESULT hr = m_pBody->End();

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

IZ_BOOL CFxShader::BeingPass(IZ_UINT nPassIdx)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(nPassIdx < m_nCurTechPassNum);

	HRESULT hr = m_pBody->BeginPass(nPassIdx);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

IZ_BOOL CFxShader::EndPass()
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	HRESULT hr = m_pBody->EndPass();

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

/**
* Returns handle of parameter by name.
*/
SHADER_PARAM_HANDLE CFxShader::GetParameterByName(IZ_PCSTR pszName)
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	SHADER_PARAM_HANDLE ret = m_pBody->GetParameterByName(
								IZ_NULL,
								pszName);
	IZ_ASSERT(ret != IZ_NULL);

	return ret;
}

/**
* Returns handle of parameterby semantic.
*/
SHADER_PARAM_HANDLE CFxShader::GetParameterBySemantic(IZ_PCSTR pszSemantic)
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	SHADER_PARAM_HANDLE ret = m_pBody->GetParameterBySemantic(
								IZ_NULL,
								pszSemantic);
	IZ_ASSERT(ret != IZ_NULL);

	return ret;
}

/**
* Gets description of parameter.
*/
IZ_BOOL CFxShader::GetParameterDesc(
	SHADER_PARAM_HANDLE hParam,
	SParamDesc* pDesc)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(hParam != IZ_NULL);
	IZ_ASSERT(pDesc != IZ_NULL);

	D3DXPARAMETER_DESC sDesc;
	HRESULT hr = m_pBody->GetParameterDesc(
					hParam,
					&sDesc);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	if (ret) {
		pDesc->Name = sDesc.Name;
		pDesc->Semantic = sDesc.Semantic;
		pDesc->Rows = sDesc.Rows;
		pDesc->Columns = sDesc.Columns;
		pDesc->Elements = sDesc.Elements;
		pDesc->Bytes = sDesc.Bytes;
	}

	return ret;
}

/**
* Sets value of parameter.
*/
IZ_BOOL CFxShader::SetValue(
	SHADER_PARAM_HANDLE hParam,
	void* pValue,
	IZ_UINT nSize)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(hParam != IZ_NULL);
	IZ_ASSERT(pValue != IZ_NULL);
	IZ_ASSERT(nSize > 0);

	HRESULT hr = m_pBody->SetValue(
					hParam,
					pValue,
					nSize);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}
