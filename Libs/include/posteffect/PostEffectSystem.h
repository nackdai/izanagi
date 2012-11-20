#if !defined(__IZANAGI_POSTEFFECT_SYSTEM_H__)
#define __IZANAGI_POSTEFFECT_SYSTEM_H__

#include "izStd.h"
#include "vs/PostEffectVSManager.h"
#include "PostEffectTextureCreator.h"

namespace izanagi {
	class CPostEffect;
	class CPostEffectShader;
	class CPostEffectPass;
	class CPostEffectFunctor;
	struct S_PES_TEXTURE;

	/**
	* ポストエフェクトシステム
	*/
	class CPostEffectSystem : public CObject {
		friend class CPostEffect;
		friend class IPostEffectFunctorCreator;

	private:
		static CPostEffectSystem* s_pInstance;

	public:
		// インスタンス作成
		static CPostEffectSystem* CreatePostEffectSystem(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice);

		// インスタンス取得
		static CPostEffectSystem* GetPostEffectSystem() { return s_pInstance; }

	private:
		CPostEffectSystem();
		~CPostEffectSystem();

		NO_COPIABLE(CPostEffectSystem);

	public:
		// ポストエフェクトの作成
		CPostEffect* CreatePostEffect(IInputStream* in);

		// ファンクタクリエイタをセット
		IZ_BOOL AddFunctorCreator(IPostEffectFunctorCreator* pFunctorCreator);

		// テクスチャクリエータをセット
		void SetTextureCreator(CPostEffectTextureCreator* pTexCreator) { SAFE_REPLACE(m_pTexCreator, pTexCreator); }

		CPostEffectTextureCreator* GetTextureCreator() { return m_pTexCreator; }

	private:
		void InternalRelease();

		// 初期化
		IZ_BOOL InitInternal();

		// ファンクタ作成
		CPostEffectFunctor* CreateFunctor(
			IZ_UINT nKey,
			IZ_UINT nPassIdx,
			IZ_UINT8* pBuffer,
			size_t* pFunctorSize);

		// ファンクタサイズ取得
		size_t GetFunctorSize(IZ_PCSTR pszName);		

	private:
		enum {
			FUNCTOR_HASH_SIZE = 5,
		};

		typedef CStdHash<CKey, IPostEffectFunctorCreator, FUNCTOR_HASH_SIZE> CFunctorCreatorHash;
		typedef CFunctorCreatorHash::Item CFunctorCreatorHashItem;

	private:
		IMemoryAllocator* m_Allocator;
		graph::CGraphicsDevice* m_pDevice;

		// テクスチャ作成用
		CPostEffectTextureCreator* m_pTexCreator;

		// ファンクタ作成用
		CFunctorCreatorHash m_FunctorCreatorHash;

		// 頂点シェーダマネージャ
		CPostEffectVSManager m_cVSMgr;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SYSTEM_H__)
