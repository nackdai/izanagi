#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_H__

#include "izGraph.h"
#include "posteffect/PESFormat.h"
#include "posteffect/PostEffect.h"
#include "posteffect/util/PostEffectUtilityParam.h"
#include "posteffect/PostEffectTextureCreator.h"

namespace izanagi {
	class CPostEffectShader;

	/**
	* ファンクタ
	*/
	class CPostEffectFunctor : public CObject {
		friend class CPostEffect;

	public:
		// ファンクタ作成(メモリを内部確保しない)
		template <class _T>
		static inline CPostEffectFunctor* CreateFunctor(
			const CKey& cKey,
			void* pBuffer);

	protected:
		inline CPostEffectFunctor();
		virtual ~CPostEffectFunctor() {}

		NO_COPIABLE(CPostEffectFunctor);

		// 解放
		void InternalRelease() { delete this; }

	public:
		// ファンクタ処理
		PURE_VIRTUAL(
			IZ_BOOL Apply(
				graph::CGraphicsDevice* pDevice,
				CPostEffectShader* pShader));

		// ファンクタ独自のテクスチャ作成
		virtual IZ_BOOL CreateTexture(
			CPostEffect* pPostEffect,
			CPostEffectShader* pShader,
			CPostEffectTextureCreator* pTexCreator,
			graph::CGraphicsDevice* pDevice)
		{
			return IZ_TRUE;
		}

		// リセット
		virtual void Reset() {}

		// ファンクタ引数セット
		virtual void SetFunctorArgs(const IZ_FLOAT* pArgs) {}

	public:
		// 対応するテクニックインデックスの取得
		IZ_UINT GetTechIdx() const { return m_nTechIdx; }

		// 対応するパスインデックスの取得
		IZ_UINT GetPassIdx() const { return m_nPassIdx; }

		// 関連付けられているパスインデックスの取得
		inline IZ_INT8 GetConnectPassIdx(IZ_UINT pos) const;

	protected:
		void InitHash(const CKey& cHashKey) { m_HashItem.Init(cHashKey, this); }

		// 初期化パラメータセット
		inline void Init(
			IZ_UINT nTechID,
			IZ_UINT nPassID);

		// 関連付けられているパスインデックスのセット
		inline void SetConnectPassIdx(IZ_UINT nIdx);

		CPostEffect::CFunctorHashItem* GetHashItem() { return &m_HashItem; }

	protected:
		// 対応するテクニック
		IZ_INT16 m_nTechIdx;

		// 対応するパス
		IZ_INT16 m_nPassIdx;

		// 関連付けられているパス
		IZ_INT8 m_nConnectPassIdx[PES_CONNECT_PASS_NUM];
		IZ_UINT m_nConnectNum;

		CPostEffect::CFunctorHashItem m_HashItem;
	};

	// inline ********************************

	/**
	* ファンクタ作成
	*/
	template <class _T>
	CPostEffectFunctor* CPostEffectFunctor::CreateFunctor(
		const CKey& cKey,
		void* pBuffer)
	{
		_T* ret = IZ_NULL;

		if (pBuffer != IZ_NULL) {
			ret = new(pBuffer) _T;
			ret->InitHash(cKey);
		}

		return ret;
	}

	// コンストラクタ
	CPostEffectFunctor::CPostEffectFunctor()
	{
		for (IZ_UINT i = 0; i < PES_CONNECT_PASS_NUM; ++i) {
			m_nConnectPassIdx[i] = -1;
		}

		m_nTechIdx = -1;
		m_nPassIdx = -1;

		m_nConnectNum = 0;
	}

	/**
	* 関連付けられているパスインデックスの取得
	*/
	IZ_INT8 CPostEffectFunctor::GetConnectPassIdx(IZ_UINT pos) const
	{
		IZ_ASSERT(pos < PES_CONNECT_PASS_NUM);
		return m_nConnectPassIdx[pos];
	}

	// 初期化パラメータセット
	void CPostEffectFunctor::Init(
		IZ_UINT nTechID,
		IZ_UINT nPassID)
	{
		m_nTechIdx = nTechID;
		m_nPassIdx = nPassID;
	}

	// 関連付けられているパスインデックスのセット
	void CPostEffectFunctor::SetConnectPassIdx(IZ_UINT nIdx)
	{
		IZ_ASSERT(m_nConnectNum < PES_CONNECT_PASS_NUM);
		m_nConnectPassIdx[m_nConnectNum++] = nIdx;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_H__)
