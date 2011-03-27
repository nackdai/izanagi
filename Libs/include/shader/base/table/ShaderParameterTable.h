#if !defined(__IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__

#include "SHDFormat.h"

namespace izanagi {
	/**
	* シェーダ定数テーブル
	*/
	class CShaderParameterTable {
		friend class CShaderBasic;

	private:
		inline CShaderParameterTable();
		~CShaderParameterTable() {}

		NO_COPIABLE(CShaderParameterTable);

	private:
		// 初期化
		inline IZ_UINT8* Init(IZ_UINT8* p);

		// パラメータセット
		inline IZ_BOOL SetParam(
			IZ_UINT idx,
			const void* pValue,
			IZ_UINT nBytes);

		// パラメータ取得
		inline const void* GetParam(
			IZ_UINT idx,
			IZ_UINT* pBytes) const;

		// パラメータ記述取得
		inline const S_SHD_PARAMETER* GetDesc(IZ_UINT idx) const;

		// パラメータインデックス取得
		inline IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
		inline IZ_INT GetIdxByNameKey(const CKey& cKey) const;

		inline IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;
		inline IZ_INT GetIdxBySemanticKey(const CKey& cKey) const;

		// パラメータアノテーション取得
		inline const S_SHD_PARAM_ANN* GetAnn(IZ_UINT idx) const;

		inline void Reset(IZ_UINT idx);
		inline void ResetAll();

	private:
		S_SHD_PARAM_HEADER m_Header;

		// パラメータ記述
		S_SHD_PARAMETER* m_pDesc;

		// パラメータアノテーション
		S_SHD_PARAM_ANN* m_pAnn;

		// パラメータバッファ
		IZ_UINT8* m_pParamBuffer;

		// パラメータバッファサイズ
		IZ_UINT m_nParamBufSize;
	};

	// inline ***********************************

	// コンストラクタ
	CShaderParameterTable::CShaderParameterTable()
	{
		FILL_ZERO(&m_Header, sizeof(m_Header));
		m_pDesc = IZ_NULL;
		m_pAnn = IZ_NULL;
		m_pParamBuffer = IZ_NULL;
		m_nParamBufSize = 0;
	}

	// 初期化
	IZ_UINT8* CShaderParameterTable::Init(IZ_UINT8* p)
	{
		// NOTE
		// +----------------+
		// |   パラメータ   |
		// |    テーブル    |
		// +----------------+
		// | アノテーション |
		// |  (パラメータ)  |
		// |    テーブル    |
		// +----------------+
		// |パラメータ初期値|
		// |    バッファ    |
		// +----------------+

		memcpy(&m_Header, p, sizeof(m_Header));
		p += sizeof(m_Header);

		// パラメータ記述
		if (m_Header.numParameter > 0) {
			m_pDesc = (S_SHD_PARAMETER*)p;
			p += sizeof(S_SHD_PARAMETER) * m_Header.numParameter;
		}

		// パラメータアノテーション
		if (m_Header.numParamAnn > 0) {
			m_pAnn = (S_SHD_PARAM_ANN*)p;
			p += sizeof(S_SHD_PARAM_ANN) * m_Header.numParamAnn;
		}

		// パラメータバッファ
		if (m_Header.sizeValueBuffer > 0) {
			m_pParamBuffer = p;
			m_nParamBufSize = m_Header.sizeValueBuffer;
			p += m_Header.sizeValueBuffer;

			IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(m_pParamBuffer);
			IZ_UINT nMappedSize = 0;
		
			for (IZ_UINT i = 0; i < m_Header.numParameter; i++) {
				S_SHD_PARAMETER& sDesc = m_pDesc[i];
				sDesc.ptrParam = pBuf;
				pBuf += sDesc.Bytes;

				nMappedSize += sDesc.Bytes;
				IZ_ASSERT(nMappedSize <= m_nParamBufSize);
			}
		}

		return p;
	}

	// パラメータセット
	IZ_BOOL CShaderParameterTable::SetParam(
		IZ_UINT idx,
		const void* pValue,
		IZ_UINT nBytes)
	{
		IZ_ASSERT(idx < m_Header.numParameter);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_SHD_PARAMETER& sDesc = m_pDesc[idx];
		VRETURN(sDesc.Bytes == nBytes);
		VRETURN(sDesc.ptrParam != IZ_NULL);

		memcpy(sDesc.ptrParam, pValue, nBytes);

		sDesc.isDirty = IZ_TRUE;

		return IZ_TRUE;
	}

	// パラメータ取得
	const void* CShaderParameterTable::GetParam(
		IZ_UINT idx,
		IZ_UINT* pBytes) const
	{
		IZ_ASSERT(idx < m_Header.numParameter);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		const S_SHD_PARAMETER& sDesc = m_pDesc[idx];
		IZ_ASSERT(sDesc.ptrParam != IZ_NULL);
		
		if (pBytes != IZ_NULL) {
			// パラメータのバイト数を返す
			*pBytes = sDesc.Bytes;
		}

		return sDesc.ptrParam;
	}

	// パラメータ記述取得
	const S_SHD_PARAMETER* CShaderParameterTable::GetDesc(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_Header.numParameter);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_SHD_PARAMETER* ret = &m_pDesc[idx];
		return ret;
	}

	// パラメータインデックス取得
	IZ_INT CShaderParameterTable::GetIdxByName(IZ_PCSTR pszName) const
	{
		IZ_UINT ret = GetIdxByNameKey(CKey(pszName));
		return ret;
	}

	IZ_INT CShaderParameterTable::GetIdxByNameKey(const CKey& cKey) const
	{
		IZ_UINT nKey = cKey.GetValue();

		for (IZ_UINT i = 0; i < m_Header.numParameter; ++i) {
			const S_SHD_PARAMETER& sDesc = m_pDesc[i];
			if (sDesc.keyName == nKey) {
				return i;
			}
		}

		return -1;
	}

	IZ_INT CShaderParameterTable::GetIdxBySemantic(IZ_PCSTR pszSemantic) const
	{
		IZ_UINT ret = GetIdxBySemanticKey(CKey(pszSemantic));
		return ret;
	}

	IZ_INT CShaderParameterTable::GetIdxBySemanticKey(const CKey& cKey) const
	{
		IZ_UINT nKey = cKey.GetValue();

		for (IZ_UINT i = 0; i < m_Header.numParameter; ++i) {
			const S_SHD_PARAMETER& sDesc = m_pDesc[i];
			if (sDesc.keySemantic == nKey) {
				return i;
			}
		}

		return -1;
	}

	// パラメータアノテーション取得
	const S_SHD_PARAM_ANN* CShaderParameterTable::GetAnn(IZ_UINT idx) const
	{
		IZ_ASSERT(m_pAnn != IZ_NULL);

		const S_SHD_PARAMETER* pDesc = GetDesc(idx);

		S_SHD_PARAM_ANN* ret = IZ_NULL;
		if (pDesc->hasAnn) {
			ret = &m_pAnn[pDesc->AnnotationIdx];
		}
		return ret;
	}

	void CShaderParameterTable::Reset(IZ_UINT idx)
	{
		IZ_ASSERT(idx < m_Header.numParameter);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_SHD_PARAMETER& sDesc = m_pDesc[idx];
		sDesc.isDirty = IZ_FALSE;
	}

	void CShaderParameterTable::ResetAll()
	{
		IZ_ASSERT(m_pDesc != IZ_NULL);

		for (IZ_UINT i = 0; i < m_Header.numParameter; ++i) {
			S_SHD_PARAMETER& sDesc = m_pDesc[i];
			sDesc.isDirty = IZ_FALSE;
		}
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__)
