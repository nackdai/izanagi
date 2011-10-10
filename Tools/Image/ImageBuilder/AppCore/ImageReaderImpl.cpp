#include "ImageReaderImpl.h"
#include "izToolKit.h"
#include "izImageLib.h"

using namespace izanagi;
using namespace izanagi_tk;

namespace {
	IZ_BOOL _SetDataAsPlane(
		IZ_UINT nWidth,
		IZ_UINT nHeight,
		IZ_UINT nLevel,
		E_GRAPH_PIXEL_FMT nFmt,
		CTextureLite* pSrcTex,
		CIMGTexture** pDstTex)
	{
		// 初期化
		(*pDstTex)->InitAsPlane(
			nWidth,
			nHeight,
			nLevel,
			nFmt);

		// データ取得
		for (IZ_UINT i = 0; i < nLevel; i++) {
			IZ_UINT nW = nWidth >> i;
			IZ_UINT nH = nHeight >> i;

			// データサイズ計算
			IZ_UINT nSize = (*pDstTex)->GetImageSize(0, i);

			void* dst = (*pDstTex)->GetImage(0)[i]->GetDataAsByte();

			void* src = IZ_NULL;
			IZ_UINT nPitch = pSrcTex->Lock(i, reinterpret_cast<void**>(&src));
			IZ_ASSERT(src != IZ_NULL);

			// コピー
			memcpy(dst, src, nSize);

			pSrcTex->Unlock(i);
		}

		return IZ_TRUE;
	}
}	// namespace

// 読み込み
CIMGTexture* CImageReaderImpl::Read(
	LPCSTR lpszPath,
	izanagi::E_GRAPH_TEX_TYPE nType)
{
	// グラフィックスデバイス取得
	CGraphicsDeviceLite* pDevice = CGraphicsDeviceLite::GetInstance();
	IZ_ASSERT(pDevice != IZ_NULL);

	// 読み込み
	CTextureLite* pTex = pDevice->CreateTextureFromFile(lpszPath);
	VRETURN_NULL(pTex);

	// テクスチャインスタンス作成
	CIMGTexture* ret = new CIMGTexture();
	IZ_ASSERT(ret != IZ_NULL);

	// データセット関数テーブル
	typedef IZ_BOOL (*SetDataFunc)(IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT, CTextureLite*, CIMGTexture**);
	static SetDataFunc func[] = {
		_SetDataAsPlane,
		IZ_NULL,
		IZ_NULL,
	};
	IZ_C_ASSERT(COUNTOF(func) == E_GRAPH_TEX_TYPE_NUM);

	IZ_UINT nWidth = pTex->GetWidth();
	IZ_UINT nHeight = pTex->GetHeight();
	IZ_UINT nLevel = pTex->GetMipMapNum();
	E_GRAPH_PIXEL_FMT nFmt = pTex->GetPixelFormat();

	// データセット
	IZ_BOOL result = (*func[nType])(
						nWidth,
						nHeight,
						nLevel,
						nFmt,
						pTex,
						&ret);
	if (!result) {
		IZ_ASSERT(result);
		delete ret;
		ret = IZ_NULL;
	}

	// もういらない
	SAFE_RELEASE(pTex);
	
	return ret;
}
