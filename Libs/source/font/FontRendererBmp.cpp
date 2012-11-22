#include "font/FontRendererBmp.h"
#include "izIo.h"
#include "std/StdUtf.h"
#include "izGraph.h"

using namespace izanagi;

// 登録情報
struct CFontRendererBmp::SRegInfo {
	const S_FNT_MAP* map;

	IZ_INT16 x;			// 描き込み位置 X
	IZ_INT16 y;			// 描き込み位置 Y

	struct {
		IZ_UINT isRenderTex	: 1;	// 描き込み済みか
	};

	CFntHashItem item;

	void Init()
	{
		IZ_ASSERT(map != IZ_NULL);
		item.Init(map->code, this);
	}
	CFntHashItem* GetItem() { return &item; }
};


/**
* インスタンス作成
*/
CFontRenderer* CFontRendererBmp::CreateFontRendererBmp(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IZ_UINT nMaxRegisterNum,
	IInputStream* in)
{
	VRETURN_VAL(nMaxRegisterNum <= MAX_FONT_NUM, IZ_NULL);

	CFontRendererBmp* pInstance = NULL;
	IZ_UINT8* pBufTop = IZ_NULL;

	// ヘッダ読み込み
	S_FNT_HEADER sHeader;
	IZ_BOOL result = IZ_INPUT_READ(in, &sHeader, 0, sizeof(sHeader));
	VGOTO(result, __EXIT__);

	// マジックナンバーチェック
	result = IS_FMT_ENCODE(sHeader.magic);
	VGOTO(result, __EXIT__);

	// 確保サイズ計算
	IZ_UINT nSize = sizeof(CFontRendererBmp);
	{
		nSize += sHeader.sizeFile - sHeader.sizeHeader;
		nSize += sizeof(SRegInfo) * nMaxRegisterNum;
	}

	// メモリ確保
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	result = (pBuf != IZ_NULL);
	VGOTO(result, __EXIT__);

	// 先頭位置を覚えておく
	pBufTop = pBuf;

	// インスタンス作成
	pInstance = new(pBuf) CFontRendererBmp;
	{
		pBuf += sizeof(CFontRendererBmp);

		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_nMaxRegisterNum = nMaxRegisterNum;

		// ヘッダ
		memcpy(&pInstance->m_sHeader, &sHeader, sizeof(sHeader));
	}

	// メモリ割り当て
	{
		// フォントマップ情報
		pInstance->m_pMapList = reinterpret_cast<S_FNT_MAP*>(pBuf);
		pBuf += sizeof(S_FNT_MAP) * sHeader.numFont;

		// フォントイメージ情報
		pInstance->m_pImageList = reinterpret_cast<S_FNT_IMAGE*>(pBuf);
		pBuf += sizeof(S_FNT_IMAGE) * sHeader.numTex;

		// NOTE
		// フォントイメージデータは、情報部を読み込まないとサイズが確定しないので、
		// 後で割り当てる

		// 登録情報
		pInstance->m_pRegList = reinterpret_cast<SRegInfo*>(pBuf);
		pBuf += sizeof(SRegInfo) * nMaxRegisterNum;
	}

	// フォントマップ情報読み込み
	result = IZ_INPUT_READ(
				in, 
				pInstance->m_pMapList,
				0,
				sizeof(S_FNT_MAP) * sHeader.numFont);
	VGOTO(result, __EXIT__);

	// フォントイメージ情報読み込み
	for (IZ_UINT i = 0; i < sHeader.numTex; ++i) {
		// 情報部読み込み
		result = IZ_INPUT_READ(
					in,
					&pInstance->m_pImageList[i],
					0,
					sizeof(S_FNT_IMAGE));
		VGOTO(result, __EXIT__);

		pInstance->m_pImageList[i].images = reinterpret_cast<const IZ_UINT8*>(pBuf);
		pBuf += pInstance->m_pImageList[i].sizeData;

		IZ_UINT8* pReadBuf = const_cast<IZ_UINT8*>(pInstance->m_pImageList[i].images);

		// データ部読み込み
		result = IZ_INPUT_READ(
					in,
					reinterpret_cast<void*>(pReadBuf),
					0,
					pInstance->m_pImageList[i].sizeData);
		VGOTO(result, __EXIT__);
	}

	// 念のため
	IZ_ASSERT((IZ_UINT)(pBuf - pBufTop) == nSize);

	// フォントテクスチャ作成
	result = pInstance->CreateTexture(in);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBufTop != IZ_NULL) {
			pAllocator->Free(pBufTop);
		}
	}
	return pInstance;
}

// コンストラクタ
CFontRendererBmp::CFontRendererBmp()
{
	m_Allocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	// ヘッダ
	CStdUtil::FillZero(&m_sHeader, 1);

	// フォントマップ情報
	m_pMapList = IZ_NULL;

	// フォントイメージ情報
	m_pImageList = IZ_NULL;

	// フォントテクスチャ
	m_pSrcTex = IZ_NULL;
	m_pDstTex = IZ_NULL;

	// イメージ情報
	CStdUtil::FillZero(&m_sImageInfo, 1);

	// 最大登録可能文字数
	m_nMaxRegisterNum = 0;

	// 登録数
	m_nRegNum = 0;

	// 登録情報
	m_pRegList = IZ_NULL;

	m_Flags.isNeedUpdateSurface = IZ_TRUE;
}

// デストラクタ
CFontRendererBmp::~CFontRendererBmp()
{
	SAFE_RELEASE(m_pSrcTex);
	SAFE_RELEASE(m_pDstTex);

	SAFE_RELEASE(m_pDevice);
}

// 開放
void CFontRendererBmp::InternalRelease()
{
	delete this;

	if (m_Allocator != IZ_NULL) {
		m_Allocator->Free(this);
	}
}

// テクスチャ作成
IZ_BOOL CFontRendererBmp::CreateTexture(IInputStream* in)
{
	IZ_BOOL ret = IZ_TRUE;

	// 縦横のフォント数
	IZ_UINT nFontNumH = static_cast<IZ_UINT>(sqrtf((IZ_FLOAT)m_nMaxRegisterNum) + 0.5f);
	IZ_UINT nFontNumV = static_cast<IZ_UINT>(static_cast<IZ_FLOAT>(m_nMaxRegisterNum) / nFontNumH + 0.5f);
	IZ_ASSERT(nFontNumH * nFontNumV >= m_nMaxRegisterNum);

	// テクスチャサイズ
	IZ_UINT nWidth = nFontNumH * m_sHeader.fontHeight;
	IZ_UINT nHeight = nFontNumV * m_sHeader.fontHeight;

	nWidth += (nFontNumH + 1) * CHAR_MARGIN;
	nHeight += (nFontNumV + 1) * CHAR_MARGIN;

#if 0
	nWidth = ComputeNextPow2(nWidth);
	nHeight = ComputeNextPow2(nHeight);
#endif

	// ソース用テクスチャ作成
	m_pSrcTex = m_pDevice->CreateTextureOnSysMem(
					nWidth,
					nHeight,
					TEX_FORMAT,
					1);
	ret = (m_pSrcTex != NULL);
	VGOTO(ret, __EXIT__);

	// フォント描画用テクスチャ作成
	// NOTE
	// UpdateSurfaceするために D3DPOOL_DEFAULT でないといけないので
	// DYNAMIC で作成する
	m_pDstTex = m_pDevice->CreateTexture(
					nWidth,
					nHeight,
					1,
					TEX_FORMAT,
					graph::E_GRAPH_RSC_TYPE_STATIC_DYNAMIC);
	ret = (m_pDstTex != NULL);
	VGOTO(ret, __EXIT__);

__EXIT__:
	return ret;
}

namespace {
	inline void* _GetOneCharCode(
		void* src,
		IZ_UINT *ret,
		izanagi::E_FONT_CHAR_ENCODE encode)
	{
		IZ_ASSERT(src != IZ_NULL);
		IZ_ASSERT(ret != IZ_NULL);

		*ret = 0;

		switch (encode) {
		case E_FONT_CHAR_ENCODE_SJIS:
			src = CStdUtf::GetOneCharCodeAsSJIS(src, ret);
			break;
		case E_FONT_CHAR_ENCODE_UTF16:
			src = CStdUtf::GetOneCharCodeAsUTF16(src, ret);
			break;
		case E_FONT_CHAR_ENCODE_UTF8:
			src = CStdUtf::GetOneCharCodeAsUTF8(src, ret);
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		return src;
	}
}	// namespace

/**
* 登録
* 存在しない文字があったときにFALSEを返す
*/
IZ_BOOL CFontRendererBmp::Register(const void* pStr)
{
	if (!BeginRegister()) {
		// 登録開始処理失敗
		return IZ_FALSE;
	}

	IZ_BOOL ret = IZ_TRUE;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	IZ_UINT nCode = 0;

	void* str = const_cast<void*>(pStr);

	for (;;) {
		str = _GetOneCharCode(
				str,
				&nCode,
				encode);

		if (nCode == 0) {
			break;
		}
		else if (CStdUtf::IsSpace(nCode)
				|| CStdUtf::IsLineFeed(nCode))
		{
			continue;
		}

		ret = (RegisterInternal(nCode) != IZ_NULL);
		IZ_ASSERT(ret);

		if (!ret) {
			break;
		}
	}


	EndRegister();

	return ret;
}

/**
* 描画
* 登録されていない場合は、描画しない
*/
void CFontRendererBmp::Render(
	const void* pStr,
	IZ_INT nX, IZ_INT nY,
	IZ_COLOR nColor/*= IZ_COLOR_RGB(255, 255, 255, 255)*/)
{
	// 転送
	if (m_Flags.isNeedUpdateSurface) {
		UpdateSurface();
		m_Flags.isNeedUpdateSurface = IZ_FALSE;
	}

	// 描画モード変更
	graph::E_GRAPH_2D_RENDER_OP nPrevOp = m_pDevice->Get2DRenderOp();
	m_pDevice->Set2DRenderOp(RENDER_2D_OP);

	// テクスチャセット
	m_pDevice->SetTexture(0, m_pDstTex);

	// 描画位置
	CIntPoint sDstPoint(nX, nY);

	// NOTE
	// LineFeed types are below.
	// - CR + LF
	// - CR
	// - LF

	// If this flag is TRUE, line feed occurs.
	IZ_BOOL bIsLineFeed = IZ_FALSE;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	IZ_UINT nCode = 0;

	void* str = const_cast<void*>(pStr);

	for (;;) {
		str = _GetOneCharCode(
				str,
				&nCode,
				encode);

		if (nCode == 0) {
			break;
		}
		else if (CStdUtf::IsSpace(nCode)) {
			sDstPoint.x += m_sHeader.fontHeight;
			continue;
		}
		else if (CStdUtf::IsLineFeed(nCode)) {
			// CR or LF
			bIsLineFeed = IZ_TRUE;
			continue;
		}

		if (bIsLineFeed) {
			// 改行
			sDstPoint.x = nX;
			sDstPoint.y += m_sHeader.fontHeight;

			bIsLineFeed = IZ_FALSE;
		}

		// 描画
		// 内部で次の描画位置も計算する
		RenderInternal(
			nCode,
			&sDstPoint,
			nColor);
	}

	// 描画モードを元に戻す
	m_pDevice->Set2DRenderOp(nPrevOp);
}

/**
* 描画
* 登録されていない場合は、登録を行う
*/
IZ_BOOL CFontRendererBmp::RenderImmediate(
	void* pStr,
	IZ_INT nX, IZ_INT nY,
	IZ_COLOR nColor/*= IZ_COLOR_RGB(255, 255, 255, 255)*/)
{
	// 登録する
	IZ_BOOL ret = Register(pStr);

	// 描画する
	Render(
		pStr, 
		nX, nY, 
		nColor);

	return ret;
}

// フォント登録
CFontRendererBmp::SRegInfo* CFontRendererBmp::RegisterInternal(IZ_UINT code)
{
	// マップ情報取得
	const S_FNT_MAP* pMap = GetFontMap(code);
	if (pMap == NULL) {
		return NULL;
	}

	// 登録情報を取得
	SRegInfo* pRegInfo = GetRegInfo(code);
	if (pRegInfo != IZ_NULL) {
		// 登録済み
		
		// ハッシュリストに再登録する
		pRegInfo->GetItem()->Leave();
		m_FontHash.Add(pRegInfo->GetItem());

		return pRegInfo;
	}

	// 空いている登録情報を取得する
	if (m_nRegNum == m_nMaxRegisterNum) {
		// NOTE
		// 古いものほどリストの先頭にくる
		CFntHashItem* pListItem = m_FontHash.GetOrderTopHashItem();
		IZ_ASSERT(pListItem != IZ_NULL);

		// リストから外す
		pListItem->Leave();

		pRegInfo = pListItem->GetData();
	}
	else {
		pRegInfo = &m_pRegList[m_nRegNum];
		pRegInfo->x = -1;
		pRegInfo->y = -1;
		++m_nRegNum;
	}

	// 登録
	pRegInfo->map = pMap;
	pRegInfo->isRenderTex = IZ_FALSE;

	pRegInfo->Init();
	m_FontHash.Add(pRegInfo->GetItem());

	// 新規に登録したので、転送元テクスチャに描き込む
	RenderToSrcTex(
		pRegInfo,
		code,
		IZ_FALSE);

	return pRegInfo;
}

// フォント登録開始
IZ_BOOL CFontRendererBmp::BeginRegister()
{
	m_sImageInfo.pitch = m_pSrcTex->Lock(
							0,
							reinterpret_cast<void**>(&m_sImageInfo.data),
							IZ_TRUE);

	IZ_BOOL ret = (m_sImageInfo.pitch > 0);
	IZ_ASSERT(ret);

	return ret;
}

// フォント登録終了
void CFontRendererBmp::EndRegister()
{
	m_pSrcTex->Unlock(0);

	m_sImageInfo.data = NULL;
	m_sImageInfo.pitch = 0;
}

// 登録情報取得
CFontRendererBmp::SRegInfo* CFontRendererBmp::GetRegInfo(IZ_UINT code)
{
	SRegInfo* ret = IZ_NULL;

#if 0
	for (IZ_UINT i = 0; i < m_nRegNum; ++i) {
		if (m_pRegList[i].map->code == code) {
			ret = &m_pRegList[i];
			break;
		};
	}
#else
	ret = m_FontHash.FindData(code);
#endif

	return ret;
}

// マップ情報取得
const S_FNT_MAP* CFontRendererBmp::GetFontMap(IZ_UINT code)
{
	S_FNT_MAP* ret = IZ_NULL;

	// 二分探索する
	IZ_INT idxTop = m_pMapList[0].idx;
	IZ_INT idxTail = m_pMapList[m_sHeader.numFont - 1].idx;

	while (idxTop <= idxTail) {
		IZ_UINT idxMid = ((idxTop + idxTail) >> 1);
		S_FNT_MAP* pMid = &m_pMapList[idxMid];

		if (pMid->code < code) {
			idxTop = idxMid + 1;
			VRETURN_NULL(idxTop < m_sHeader.numFont);
		}
		else if (pMid->code > code) {
			idxTail = idxMid - 1;
			VRETURN_NULL(idxTail >= 0);
		}
		else {
			// あった
			ret = pMid;
			break;
		}
	}

	if (ret == IZ_NULL) {
		// 線形探索してみる
		for (IZ_UINT i = 0; i < m_sHeader.numFont; i++) {
			if (m_pMapList[i].code == code) {
				ret = &m_pMapList[i];
				break;
			}
		}
		IZ_ASSERT(ret != IZ_NULL);
	}

	return ret;
}

// 転送元テクスチャにフォント描画
void CFontRendererBmp::RenderToSrcTex(
	SRegInfo* pRegInfo,
	IZ_UINT code,
	IZ_BOOL bRegister)
{
	if (pRegInfo == NULL) {
		if (bRegister) {
			// 登録
			pRegInfo = RegisterInternal(code);
		}
		else {
			// 探す
			pRegInfo = GetRegInfo(code);
		}
	}

	IZ_ASSERT(pRegInfo != IZ_NULL);
	
	if (pRegInfo->isRenderTex) {
		// 既に描き込み済みなので終了
		return;
	}

	if (pRegInfo != IZ_NULL) {
		// イメージ情報取得
		const S_FNT_IMAGE& sFntImage = m_pImageList[pRegInfo->map->texID];

#if 0
		IZ_UINT nDstFontWidth = pRegInfo->map->fontWidth;
#else
		IZ_UINT nDstFontWidth = m_sHeader.fontHeight;
#endif

		// 転送元矩形
		SIntRect sSrcRect;
		{
			sSrcRect.left = pRegInfo->map->srcX;
			sSrcRect.top = pRegInfo->map->srcY;
			sSrcRect.right = sSrcRect.left + pRegInfo->map->fontWidth;
			sSrcRect.bottom = sSrcRect.top + m_sHeader.fontHeight;
		}

		// 転送元
		const IZ_UINT8* pSrc = sFntImage.images;
		IZ_UINT nSrcPitch = sFntImage.pitch;
		IZ_UINT nSrcBpp = (m_sHeader.texFmt == graph::E_GRAPH_PIXEL_FMT_A8 ? 1 : 4);
		IZ_ASSERT(nSrcBpp == 1);

		// 転送先矩形
		SIntRect sDstRect;
		{
			if (pRegInfo->x < 0) {
				IZ_UINT pos = m_sImageInfo.x + nDstFontWidth;
				if (pos > m_pSrcTex->GetWidth()) {
					// テクスチャ幅超えたので、フォント１つ分ずらす
					m_sImageInfo.x = CHAR_MARGIN;
					m_sImageInfo.y += m_sHeader.fontHeight;
				}

				pRegInfo->x = m_sImageInfo.x;
				pRegInfo->y = m_sImageInfo.y;

				m_sImageInfo.x += nDstFontWidth + CHAR_MARGIN;
			}
			
			sDstRect.left = pRegInfo->x;
			sDstRect.top = pRegInfo->y;

			sDstRect.right = sDstRect.left + nDstFontWidth;
			sDstRect.bottom = sDstRect.top + m_sHeader.fontHeight;
		}

		// 転送先
		IZ_UINT8* pDst = m_sImageInfo.data;
		IZ_UINT nDstPitch = m_sImageInfo.pitch;
		IZ_ASSERT(pDst != NULL);

		// コピー
		for (IZ_UINT y = 0; y < m_sHeader.fontHeight; ++y) {
			const IZ_UINT8* src = pSrc + nSrcPitch * (y + sSrcRect.top) + nSrcBpp * sSrcRect.left;
			IZ_UINT8* dst = pDst + nDstPitch * (y + sDstRect.top) + TEX_BPP * sDstRect.left;

			for (IZ_UINT x = 0; x < pRegInfo->map->fontWidth; ++x) {
				IZ_UINT8 alpha = *src;

#if 0
				*(dst + 0) = 0xff;
				*(dst + 1) = 0xff;
				*(dst + 2) = 0xff;
				*(dst + 3) = alpha;
#else
				*(dst) = alpha;
#endif

				src += nSrcBpp;
				dst += TEX_BPP;
			}
		}
	}

	// 描き込み済みにする
	pRegInfo->isRenderTex = IZ_TRUE;

	// 転送が必要になる
	m_Flags.isNeedUpdateSurface = IZ_TRUE;
}

// 転送
IZ_BOOL CFontRendererBmp::UpdateSurface()
{
	IZ_ASSERT(m_pDevice != NULL);

	// TODO
    D3D_DEVICE* pD3DDev = (D3D_DEVICE*)m_pDevice->GetPlatformInterface();
    D3D_SURFACE* src = (D3D_SURFACE*)m_pSrcTex->GetSurface(0)->GetPlatformInterface();
    D3D_SURFACE* dst = (D3D_SURFACE*)m_pDstTex->GetSurface(0)->GetPlatformInterface();

	HRESULT hr = pD3DDev->UpdateSurface(
					src,
					NULL,
					dst,
					NULL);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

// フォント描画
void CFontRendererBmp::RenderInternal(
	IZ_UINT code,
	CIntPoint* pDstPoint,
	IZ_COLOR nColor)
{
	SRegInfo* pRegInfo = GetRegInfo(code);

	if (pRegInfo == NULL) {
		// TODO
		// 存在しないのでとばす
		pDstPoint->x += m_sHeader.fontHeight;
	}
	else {
		SIntRect sSrcRect;
		{
			sSrcRect.left = pRegInfo->x;
			sSrcRect.top = pRegInfo->y;

			sSrcRect.right = sSrcRect.left + pRegInfo->map->fontWidth;
			sSrcRect.bottom = sSrcRect.top + m_sHeader.fontHeight;
		}

		SIntRect sDstRect;
		{
#if 0
			sDstRect.left = pDstPoint->x;
#else
			sDstRect.left = pDstPoint->x;
			if (m_sHeader.isFixedPitch) {
				sDstRect.left += (m_sHeader.fontHeight - pRegInfo->map->fontWidth) >> 1;
			}
#endif
			sDstRect.top = pDstPoint->y;
			sDstRect.right = sDstRect.left + pRegInfo->map->fontWidth;
			sDstRect.bottom = sDstRect.top + m_sHeader.fontHeight;
		}

		// 描画
		if (m_pRenderFontHadler != IZ_NULL) {
			IZ_UINT nW = (*m_pRenderFontHadler)(
							m_pDevice,
							pRegInfo->map->code,
							sSrcRect,
							pDstPoint->x,
							pDstPoint->y,
							nColor);
			pDstPoint->x += nW;
		}
		else {
			m_pDevice->Draw2DSpriteEx(sSrcRect, sDstRect, nColor);
#if 0
			pDstPoint->x += pRegInfo->map->fontWidth;
#else
			if (m_sHeader.isFixedPitch) {
				pDstPoint->x += m_sHeader.fontHeight;
			}
			else {
				pDstPoint->x += pRegInfo->map->fontWidth;
			}
#endif
		}
	}
}

/**
* ピクセル幅取得
*/
IZ_UINT CFontRendererBmp::GetWidth(void* pStr)
{
	// NOTE
	// 複数行あるときは一番長い行について返す

	IZ_UINT nWidth = 0;
	IZ_UINT nMaxWidth = 0;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	IZ_UINT nCode = 0;

	IZ_BOOL bIsLineFeed = IZ_FALSE;

	for (;;) {
		pStr = _GetOneCharCode(
				pStr,
				&nCode,
				encode);

		if (nCode == 0) {
			break;
		}
		else if (CStdUtf::IsAscii(nCode)) {
			if ((nCode == 0x0d) || (nCode == 0x0a)) {
				// CR or LF
				bIsLineFeed = IZ_TRUE;
				continue;
			}
		}

		if (bIsLineFeed) {
			nMaxWidth = (nWidth > nMaxWidth ? nWidth : nMaxWidth);
			bIsLineFeed = IZ_FALSE;
			continue;
		}

		const S_FNT_MAP* pMap = GetFontMap(nCode);
		IZ_ASSERT(pMap != IZ_NULL);

		if (pMap != IZ_NULL) {
			nWidth += pMap->fontWidth;
		}
	}

	nMaxWidth = (nWidth > nMaxWidth ? nWidth : nMaxWidth);

	return nMaxWidth;
}

/**
* 登録済みかどうか
* 最初の１文字のみ調べる
*/
IZ_BOOL CFontRendererBmp::IsRegistered(IZ_UINT code)
{
	// 登録情報探す
	SRegInfo* pRegInfo = GetRegInfo(code);
	return (pRegInfo != IZ_NULL);
}
