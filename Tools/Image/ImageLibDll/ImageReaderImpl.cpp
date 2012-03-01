#include "ImageReaderImpl.h"

namespace izanagi {

static tool::CGraphicsDeviceLite* s_Device = IZ_NULL;

tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd)
{
	// GraphcsDevice作成
	if (s_Device == IZ_NULL) {
		s_Device = izanagi::tool::CGraphicsDeviceLite::CreateGraphicsDeviceLight((HWND)hWnd);
		VRETURN(s_Device != IZ_NULL);
	}
	return s_Device;
}

void ReleaseGraphicsDevice()
{
	SAFE_RELEASE(s_Device);
}

tool::CGraphicsDeviceLite* GetGraphicsDevice()
{
	return s_Device;
}

IZ_BOOL _SetDataAsPlane(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	tool::CTextureLite* pSrcTex,
	tool::CIMGTexture** pDstTex)
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

// 読み込み
tool::CIMGTexture* Read(
	const char* path,
	izanagi::E_GRAPH_TEX_TYPE type)
{
	tool::CGraphicsDeviceLite* device = s_Device;

	// 読み込み
	tool::CTextureLite* pTex = device->CreateTextureFromFile(path);
	VRETURN_NULL(pTex);

	// テクスチャインスタンス作成
	tool::CIMGTexture* ret = new tool::CIMGTexture();
	IZ_ASSERT(ret != IZ_NULL);

	// データセット関数テーブル
	typedef IZ_BOOL (*SetDataFunc)(IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT, tool::CTextureLite*, tool::CIMGTexture**);
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
	IZ_BOOL result = (*func[type])(
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

}	// namespace izanagi
