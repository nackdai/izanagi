#if !defined(__IZANAGI_POSTEFFECT_TECH_TABLE_H__)
#define __IZANAGI_POSTEFFECT_TECH_TABLE_H__

#include "../PESFormat.h"

namespace izanagi {
	/**
	* テクニックテーブル
	*
	* シェーダ内のテクニックを管理するクラス
	*/
	class CPostEffectTechTable {
		friend class CPostEffect;
		friend class CPostEffectShader;

	private:
		inline CPostEffectTechTable();
		~CPostEffectTechTable() {}

		NO_COPIABLE(CPostEffectTechTable);

	private:
		// 初期化
		inline IZ_UINT8* Init(
			const S_PES_HEADER& sHeader,
			IZ_UINT8* p);

		// テクニック記述取得
		inline const S_PES_TECHNIQUE* GetDesc(IZ_UINT idx) const;

		// 所属パスインデックス取得
		inline IZ_UINT GetPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassPos) const;

	private:
		// テクニック数
		IZ_UINT m_nTechNum;

		// テクニック記述
		S_PES_TECHNIQUE* m_pDesc;
	};

	// inline ********************************

	// コンストラクタ
	CPostEffectTechTable::CPostEffectTechTable()
	{
		m_nTechNum = 0;
		m_pDesc = IZ_NULL;
	}

	// 初期化
	IZ_UINT8* CPostEffectTechTable::Init(
		const S_PES_HEADER& sHeader,
		IZ_UINT8* p)
	{
		// NOTE
		// +----------------+
		// |   テクニック   |
		// |    テーブル    |
		// +----------------+

		m_nTechNum = sHeader.numTech;

		if (m_nTechNum > 0) {
			// テクニック記述
			m_pDesc = (S_PES_TECHNIQUE*)p;
			p += sizeof(S_PES_TECHNIQUE) * m_nTechNum;
		}

		return p;
	}

	// テクニック記述取得
	const S_PES_TECHNIQUE* CPostEffectTechTable::GetDesc(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_nTechNum);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_PES_TECHNIQUE* ret = &m_pDesc[idx];
		return ret;
	}

	// 所属パスインデックス取得
	IZ_UINT CPostEffectTechTable::GetPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassPos) const
	{
		IZ_ASSERT(nTechIdx < m_nTechNum);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		const S_PES_TECHNIQUE& sDesc = m_pDesc[nTechIdx];

		IZ_ASSERT(nPassPos < sDesc.numPass);
		IZ_UINT ret = sDesc.posPass + nPassPos;

		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_TECH_TABLE_H__)
