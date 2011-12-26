#if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
#define __IZANAGI_GRAPH_CUBE_TEXTURE_H__

#include "BaseTexture.h"

namespace izanagi {
	class CGraphicsDevice;

	class CCubeTexture : public CBaseTexture {
		friend class CGraphicsDevice;

	private:
		// ファイルからテクスチャ作成
		static CCubeTexture* CreateCubeTextureFromFile(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		static CCubeTexture* CreateCubeTexture(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt);

	private:
		typedef IZ_BOOL (CCubeTexture::*CreateTexFunc)(IZ_PCSTR, IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT);

		static CCubeTexture* CreateInternal(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_PCSTR lpszPathName,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			CreateTexFunc pCreateTexFunc);

	private:
		inline CCubeTexture();
		inline ~CCubeTexture();

		CCubeTexture(const CCubeTexture& rhs);
		const CCubeTexture& operator=(const CCubeTexture& rhs);

	private:
		// 解放
		void InternalRelease();

		// ファイルからテクスチャ作成
		IZ_BOOL CreateTextureFromFileImpl(
			IZ_PCSTR lpszPathName,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT nFmt);

		// テクスチャ作成
		IZ_BOOL CreateTextureImpl(
			IZ_PCSTR lpszPathName,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT nFmt);

		// テクスチャ情報取得
		void GetTextureInfo();

	public:
		// ロック
		IZ_UINT Lock(
			E_GRAPH_CUBE_TEX_FACE nFace,
			UINT nLevel,
			void** data,
			IZ_BOOL bIsReadOnly,
			IZ_BOOL bIsDiscard = IZ_FALSE);

		// アンロック
		IZ_BOOL Unlock(
			E_GRAPH_CUBE_TEX_FACE nFace,
			IZ_UINT nLevel);

	public:
		inline TEX_HANDLE GetTexHandle();

	public:
		D3D_CUBETEXTURE* GetRawInterface() { return m_pTexture; }

	private:
		// 本体
		D3D_CUBETEXTURE* m_pTexture;
	};

	// inline ***********************************************
	
	// コンストラクタ
	CCubeTexture::CCubeTexture()
	{
		m_pTexture = IZ_NULL;
	}

	// デストラクタ
	CCubeTexture::~CCubeTexture()
	{
		SAFE_RELEASE(m_pTexture);
	}

	TEX_HANDLE CCubeTexture::GetTexHandle()
	{
		return m_pTexture;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
