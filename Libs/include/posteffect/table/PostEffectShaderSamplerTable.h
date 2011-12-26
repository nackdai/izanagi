#if !defined(__IZANAGI_POSTEFFECT_SMAPLER_TABLE_H__)
#define __IZANAGI_POSTEFFECT_SMAPLER_TABLE_H__

#include "../PESFormat.h"

namespace izanagi {
	/**
	* サンプラテーブル
	*
	* シェーダ内のサンプラを管理するクラス
	*/
	class CPostEffectSamplerTable{
		friend class CPostEffectShader;
		friend class CPostEffectSystem;

	private:
		inline CPostEffectSamplerTable();
		~CPostEffectSamplerTable() {}

		NO_COPIABLE(CPostEffectSamplerTable);

	private:
		// 初期化
		inline IZ_UINT8* Init(
			const S_PES_HEADER& sHeader,
			IZ_UINT8* p);

		// サンプラ記述取得
		inline const S_PES_SAMPLER* GetDesc(IZ_UINT idx) const;

	private:
		// サンプラ数
		IZ_UINT m_nSmplNum;

		// サンプラ記述
		S_PES_SAMPLER* m_pDesc;
	};

	// inline *****************************

	// コンストラクタ
	CPostEffectSamplerTable::CPostEffectSamplerTable()
	{
		m_nSmplNum = 0;
		m_pDesc = IZ_NULL;
	}

	// 初期化
	IZ_UINT8* CPostEffectSamplerTable::Init(
		const S_PES_HEADER& sHeader,
		IZ_UINT8* p)
	{
		// NOTE
		// +----------------+
		// |サンプラテーブル|
		// +----------------+

		m_nSmplNum = sHeader.numSampler;

		if (m_nSmplNum > 0) {
			// サンプラ記述
			m_pDesc = (S_PES_SAMPLER*)p;
			p += sizeof(S_PES_SAMPLER) * m_nSmplNum;
		}

		return p;
	}

	// サンプラ記述取得
	const S_PES_SAMPLER* CPostEffectSamplerTable::GetDesc(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_nSmplNum);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_PES_SAMPLER* ret = &m_pDesc[idx];
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SMAPLER_TABLE_H__)
