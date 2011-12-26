#if !defined(__IZANAGI_POSTEFFECT_TEXTURE_CREATOR_H__)
#define __IZANAGI_POSTEFFECT_TEXTURE_CREATOR_H__

#include "izGraph.h"
#include "PostEffectDefs.h"
#include "util/PostEffectUtilityParam.h"

namespace izanagi {
	/**
	* テクスチャクリエータ
	*/
	class CPostEffectTextureCreator : public CObject {
	public:
		static CPostEffectTextureCreator* CreatePostEffectTextureCreator(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT nMaxTexNum);

	private:
		CPostEffectTextureCreator();
		virtual ~CPostEffectTextureCreator();

		NO_COPIABLE(CPostEffectTextureCreator);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// テクスチャ作成開始
		void BeginCreate();

		// テクスチャ作成終了
		void EndCreate() {}

		// テクスチャ作成
		CTexture* Create(
			IZ_UINT16 nWidth,
			IZ_UINT16 nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_BOOL bIsRenderTarget,
			E_GRAPH_RSC_TYPE nCreateType,
			E_POSTEFFECT_TEXTURE_TYPE type);

		// テクスチャを探す
		CTexture* Find(
			IZ_UINT16 nWidth,
			IZ_UINT16 nHeight,
			E_GRAPH_PIXEL_FMT fmt,
			IZ_BOOL bIsRenderTarget,
			E_GRAPH_RSC_TYPE nCreateType);

		void ClearAll();

		void SetEnableFind(IZ_BOOL flag) { m_bEnableFind = flag; }
		IZ_BOOL EnableFind() const { return m_bEnableFind; }

	private:
		enum {
			HASH_SIZE = 5,
		};

		struct STexHolder;

		typedef CStdHash<IZ_UINT, STexHolder, HASH_SIZE>	CTexHash;
		typedef CTexHash::Item	CTexHashItem;

	private:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		// テクスチャリスト
		STexHolder* m_pTexList;

		IZ_UINT m_nTexNum;
		IZ_UINT m_nTexPos;

		IZ_BOOL m_bEnableFind;

		CTexHash m_TexHash;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_TEXTURE_CREATOR_H__)
