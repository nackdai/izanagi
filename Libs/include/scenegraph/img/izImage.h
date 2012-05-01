#if !defined(__IZANAGI_SCENEGRAPH_IZ_IMAGE_H__)
#define __IZANAGI_SCENEGRAPH_IZ_IMAGE_H__

#include "izStd.h"
#include "izGraph.h"
#include "IMGFormat.h"

namespace izanagi {
	class IInputStream;

	/**
	*/
	class CImage : public CObject {
	public:
		// インスタンス作成
		static CImage* CreateImage(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IInputStream* pInputStream);

	protected:
		inline CImage();
		inline ~CImage();

		NO_COPIABLE(CImage);

	protected:
		// 解放
		inline void InternalRelease();

		// テクスチャ読み込み
		IZ_BOOL ReadTexture(
			CGraphicsDevice* pDevice,
			IInputStream* pInputStream);

	public:
		// ヘッダ取得
		inline const S_IMG_HEADER& GetHeader() const;

		// テクスチャ数取得
		inline IZ_UINT GetTexNum() const;

		// テクスチャ取得
		inline CBaseTexture* GetTexture(IZ_UINT nIdx);

	protected:
		IMemoryAllocator* m_Allocator;

		S_IMG_HEADER m_Header;
		CBaseTexture** m_pTexture;
	};

	// inline **********************************

	// コンストラクタ
	CImage::CImage()
	{
		m_Allocator = IZ_NULL;
		m_pTexture = IZ_NULL;
	}

	// デストラクタ
	CImage::~CImage()
	{
		for (IZ_UINT i = 0; i < m_Header.numTextures; ++i) {
			SAFE_RELEASE(m_pTexture[i]);
		}
	}

	// 解放
	void CImage::InternalRelease()
	{
		delete this;
		if (m_Allocator != IZ_NULL) {
			FREE(m_Allocator, this);
		}
	}

	/**
	* ヘッダ取得
	*/
	const S_IMG_HEADER& CImage::GetHeader() const
	{
		return m_Header;
	}

	/**
	* テクスチャ数取得
	*/
	IZ_UINT CImage::GetTexNum() const
	{
		return m_Header.numTextures;
	}

	/**
	* テクスチャ取得
	*/
	CBaseTexture* CImage::GetTexture(IZ_UINT nIdx)
	{
		IZ_ASSERT(nIdx < m_Header.numTextures);
		IZ_ASSERT(m_pTexture != IZ_NULL);
		return m_pTexture[nIdx];
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_IMAGE_H__)
