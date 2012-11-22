#if !defined(__IZANAGI_IMAGE_LIB_IMG_BODY_H__)
#define __IZANAGI_IMAGE_LIB_IMG_BODY_H__

#include <vector>
#include "izDefs.h"
#include "izGraph.h"
#include "izStd.h"
#include "IMGTexture.h"
#include "IMGImage.h"

namespace izanagi {
namespace tool {
	/**
	*/
	class CIMGMaster {
	public:
		CIMGMaster();
		~CIMGMaster();

		NO_COPIABLE(CIMGMaster);

	public:
		// テクスチャ追加
		IZ_BOOL Add(CIMGTexture* pTex);

		// テクスチャ削除
		IZ_BOOL Remove(IZ_UINT nIdx);

		// テクスチャ全削除
		void RemoveAll();

	public:
		// 読み込み
		IZ_BOOL Read(LPCSTR lpszPath);

		// 出力
		IZ_BOOL Write(LPCSTR lpszPath);

	public:
		// テクスチャ数取得
		inline IZ_UINT GetTexNum() const;

		// テクスチャ取得
		inline CIMGTexture* GetTexture(IZ_UINT nIdx);
		inline CIMGTexture* GetTexture(const CKey& cKey);

	private:
		std::vector<CIMGTexture*> m_Textures;
	};

	// inline ************************************:

	/**
	* テクスチャ数取得
	*/
	IZ_UINT CIMGMaster::GetTexNum() const
	{
		IZ_UINT ret = static_cast<IZ_UINT>(m_Textures.size());
		return ret;
	}

	/**
	* テクスチャ取得
	*/
	CIMGTexture* CIMGMaster::GetTexture(IZ_UINT nIdx)
	{
		IZ_ASSERT(nIdx < GetTexNum());
		CIMGTexture* ret = m_Textures[nIdx];
		return ret;
	}

	/**
	* テクスチャ取得
	*/
	CIMGTexture* CIMGMaster::GetTexture(const CKey& cKey)
	{
		CIMGTexture* ret = IZ_NULL;

		for (std::vector<CIMGTexture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++) {
			CIMGTexture* pTex = *it;

			// TODO
		}

		return ret;
	}
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_IMG_BODY_H__)
