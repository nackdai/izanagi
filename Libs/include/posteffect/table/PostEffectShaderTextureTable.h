#if !defined(__POSTEFFECT_TEXTURE_TABLE_H__)
#define __POSTEFFECT_TEXTURE_TABLE_H__

#include "posteffect/PESFormat.h"
#include "posteffect/util/PostEffectShaderUtil.h"

namespace izanagi {
	/**
	* テクスチャテーブル
	*
	* シェーダ内のテクスチャを管理するクラス
	*/
	class CPostEffectTextureTable {
		friend class CPostEffectShader;
		friend class CPostEffect;

		struct STexHolder {
			IZ_BOOL isDirty;
			CTexture* tex;

			STexHolder()
			{
				isDirty = IZ_FALSE;
				tex = IZ_NULL;
			}
			void SetTexture(CTexture* _tex)
			{
				SAFE_REPLACE(tex, _tex);
				isDirty = (tex != IZ_NULL);
			}
			void Clean()
			{
				isDirty = IZ_FALSE;
			}
			void Release()
			{
				SAFE_RELEASE(tex);
				isDirty = IZ_FALSE;
			}
		};

	public:
		static inline size_t ComputeTexBufferSize(IZ_UINT nTexNum);

	private:
		inline CPostEffectTextureTable();
		~CPostEffectTextureTable() { Release(); }

		NO_COPIABLE(CPostEffectTextureTable);

	private:
		// 初期化
		inline IZ_UINT8* Init(
			const S_PES_HEADER& sHeader,
			IZ_UINT8* p);

		// テクスチャバッファ設定
		inline IZ_UINT8* SetTexBuffer(IZ_UINT8* p);

		inline IZ_UINT8* SetTexBufferDirectly(
			IZ_UINT nTexNum,
			IZ_UINT8* p);

		// 解放
		inline void Release();

	private:
		// テクスチャ記述取得
		inline const S_PES_TEXTURE* GetDesc(IZ_UINT idx) const;

		// インデックス取得
		inline IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
		inline IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;

		// テクスチャハンドルをセット
		inline IZ_BOOL SetTexture(IZ_UINT idx, CTexture* pTex);
		inline IZ_BOOL SetTextureByName(IZ_PCSTR pszName, CTexture* pTex);
		inline IZ_BOOL SetTextureBySemantic(IZ_PCSTR pszSemantic, CTexture* pTex);

		// テクスチャの有無を確認
		inline IZ_BOOL HasTextureBySemantic(IZ_PCSTR pszSemantic);

		// テクスチャを取得
		inline CTexture* GetTexture(IZ_UINT idx);

		// テクスチャ数を取得
		inline IZ_UINT GetTexNum() const;

		// テクスチャタイプごとの枚数を取得
		inline IZ_INT GetTexNumByTextureType(E_POSTEFFECT_TEXTURE_TYPE type) const;

		// 指定されたテクスチャタイプの取得
		// 複数ある場合は最初に見つかったものを返す
		inline CTexture* GetTextureByTextureType(E_POSTEFFECT_TEXTURE_TYPE type);

	private:
		inline void Clean();

		inline CTexture* GetTextureIfDirty(IZ_UINT idx);

	private:
		// テクスチャ数
		IZ_UINT m_nTexNum;

		// テクスチャ記述
		S_PES_TEXTURE* m_pDesc;

		// テクスチャバッファ
		STexHolder* m_pTexBuffer;
	};

	// inline ************************************

	// テクスチャバッファに必要なサイズを計算
	size_t CPostEffectTextureTable::ComputeTexBufferSize(IZ_UINT nTexNum)
	{
		size_t ret = sizeof(STexHolder) * nTexNum;
		return ret;
	}

	// コンストラクタ
	CPostEffectTextureTable::CPostEffectTextureTable()
	{
		m_nTexNum = 0;
		m_pDesc = IZ_NULL;
		m_pTexBuffer = IZ_NULL;
	}

	// 初期化
	IZ_UINT8* CPostEffectTextureTable::Init(
		const S_PES_HEADER& sHeader,
		IZ_UINT8* p)
	{
		// NOTE
		// +----------------+
		// |   テクスチャ   |
		// |    テーブル    |
		// +----------------+

		m_nTexNum = sHeader.numTex;

		// テクスチャ記述
		if (m_nTexNum > 0) {
			m_pDesc = (S_PES_TEXTURE*)p;
			p += sizeof(S_PES_TEXTURE) * m_nTexNum;
		}

		return p;
	}

	// テクスチャハンドルバッファ設定
	IZ_UINT8* CPostEffectTextureTable::SetTexBuffer(IZ_UINT8* p)
	{
		if (m_nTexNum > 0) {
#if 0
			m_pTexBuffer = (CTexture**)p;
			p += sizeof(CTexture*) * m_nTexNum;
#else
			m_pTexBuffer = (STexHolder*)p;
			p += sizeof(STexHolder) * m_nTexNum;
#endif
		}

		return p;
	}

	IZ_UINT8* CPostEffectTextureTable::SetTexBufferDirectly(
		IZ_UINT nTexNum,
		IZ_UINT8* p)
	{
		IZ_ASSERT(m_pTexBuffer == IZ_NULL);

		m_nTexNum = nTexNum;
		IZ_UINT8* ret = SetTexBuffer(p);
		return ret;
	}

	// 解放
	void CPostEffectTextureTable::Release()
	{
		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			//SAFE_RELEASE(m_pTexBuffer[i].tex);
			m_pTexBuffer[i].Release();
		}
	}

	// テクスチャ記述取得
	const S_PES_TEXTURE* CPostEffectTextureTable::GetDesc(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < m_nTexNum);
		IZ_ASSERT(m_pDesc != IZ_NULL);

		S_PES_TEXTURE* ret = &m_pDesc[idx];
		return ret;
	}

	// インデックス取得
	IZ_INT CPostEffectTextureTable::GetIdxByName(IZ_PCSTR pszName) const
	{
		IZ_ASSERT(m_pDesc != IZ_NULL);

		IZ_UINT nKey = CKey::GenerateValue(pszName);

		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			const S_PES_TEXTURE& sDesc = m_pDesc[i];
			if (sDesc.keyName == nKey) {
				return i;
			}
		}

		return -1;
	}

	IZ_INT CPostEffectTextureTable::GetIdxBySemantic(IZ_PCSTR pszSemantic) const
	{
		IZ_ASSERT(m_pDesc != IZ_NULL);

		IZ_UINT nKey = CKey::GenerateValue(pszSemantic);

		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			const S_PES_TEXTURE& sDesc = m_pDesc[i];
			if (sDesc.keySemantic == nKey) {
				return i;
			}
		}

		return -1;
	}

	// テクスチャハンドルをセット
	IZ_BOOL CPostEffectTextureTable::SetTexture(IZ_UINT idx, CTexture* pTex)
	{
		IZ_ASSERT(m_pTexBuffer != IZ_NULL);

		IZ_BOOL ret = (idx < m_nTexNum);
		if (ret) {
			m_pTexBuffer[idx].SetTexture(pTex);
		}
		return ret;
	}

	IZ_BOOL CPostEffectTextureTable::SetTextureByName(IZ_PCSTR pszName, CTexture* pTex)
	{
		IZ_INT nIdx = GetIdxByName(pszName);

		IZ_BOOL ret = (nIdx >= 0);
		VRETURN(ret);

		if (ret) {
			ret = SetTexture(nIdx, pTex);
			VRETURN(ret);
		}

		return ret;
	}

	IZ_BOOL CPostEffectTextureTable::SetTextureBySemantic(IZ_PCSTR pszSemantic, CTexture* pTex)
	{
		IZ_INT nIdx = GetIdxBySemantic(pszSemantic);

		IZ_BOOL ret = (nIdx >= 0);
		VRETURN(ret);

		if (ret) {
			ret = SetTexture(nIdx, pTex);
			VRETURN(ret);
		}

		return ret;
	}

	// テクスチャの有無を確認
	IZ_BOOL CPostEffectTextureTable::HasTextureBySemantic(IZ_PCSTR pszSemantic)
	{
		IZ_INT nIdx = GetIdxBySemantic(pszSemantic);
		IZ_BOOL ret = (GetTexture(nIdx) != IZ_NULL);
		return ret;
	}

	// テクスチャハンドルを取得
	CTexture* CPostEffectTextureTable::GetTexture(IZ_UINT idx)
	{
		CTexture* ret = IZ_NULL;

		if (idx < m_nTexNum) {
			ret = m_pTexBuffer[idx].tex;
		}

		return ret;
	}

	// テクスチャ数を取得
	IZ_UINT CPostEffectTextureTable::GetTexNum() const
	{
		return m_nTexNum;
	}

	// テクスチャタイプごとの枚数を取得
	IZ_INT CPostEffectTextureTable::GetTexNumByTextureType(E_POSTEFFECT_TEXTURE_TYPE type) const
	{
		IZ_INT ret = 0;

		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			ret = (m_pDesc[i].type == type ? ret + 1 : ret);
		}

		return ret;
	}

	// 指定されたテクスチャタイプの取得
	// 複数ある場合は最初に見つかったものを返す
	CTexture* CPostEffectTextureTable::GetTextureByTextureType(E_POSTEFFECT_TEXTURE_TYPE type)
	{
		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			if (m_pDesc[i].type == type) {
				return m_pTexBuffer[i].tex;
			}
		}
		return IZ_NULL;
	}

	void CPostEffectTextureTable::Clean()
	{
		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			m_pTexBuffer[i].Clean();
		}
	}

	CTexture* CPostEffectTextureTable::GetTextureIfDirty(IZ_UINT idx)
	{
		CTexture* ret = IZ_NULL;

		if ((idx < m_nTexNum)
			&& (m_pTexBuffer[idx].isDirty))
		{
			ret = m_pTexBuffer[idx].tex;
			IZ_ASSERT(ret != IZ_NULL);
		}

		return ret;
	}

}	// namespace izanagi

#endif	// #if !defined(__POSTEFFECT_TEXTURE_TABLE_H__)
