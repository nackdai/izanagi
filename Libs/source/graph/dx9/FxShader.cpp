#include "graph/dx9/FxShader.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace uranus;

namespace {
	struct _SFuncCreateEffect {
		UN_BOOL operator()(
			CGraphicsDevice* pDevice,
			const void* pProgram,
			UN_UINT nProgramLen,
			D3D_EFFECT** pEffect)
		{
			HRESULT hr = D3DXCreateEffect(
							pDevice->GetRawInterface(),
							pProgram,
							nProgramLen,
							UN_NULL,
							UN_NULL,
							0,
							UN_NULL,
							pEffect,
							UN_NULL);
			UN_BOOL ret = SUCCEEDED(hr);
			UN_ASSERT(ret);
			return ret;
		}
	};

	struct _SFuncCreateEffectFromFile {
		UN_BOOL operator()(
			CGraphicsDevice* pDevice,
			const char* pszFile,
			UN_UINT nDummy,
			D3D_EFFECT** pEffect)
		{
			UNUSED_ALWAYS(pEffect);

			HRESULT hr = D3DXCreateEffectFromFile(
							pDevice->GetRawInterface(),
							pszFile,
							UN_NULL,
							UN_NULL,
							0,
							UN_NULL,
							pEffect,
							UN_NULL);
			UN_BOOL ret = SUCCEEDED(hr);
			UN_ASSERT(ret);
			return ret;
		}
	};

	struct _SFuncCreateEffectDummy {
		UN_BOOL operator()(
			CGraphicsDevice* pDevice,
			const void* p,
			UN_UINT n,
			D3D_EFFECT** pEffect)
		{
			return UN_FALSE;
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
	UN_UINT nProgramLen)
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
	UN_PCSTR pszFile)
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
	UN_UINT nInputLen,
	_Func& func)
{
	UN_ASSERT(pAllocator != UN_NULL);
	UN_ASSERT(pDevice != UN_NULL);

	CFxShader* pInstance = UN_NULL;

	// メモリ確保
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CFxShader));
	UN_BOOL result = (pBuf != UN_NULL);
	VGOTO(result, __EXIT__);

	// インスタンス作成
	pInstance = new(pBuf) CFxShader;
	{
		pInstance->m_pAllocator = pAllocator;
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
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
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
	if (m_pAllocator != UN_NULL) {
		FREE(m_pAllocator, this);
	}
}

UN_BOOL CFxShader::SetProgram(
	const void* pProgram,
	UN_UINT nProgramLen)
{
	UN_ASSERT(pProgram != UN_NULL);
	UN_ASSERT(nProgramLen > 0);
	UN_ASSERT(m_pDevice != UN_NULL);

	if (m_pBody != UN_NULL) {
		SAFE_RELEASE(m_pBody);
	}
	
	HRESULT hr = D3DXCreateEffect(
					m_pDevice->GetRawInterface(),
					pProgram,
					nProgramLen,
					UN_NULL,
					UN_NULL,
					0,
					UN_NULL,
					&m_pBody,
					UN_NULL);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);
	return ret;
}

/**
* Retruns num of techniques.
*/
UN_UINT CFxShader::GetTechniqueNum()
{
	return m_sDesc.Techniques;
}

UN_BOOL CFxShader::SetTechnique(UN_UINT nTechIdx)
{
	UN_ASSERT(m_pBody != UN_NULL);
	UN_ASSERT(nTechIdx < GetTechniqueNum());

	SHADER_PARAM_HANDLE hTech = m_pBody->GetTechnique(nTechIdx);
	UN_ASSERT(hTech != UN_NULL);

	HRESULT hr = m_pBody->SetTechnique(hTech);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

UN_BOOL CFxShader::Begin(UN_UINT* pPassNum)
{
	UN_ASSERT(m_pBody != UN_NULL);

	HRESULT hr = m_pBody->Begin(
					&m_nCurTechPassNum,
					D3DXFX_DONOTSAVESAMPLERSTATE);

	if (pPassNum != UN_NULL) {
		*pPassNum = m_nCurTechPassNum;
	}

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

UN_BOOL CFxShader::End()
{
	UN_ASSERT(m_pBody != UN_NULL);

	m_nCurTechPassNum = 0;
	HRESULT hr = m_pBody->End();

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

UN_BOOL CFxShader::BeingPass(UN_UINT nPassIdx)
{
	UN_ASSERT(m_pBody != UN_NULL);
	UN_ASSERT(nPassIdx < m_nCurTechPassNum);

	HRESULT hr = m_pBody->BeginPass(nPassIdx);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

UN_BOOL CFxShader::EndPass()
{
	UN_ASSERT(m_pBody != UN_NULL);

	HRESULT hr = m_pBody->EndPass();

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

/**
* Returns handle of parameter by name.
*/
SHADER_PARAM_HANDLE CFxShader::GetParameterByName(UN_PCSTR pszName)
{
	UN_ASSERT(m_pBody != UN_NULL);

	SHADER_PARAM_HANDLE ret = m_pBody->GetParameterByName(
								UN_NULL,
								pszName);
	UN_ASSERT(ret != UN_NULL);

	return ret;
}

/**
* Returns handle of parameterby semantic.
*/
SHADER_PARAM_HANDLE CFxShader::GetParameterBySemantic(UN_PCSTR pszSemantic)
{
	UN_ASSERT(m_pBody != UN_NULL);

	SHADER_PARAM_HANDLE ret = m_pBody->GetParameterBySemantic(
								UN_NULL,
								pszSemantic);
	UN_ASSERT(ret != UN_NULL);

	return ret;
}

/**
* Gets description of parameter.
*/
UN_BOOL CFxShader::GetParameterDesc(
	SHADER_PARAM_HANDLE hParam,
	SParamDesc* pDesc)
{
	UN_ASSERT(m_pBody != UN_NULL);
	UN_ASSERT(hParam != UN_NULL);
	UN_ASSERT(pDesc != UN_NULL);

	D3DXPARAMETER_DESC sDesc;
	HRESULT hr = m_pBody->GetParameterDesc(
					hParam,
					&sDesc);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

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
UN_BOOL CFxShader::SetValue(
	SHADER_PARAM_HANDLE hParam,
	void* pValue,
	UN_UINT nSize)
{
	UN_ASSERT(m_pBody != UN_NULL);
	UN_ASSERT(hParam != UN_NULL);
	UN_ASSERT(pValue != UN_NULL);
	UN_ASSERT(nSize > 0);

	HRESULT hr = m_pBody->SetValue(
					hParam,
					pValue,
					nSize);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}
