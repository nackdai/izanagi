#if !defined(__URANUS_GRAPH_INTERNAL_SHADER_BASE_H__)
#define __URANUS_GRAPH_INTERNAL_SHADER_BASE_H__

#include "unD3DDefs.h"
#include "unDefs.h"
#include "unMath.h"
#include "GraphicsDevice.h"
#include "graph/GraphDefs.h"
#include "std/StdObject.h"

namespace uranus {
	// �V�F�[�_�萔�e�[�u��
	class CShaderConstTable : public CObject {
	protected:
		inline CShaderConstTable();
		virtual ~CShaderConstTable();

		CShaderConstTable(const CShaderConstTable& rhs);
		const CShaderConstTable& operator=(const CShaderConstTable& rhs);

	public:
		inline SHADER_PARAM_HANDLE GetHandleByIdx(UN_UINT idx);
		inline SHADER_PARAM_HANDLE GetHandleByName(UN_PCSTR pName);

		inline UN_UINT GetArrayLengthOf(SHADER_PARAM_HANDLE handle);

	public:
		inline UN_BOOL SetDefaults();

		inline UN_BOOL SetBool(SHADER_PARAM_HANDLE handle, UN_BOOL b);
		inline UN_BOOL SetBoolArray(SHADER_PARAM_HANDLE handle, const UN_BOOL* pB, UN_UINT num);

		inline UN_BOOL SetFloat(SHADER_PARAM_HANDLE handle, UN_FLOAT f);
		inline UN_BOOL SetFloatArray(SHADER_PARAM_HANDLE handle, const UN_FLOAT* pF, UN_UINT num);

		inline UN_BOOL SetInt(SHADER_PARAM_HANDLE handle, UN_INT n);
		inline UN_BOOL SetIntArray(SHADER_PARAM_HANDLE handle, const UN_INT* pN, UN_UINT num);

		inline UN_BOOL SetMatrix(SHADER_PARAM_HANDLE handle, const SMatrix& m);
		inline UN_BOOL SetMatrixArray(SHADER_PARAM_HANDLE handle, const SMatrix* pM, UN_UINT num);

		inline UN_BOOL SetVector(SHADER_PARAM_HANDLE handle, const SVector& v);
		inline UN_BOOL SetVectorArray(SHADER_PARAM_HANDLE handle, const SVector* pV, UN_UINT num);

		inline UN_BOOL SetValue(SHADER_PARAM_HANDLE handle, const void* p, UN_UINT size);

	public:
		inline D3D_CONSTTABLE* GetRawInterface() { return m_pConstTable; }

	protected:
		CGraphicsDevice* m_pDevice;

		// �V�F�[�_�萔�e�[�u��
		D3D_CONSTTABLE* m_pConstTable;
	};

	// inlien **************************************

	// �R���X�g���N�^
	CShaderConstTable::CShaderConstTable()
	{
		m_pDevice = UN_NULL;
		m_pConstTable = UN_NULL;
	}

	/**
	* �n���h���擾
	*/
	SHADER_PARAM_HANDLE CShaderConstTable::GetHandleByIdx(UN_UINT idx)
	{
		SHADER_PARAM_HANDLE ret = m_pConstTable->GetConstant(UN_NULL, idx);
		//UN_ASSERT(ret != UN_NULL);
		return ret;
	}

	/**
	* �n���h���擾
	*/
	SHADER_PARAM_HANDLE CShaderConstTable::GetHandleByName(UN_PCSTR pName)
	{
		SHADER_PARAM_HANDLE ret = m_pConstTable->GetConstantByName(UN_NULL, pName);
		//UN_ASSERT(ret != UN_NULL);
		return ret;
	}

	/**
	* �p�����[�^�̔z�񐔎擾
	*/
	UN_UINT CShaderConstTable::GetArrayLengthOf(SHADER_PARAM_HANDLE handle)
	{
		UN_UINT ret = 0;

		D3DXCONSTANT_DESC desc;
		UN_UINT count = 1;

		HRESULT hr = m_pConstTable->GetConstantDesc(handle, &desc, &count);
		if (SUCCEEDED(hr)) {
			ret = desc.Elements;
		}

		return ret;
	}

	/**
	* �萔���f�t�H���g�l�ɐݒ�
	*/
	UN_BOOL CShaderConstTable::SetDefaults()
	{
		HRESULT hr = m_pConstTable->SetDefaults(m_pDevice->GetRawInterface());
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �u�[���l��ݒ�
	*/
	UN_BOOL CShaderConstTable::SetBool(SHADER_PARAM_HANDLE handle, UN_BOOL b)
	{
		HRESULT hr = m_pConstTable->SetBool(
						m_pDevice->GetRawInterface(),
						handle,
						b);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �u�[���l��ݒ�
	*/
	UN_BOOL CShaderConstTable::SetBoolArray(SHADER_PARAM_HANDLE handle, const UN_BOOL* pB, UN_UINT num)
	{
		UN_C_ASSERT(sizeof(BOOL) == sizeof(UN_BOOL));

		HRESULT hr = m_pConstTable->SetBoolArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const BOOL*)pB, num);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* ���������_����ݒ�
	*/
	UN_BOOL CShaderConstTable::SetFloat(SHADER_PARAM_HANDLE handle, UN_FLOAT f)
	{
		HRESULT hr = m_pConstTable->SetFloat(
						m_pDevice->GetRawInterface(),
						handle,
						f);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* ���������_����ݒ�
	*/
	UN_BOOL CShaderConstTable::SetFloatArray(SHADER_PARAM_HANDLE handle, const UN_FLOAT* pF, UN_UINT num)
	{
		HRESULT hr = m_pConstTable->SetFloatArray(
						m_pDevice->GetRawInterface(),
						handle,
						pF, num);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �����l��ݒ�
	*/
	UN_BOOL CShaderConstTable::SetInt(SHADER_PARAM_HANDLE handle, UN_INT n)
	{
		HRESULT hr = m_pConstTable->SetInt(
						m_pDevice->GetRawInterface(),
						handle,
						n);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �����l��ݒ�
	*/
	UN_BOOL CShaderConstTable::SetIntArray(SHADER_PARAM_HANDLE handle, const UN_INT* pN, UN_UINT num)
	{
		HRESULT hr = m_pConstTable->SetIntArray(
						m_pDevice->GetRawInterface(),
						handle,
						pN, num);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �s���ݒ�
	*/
	UN_BOOL CShaderConstTable::SetMatrix(SHADER_PARAM_HANDLE handle, const SMatrix& m)
	{
		UN_C_ASSERT(sizeof(D3DXMATRIX) == sizeof(SMatrix));

		HRESULT hr = m_pConstTable->SetMatrix(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXMATRIX*)&m);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �s���ݒ�
	*/
	UN_BOOL CShaderConstTable::SetMatrixArray(SHADER_PARAM_HANDLE handle, const SMatrix* pM, UN_UINT num)
	{
		UN_C_ASSERT(sizeof(D3DXMATRIX) == sizeof(SMatrix));

		HRESULT hr = m_pConstTable->SetMatrixArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXMATRIX*)pM, num);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 4D �x�N�g����ݒ�
	*/
	UN_BOOL CShaderConstTable::SetVector(SHADER_PARAM_HANDLE handle, const SVector& v)
	{
		UN_C_ASSERT(sizeof(D3DXVECTOR4) == sizeof(SVector));

		HRESULT hr = m_pConstTable->SetVector(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXVECTOR4*)&v);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* 4D �x�N�g����ݒ�
	*/
	UN_BOOL CShaderConstTable::SetVectorArray(SHADER_PARAM_HANDLE handle, const SVector* pV, UN_UINT num)
	{
		UN_C_ASSERT(sizeof(D3DXVECTOR4) == sizeof(SVector));

		HRESULT hr = m_pConstTable->SetVectorArray(
						m_pDevice->GetRawInterface(),
						handle,
						(const D3DXVECTOR4*)pV, num);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	/**
	* �o�b�t�@�̓��e��萔�e�[�u���ɐݒ�
	*/
	UN_BOOL CShaderConstTable::SetValue(SHADER_PARAM_HANDLE handle, const void* p, UN_UINT size)
	{
		HRESULT hr = m_pConstTable->SetValue(
						m_pDevice->GetRawInterface(),
						handle,
						p, size);
		UN_ASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_INTERNAL_SHADER_BASE_H__)
