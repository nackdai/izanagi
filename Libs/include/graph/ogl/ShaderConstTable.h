#if !defined(__IZANAGI_GRAPH_INTERNAL_SHADER_BASE_H__)
#define __IZANAGI_GRAPH_INTERNAL_SHADER_BASE_H__

#include "izD3DDefs.h"
#include "izDefs.h"
#include "izMath.h"
#include "GraphicsDevice.h"
#include "graph/GraphDefs.h"
#include "std/StdObject.h"

namespace izanagi {
	// シェーダ定数テーブル
	class CShaderConstTable : public CObject {
	protected:
		inline CShaderConstTable();
		virtual ~CShaderConstTable();

		CShaderConstTable(const CShaderConstTable& rhs);
		const CShaderConstTable& operator=(const CShaderConstTable& rhs);

	public:
		inline SHADER_PARAM_HANDLE GetHandleByIdx(IZ_UINT idx);
		inline SHADER_PARAM_HANDLE GetHandleByName(IZ_PCSTR pName);

		inline IZ_UINT GetArrayLengthOf(SHADER_PARAM_HANDLE handle);

	public:
		inline IZ_BOOL SetDefaults();

		inline IZ_BOOL SetBool(SHADER_PARAM_HANDLE handle, IZ_BOOL b);
		inline IZ_BOOL SetBoolArray(SHADER_PARAM_HANDLE handle, const IZ_BOOL* pB, IZ_UINT num);

		inline IZ_BOOL SetFloat(SHADER_PARAM_HANDLE handle, IZ_FLOAT f);
		inline IZ_BOOL SetFloatArray(SHADER_PARAM_HANDLE handle, const IZ_FLOAT* pF, IZ_UINT num);

		inline IZ_BOOL SetInt(SHADER_PARAM_HANDLE handle, IZ_INT n);
		inline IZ_BOOL SetIntArray(SHADER_PARAM_HANDLE handle, const IZ_INT* pN, IZ_UINT num);

		inline IZ_BOOL SetMatrix(SHADER_PARAM_HANDLE handle, const SMatrix& m);
		inline IZ_BOOL SetMatrixArray(SHADER_PARAM_HANDLE handle, const SMatrix* pM, IZ_UINT num);

		inline IZ_BOOL SetVector(SHADER_PARAM_HANDLE handle, const SVector& v);
		inline IZ_BOOL SetVectorArray(SHADER_PARAM_HANDLE handle, const SVector* pV, IZ_UINT num);

		inline IZ_BOOL SetValue(SHADER_PARAM_HANDLE handle, const void* p, IZ_UINT size);

	public:
		inline D3D_CONSTTABLE* GetRawInterface() { return m_pConstTable; }

	protected:
		CGraphicsDevice* m_pDevice;

		// シェーダ定数テーブル
		D3D_CONSTTABLE* m_pConstTable;
	};

	// inlien **************************************

	// コンストラクタ
	CShaderConstTable::CShaderConstTable()
	{
		m_pDevice = IZ_NULL;
		m_pConstTable = IZ_NULL;
	}

	/**
	* ハンドル取得
	*/
	SHADER_PARAM_HANDLE CShaderConstTable::GetHandleByIdx(IZ_UINT idx)
	{
		SHADER_PARAM_HANDLE ret = m_pConstTable->GetConstant(IZ_NULL, idx);
		//IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

	/**
	* ハンドル取得
	*/
	SHADER_PARAM_HANDLE CShaderConstTable::GetHandleByName(IZ_PCSTR pName)
	{
		SHADER_PARAM_HANDLE ret = m_pConstTable->GetConstantByName(IZ_NULL, pName);
		//IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

	/**
	* パラメータの配列数取得
	*/
	IZ_UINT CShaderConstTable::GetArrayLengthOf(SHADER_PARAM_HANDLE handle)
	{
		IZ_UINT ret = 0;

		D3DXCONSTANT_DESC desc;
		IZ_UINT count = 1;

		HRESULT hr = m_pConstTable->GetConstantDesc(handle, &desc, &count);
		if (SUCCEEDED(hr)) {
			ret = desc.Elements;
		}

		return ret;
	}

	/**
	* 定数をデフォルト値に設定
	*/
	IZ_BOOL CShaderConstTable::SetDefaults()
	{
		HRESULT hr = m_pConstTable->SetDefaults(m_pDevice->GetRawInterface());
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* ブール値を設定
	*/
	IZ_BOOL CShaderConstTable::SetBool(SHADER_PARAM_HANDLE handle, IZ_BOOL b)
	{
		HRESULT hr = m_pConstTable->SetBool(
						m_pDevice->GetRawInterface(),
						handle,
						b);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* ブール値を設定
	*/
	IZ_BOOL CShaderConstTable::SetBoolArray(SHADER_PARAM_HANDLE handle, const IZ_BOOL* pB, IZ_UINT num)
	{
		IZ_C_ASSERT(sizeof(BOOL) == sizeof(IZ_BOOL));

		HRESULT hr = m_pConstTable->SetBoolArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const BOOL*)pB, num);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 浮動小数点数を設定
	*/
	IZ_BOOL CShaderConstTable::SetFloat(SHADER_PARAM_HANDLE handle, IZ_FLOAT f)
	{
		HRESULT hr = m_pConstTable->SetFloat(
						m_pDevice->GetRawInterface(),
						handle,
						f);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 浮動小数点数を設定
	*/
	IZ_BOOL CShaderConstTable::SetFloatArray(SHADER_PARAM_HANDLE handle, const IZ_FLOAT* pF, IZ_UINT num)
	{
		HRESULT hr = m_pConstTable->SetFloatArray(
						m_pDevice->GetRawInterface(),
						handle,
						pF, num);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 整数値を設定
	*/
	IZ_BOOL CShaderConstTable::SetInt(SHADER_PARAM_HANDLE handle, IZ_INT n)
	{
		HRESULT hr = m_pConstTable->SetInt(
						m_pDevice->GetRawInterface(),
						handle,
						n);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 整数値を設定
	*/
	IZ_BOOL CShaderConstTable::SetIntArray(SHADER_PARAM_HANDLE handle, const IZ_INT* pN, IZ_UINT num)
	{
		HRESULT hr = m_pConstTable->SetIntArray(
						m_pDevice->GetRawInterface(),
						handle,
						pN, num);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 行列を設定
	*/
	IZ_BOOL CShaderConstTable::SetMatrix(SHADER_PARAM_HANDLE handle, const SMatrix& m)
	{
		IZ_C_ASSERT(sizeof(D3DXMATRIX) == sizeof(SMatrix));

		HRESULT hr = m_pConstTable->SetMatrix(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXMATRIX*)&m);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 行列を設定
	*/
	IZ_BOOL CShaderConstTable::SetMatrixArray(SHADER_PARAM_HANDLE handle, const SMatrix* pM, IZ_UINT num)
	{
		IZ_C_ASSERT(sizeof(D3DXMATRIX) == sizeof(SMatrix));

		HRESULT hr = m_pConstTable->SetMatrixArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXMATRIX*)pM, num);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 4D ベクトルを設定
	*/
	IZ_BOOL CShaderConstTable::SetVector(SHADER_PARAM_HANDLE handle, const SVector& v)
	{
		IZ_C_ASSERT(sizeof(D3DXVECTOR4) == sizeof(SVector));

		HRESULT hr = m_pConstTable->SetVector(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXVECTOR4*)&v);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 4D ベクトルを設定
	*/
	IZ_BOOL CShaderConstTable::SetVectorArray(SHADER_PARAM_HANDLE handle, const SVector* pV, IZ_UINT num)
	{
		IZ_C_ASSERT(sizeof(D3DXVECTOR4) == sizeof(SVector));

		HRESULT hr = m_pConstTable->SetVectorArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXVECTOR4*)pV, num);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* バッファの内容を定数テーブルに設定
	*/
	IZ_BOOL CShaderConstTable::SetValue(SHADER_PARAM_HANDLE handle, const void* p, IZ_UINT size)
	{
		HRESULT hr = m_pConstTable->SetValue(
						m_pDevice->GetRawInterface(),
						handle,
						p, size);
		IZ_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_INTERNAL_SHADER_BASE_H__)
