#if !defined(__IZANAGI_POSTEFFECT_POSTEFFECT_H__)
#define __IZANAGI_POSTEFFECT_POSTEFFECT_H__

#include "izStd.h"
#include "PostEffectDefs.h"
#include "PostEffectShader.h"
#include "PostEffectShaderPass.h"

namespace izanagi {
	class CPostEffectVSManager;
	class CPostEffectFunctor;
	class CPostEffectTextureCreator;

	/**
	* ポストエフェクト本体
	*/
	class CPostEffect : public CObject {
		friend class CPostEffectSystem;
		friend class CPostEffectFunctor;

	public:
		// For Tool
		static size_t ComputeSizePassBuffForParamIdx(
			IZ_UINT nConstNum,
			IZ_UINT nSamplerNum)
		{
			size_t ret = CPostEffectPass::ComputeBufferSize(
							nConstNum,
							nSamplerNum);
			return ret;
		}

	private:
		// インスタンス作成
		static CPostEffect* CreatePostEffect(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			CPostEffectTextureCreator* pTexCreator,
			CPostEffectVSManager* pVSMgr,
			IInputStream* in);

		static size_t ComputeFunctorSize(CPostEffectShader* pShader);

	private:
		CPostEffect();
		~CPostEffect();

		NO_COPIABLE(CPostEffect);

		IZ_DEFINE_INTERNAL_RELEASE();

	private:
		IZ_UINT8* CreateFunctor(IZ_UINT8* pBuffer);

		IZ_BOOL CreateTexture(CPostEffectTextureCreator* pTexCreator);
		IZ_BOOL CreateTexture(
			CPostEffectTextureCreator* pTexCreator,
			IZ_UINT nPassIdx,
			IZ_UINT& nCreatedTexTblPos,
			const CTexture* tblCreatedTex[]);
		CTexture* CreateTexture(
			CPostEffectTextureCreator* pTexCreator,
			const S_PES_TEXTURE* pTexDesc);

	public:
		// ポストエフェクト実行
		IZ_BOOL Apply(
			CTexture* pInputSceneTex,
			IZ_UINT nEnableTechIdx);

		// リセット
		void Reset(IZ_UINT nTechIdx);

		IZ_POSTEFFECT_HANDLE GetParamHandleByName(IZ_PCSTR pszName) const { return m_pShader->GetParamHandleByName(pszName); }
		IZ_POSTEFFECT_HANDLE GetParamHandleBySemantic(IZ_PCSTR pszSemantic) const { return m_pShader->GetParamHandleBySemantic(pszSemantic); }

		// パラメータをセットする
		IZ_BOOL SetParameter(
			IZ_POSTEFFECT_HANDLE nHandle,
			const void* pValue,
			IZ_UINT nBytes)
		{
			return m_pShader->SetParameter(nHandle, pValue, nBytes);
		}

		// テクスチャをセットする
		IZ_BOOL SetTexture(
			IZ_POSTEFFECT_HANDLE nHandle,
			CTexture* pTex)
		{
			return m_pShader->SetTexture(nHandle, pTex);
		}

		CTexture* GetTexture(IZ_UINT nIdx) { return m_pShader->GetTexture(nIdx); }

	public:
		// パラメータ総数
		IZ_UINT GetParamNum() const { return m_pShader->GetHeader().numParam; }

		// テクスチャ総数
		IZ_UINT GetTexNum() const { return m_pShader->GetHeader().numTex; }

		// サンプラ数取得
		IZ_UINT GetSamplerNum() const { return m_pShader->GetHeader().numSampler; }

		// パス数取得
		IZ_UINT GetPassNum() const { return m_pShader->GetHeader().numPass; }

		// テクニック数取得
		IZ_UINT GetTechniqueNum() const { return m_pShader->GetHeader().numTech; }

		// ID取得
		IZ_PCSTR GetID() const { return m_pShader->GetHeader().pesID; }

		// INPUT_SCENEテクスチャ取得
		CTexture* GetInputSceneTex() { return m_pShader->GetTextureTable().GetTextureByTextureType(E_POSTEFFECT_TEXTURE_TYPE_INPUT_SCENE); }

	private:
		// シェーダ取得
		CPostEffectShader* GetShader() { return m_pShader; }

		// ファンクタを取得
		inline CPostEffectFunctor* GetFunctor(IZ_UINT nFunctorKey);

		CStdList<CPostEffect>::Item* GetListItem() { return &m_ListItem; }

	private:
		enum {
			FUNCTOR_HASH_SIZE = 5,
		};

		typedef CStdHash<CKey, CPostEffectFunctor, FUNCTOR_HASH_SIZE> CFunctorHash;
		typedef CFunctorHash::Item CFunctorHashItem;
	
	private:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CPostEffectShader* m_pShader;

		IZ_UINT16 m_nTechNum;		// テクニック数
		IZ_INT16 m_nEnableTechIdx;	// 有効なテクニックインデックス

		CFunctorHash m_FunctorHash;

		CStdList<CPostEffect>::Item m_ListItem;
	};

	// inline *********************************
	// ファンクタを取得
	CPostEffectFunctor* CPostEffect::GetFunctor(IZ_UINT nFunctorKey)
	{
		CPostEffectFunctor* ret = m_FunctorHash.FindData(nFunctorKey);
		IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_POSTEFFECT_H__)
