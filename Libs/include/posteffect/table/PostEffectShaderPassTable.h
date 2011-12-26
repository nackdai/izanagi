#if !defined(__IZANAGI_POSTEFFECT_PASS_TABLE_H__)
#define __IZANAGI_POSTEFFECT_PASS_TABLE_H__

#include "izGraph.h"
#include "izIo.h"
#include "posteffect/PostEffectShaderPass.h"
#include "posteffect/PESFormat.h"
#include "posteffect/util/PostEffectUtilityParam.h"

namespace izanagi {
	class CPostEffectTechTable;

	/**
	* パステーブル
	*
	* テクニックの持つパスを管理するクラス
	*/
	class CPostEffectPassTable {
		friend class CPostEffectShader;

	private:
		inline CPostEffectPassTable();
		~CPostEffectPassTable() {}

		NO_COPIABLE(CPostEffectPassTable);

	private:
		// 初期化
		inline IZ_UINT8* Init(
			const S_PES_HEADER& sHeader,
			IZ_UINT8* p);

	private:
		// パス数取得
		inline IZ_UINT GetPassNum() const;

		// パス記述取得
		inline const S_PES_PASS* GetDesc(IZ_UINT idx) const;

	private:
		// パス数
		IZ_UINT m_nPassNum;

		// パス記述
		S_PES_PASS* m_pDesc;
	};

	// inline *******************************************

	// コンストラクタ
	CPostEffectPassTable::CPostEffectPassTable()
	{
		m_nPassNum = 0;
		m_pDesc = IZ_NULL;
	}

	// 初期化
	IZ_UINT8* CPostEffectPassTable::Init(
		const S_PES_HEADER& sHeader,
		IZ_UINT8* p)
	{
		// NOTE
		// +----------------+
		// |  パステーブル  |
		// +----------------+
		// |パスで利用される|
		// |  パラメータと  |
		// |   サンプラの   |
		// |  インデックス  |
		// +----------------+

		m_nPassNum = sHeader.numPass;

		// パス記述
		if (m_nPassNum > 0) {
			m_pDesc = (S_PES_PASS*)p;
			p += sizeof(S_PES_PASS) * m_nPassNum;
		}

		// 参照するパラメータ or サンプラインデックス
		if (m_nPassNum > 0) {
			// TODO
			// この条件でいいのか？
			IZ_UINT* pUsedParamIdxList = (IZ_UINT*)p;

			IZ_UINT nPos = 0;
			
			for (IZ_UINT i = 0; i < m_nPassNum; ++i) {
				S_PES_PASS& sDesc = m_pDesc[i];

				if (sDesc.numConst > 0) {
					sDesc.ptrConst = pUsedParamIdxList + nPos;
					p += sizeof(IZ_UINT) * sDesc.numConst;
					nPos += sDesc.numConst;
				}

				if (sDesc.numSampler > 0) {
					sDesc.ptrSampler = pUsedParamIdxList + nPos;
					p += sizeof(IZ_UINT) * sDesc.numSampler;
					nPos += sDesc.numSampler;
				}
			}
		}
		
		return p;
	}

	// パス数取得
	IZ_UINT CPostEffectPassTable::GetPassNum() const
	{
		return m_nPassNum;
	}

	// パス記述取得
	const S_PES_PASS* CPostEffectPassTable::GetDesc(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_nPassNum);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_PES_PASS* ret = &m_pDesc[idx];
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_PASS_TABLE_H__)
