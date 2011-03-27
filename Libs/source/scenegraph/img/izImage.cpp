#include "scenegraph/img/izImage.h"
#include "izIo.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CImage* CImage::CreateImage(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IInputStream* pInputStream)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pDevice != IZ_NULL);
	IZ_ASSERT(pInputStream != IZ_NULL);

	IZ_BYTE* pBuf = IZ_NULL;
	CImage* pInstance = IZ_NULL;

	// ヘッダ読み込み
	S_IMG_HEADER sHeader;
	IZ_BOOL result = IZ_INPUT_READ(pInputStream, &sHeader, 0, sizeof(sHeader));
	VGOTO(result, __EXIT__);

	// マジックナンバーチェック
	result = IS_IMG_FORMAT(sHeader.magic);
	VGOTO(result, __EXIT__);

	// 確保サイズ
	size_t nSize = sizeof(CImage) + sizeof(CTexture*) * sHeader.numTextures;

	// メモリ確保
	pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, nSize);
	result = (pBuf != IZ_NULL);
	VGOTO(result, __EXIT__);

	// インスタンス作成
	pInstance = new(pBuf) CImage;
	{
		pBuf += sizeof(CImage);

		pInstance->AddRef();

		pInstance->m_pAllocator = pAllocator;
		pInstance->m_pTexture = (CBaseTexture**)pBuf;

		memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));
	}

	// テクスチャ読み込み
	result = pInstance->ReadTexture(
				pDevice,
				pInputStream);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			FREE(pAllocator, pBuf);
		}
	}
	return pInstance;
}

namespace {
	// テクスチャ作成
	CBaseTexture* _CreateTexture(
		CGraphicsDevice* pDevice,
		IZ_UINT nWidth,
		IZ_UINT nHeight,
		IZ_UINT nMipLevel,
		E_GRAPH_PIXEL_FMT nFmt,
		IZ_BYTE** pBuf)
	{
		// NOTE
		// データセットするためにロックをしたいので、
		// 動的にテクスチャを作成する

		// テクスチャ作成
		CTexture* pTex = IZ_NULL;
		pTex = pDevice->CreateTexture(
				nWidth,
				nHeight,
				nMipLevel,
				nFmt,
				E_GRAPH_RSC_TYPE_STATIC);

		VRETURN_NULL(pTex != IZ_NULL);

		IZ_BOOL result = IZ_FALSE;

		for (IZ_UINT nLevel = 0; nLevel < nMipLevel; ++nLevel) {
			// データセット
			IZ_BYTE* data = IZ_NULL;
			IZ_UINT nPitch = pTex->Lock(
								nLevel, 
								reinterpret_cast<void**>(&data),
								IZ_FALSE);

			result = (nPitch > 0);
			VGOTO(result, __EXIT__);

			// TODO
			IZ_UINT nSize = nPitch * nHeight;

			memcpy(data, *pBuf, nSize); 

			pTex->Unlock(nLevel);

			*pBuf += nSize;

			nWidth >>= 1;
			nHeight >>= 1;
		}

__EXIT__:
		if (!result) {
			SAFE_RELEASE(pTex);
		}
		return pTex;
	}

	// キューブテクスチャ作成
	CBaseTexture* _CreateCubeTexture(
		CGraphicsDevice* pDevice,
		IZ_UINT nWidth,
		IZ_UINT nHeight,
		IZ_UINT nMipLevel,
		E_GRAPH_PIXEL_FMT nFmt,
		IZ_BYTE** pBuf)
	{
		// テクスチャ作成
		CCubeTexture* pTex = IZ_NULL;
		pTex = pDevice->CreateCubeTexture(
				nWidth,
				nHeight,
				nMipLevel,
				nFmt,
				IZ_TRUE);

		VRETURN_NULL(pTex != IZ_NULL);

		IZ_BOOL result = IZ_FALSE;

		for (IZ_UINT nFace = 0; nFace < E_GRAPH_CUBE_TEX_FACE_NUM; ++nFace) {
			IZ_UINT nW = nWidth;
			IZ_UINT nH = nHeight;

			for (IZ_UINT nLevel = 0; nLevel < nMipLevel; ++nLevel) {
				E_GRAPH_CUBE_TEX_FACE face = static_cast<E_GRAPH_CUBE_TEX_FACE>(nFace);

				// データセット
				IZ_BYTE* data = IZ_NULL;
				IZ_UINT nPitch = pTex->Lock(
									face,
									nLevel, 
									reinterpret_cast<void**>(&data),
									IZ_FALSE);

				result = (nPitch > 0);
				VGOTO(result, __EXIT__);

				// TODO
				IZ_UINT nSize = nPitch * nH;

				memcpy(data, *pBuf, nSize); 

				pTex->Unlock(face, nLevel);

				*pBuf += nSize;

				nW >>= 1;
				nH >>= 1;
			}
		}

__EXIT__:
		if (!result) {
			SAFE_RELEASE(pTex);
		}
		return pTex;
	}

	// テクスチャ作成
	inline CBaseTexture* _CreateTexture(
		CGraphicsDevice* pDevice,
		const S_IMG_TEX_HEADER& sTexHeader,
		IZ_BYTE** pBuf)
	{
		IZ_ASSERT(pDevice != IZ_NULL);

		IZ_UINT nWidth = 1 << sTexHeader.w;
		IZ_UINT nHeight = 1 << sTexHeader.h;

		IZ_UINT nMipLevel = sTexHeader.level;

		E_GRAPH_PIXEL_FMT nFmt = sTexHeader.fmt;

		E_GRAPH_TEX_TYPE nType = static_cast<E_GRAPH_TEX_TYPE>(sTexHeader.type);

		// テクスチャ作成関数テーブル
		typedef CBaseTexture* (*CreateTexFunc)(CGraphicsDevice*, IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT, IZ_BYTE**);
		static CreateTexFunc FuncTbl[] = {
			_CreateTexture,
			_CreateCubeTexture,
			IZ_NULL,
		};
		IZ_C_ASSERT(COUNTOF(FuncTbl) == E_GRAPH_TEX_TYPE_NUM);

		IZ_ASSERT(FuncTbl[nType] != IZ_NULL);

		CBaseTexture* pTex = IZ_NULL;

		// テクスチャ作成
		pTex = (*FuncTbl[nType])(
					pDevice,
					nWidth,
					nHeight,
					nMipLevel,
					nFmt,
					pBuf);

		IZ_ASSERT(pTex != IZ_NULL);
		return pTex;
	}
}	// namespace

// テクスチャ読み込み
IZ_BOOL CImage::ReadTexture(
	CGraphicsDevice* pDevice,
	IInputStream* pInputStream)
{
	IZ_ASSERT(m_pAllocator != IZ_NULL);
	IZ_ASSERT(pDevice != IZ_NULL);
	IZ_ASSERT(pInputStream != IZ_NULL);

	// 読み込みバッファメモリ確保
	IZ_BYTE* pBuf = (IZ_BYTE*)ALLOC_ZERO(m_pAllocator, m_Header.sizeMax);
	VRETURN(pBuf != IZ_NULL);

	// バッファ先頭位置を保持
	IZ_BYTE* pBufTop = pBuf;

	// ジャンプテーブル分飛ばす
	IZ_BOOL ret = pInputStream->Seek(m_Header.numTextures * sizeof(IZ_UINT), E_IO_STREAM_SEEK_POS_CUR);
	VGOTO(ret, __EXIT__);

	S_IMG_TEX_HEADER sTexHeader;

	// 読み込み
	for (IZ_UINT i = 0; i < m_Header.numTextures; ++i) {
		// ヘッダ読み込み
		ret = IZ_INPUT_READ(pInputStream, &sTexHeader, 0, sizeof(sTexHeader));
		VGOTO(ret, __EXIT__);

		// データ読み込み
		ret = IZ_INPUT_READ(pInputStream, pBuf, 0, sTexHeader.size);
		VGOTO(ret, __EXIT__);

		// テクスチャ作成
		m_pTexture[i] = _CreateTexture(
							pDevice,
							sTexHeader,
							&pBuf);
		ret = (m_pTexture[i] != IZ_NULL);
		VGOTO(ret, __EXIT__);

		// パラメータセット
		m_pTexture[i]->SetAddress(
			static_cast<E_GRAPH_TEX_ADDRESS>(sTexHeader.addressU),
			static_cast<E_GRAPH_TEX_ADDRESS>(sTexHeader.addressV));
		m_pTexture[i]->SetFilter(
			static_cast<E_GRAPH_TEX_FILTER>(sTexHeader.minFilter),
			static_cast<E_GRAPH_TEX_FILTER>(sTexHeader.magFilter),
			static_cast<E_GRAPH_TEX_FILTER>(sTexHeader.mipFilter));
	}

__EXIT__:
	FREE(m_pAllocator, pBufTop);
	return ret;
}
