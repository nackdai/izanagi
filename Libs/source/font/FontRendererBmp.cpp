#include "font/FontRendererBmp.h"
#include "unIo.h"
#include "std/StdUtf.h"
#include "unGraph.h"

using namespace uranus;

// 登録情報
struct CFontRendererBmp::SRegInfo {
	const S_FNT_MAP* map;

	UN_INT16 x;			// 描き込み位置 X
	UN_INT16 y;			// 描き込み位置 Y

	struct {
		UN_UINT isRenderTex	: 1;	// 描き込み済みか
	};

	CFntHashItem item;

	void Init()
	{
		UN_ASSERT(map != UN_NULL);
		item.Init(map->code, this);
	}
	CFntHashItem* GetItem() { return &item; }
};


/**
* インスタンス作成
*/
CFontRenderer* CFontRendererBmp::CreateFontRendererBmp(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT nMaxRegisterNum,
	IInputStream* in)
{
	VRETURN_VAL(nMaxRegisterNum <= MAX_FONT_NUM, UN_NULL);

	CFontRendererBmp* pInstance = NULL;
	UN_UINT8* pBufTop = UN_NULL;

	// ヘッダ読み込み
	S_FNT_HEADER sHeader;
	UN_BOOL result = UN_INPUT_READ(in, &sHeader, 0, sizeof(sHeader));
	VGOTO(result, __EXIT__);

	// マジックナンバーチェック
	result = IS_FMT_ENCODE(sHeader.magic);
	VGOTO(result, __EXIT__);

	// 確保サイズ計算
	UN_UINT nSize = sizeof(CFontRendererBmp);
	{
		nSize += sHeader.sizeFile - sHeader.sizeHeader;
		nSize += sizeof(SRegInfo) * nMaxRegisterNum;
	}

	// メモリ確保
	UN_UINT8* pBuf = reinterpret_cast<UN_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	result = (pBuf != UN_NULL);
	VGOTO(result, __EXIT__);

	// 先頭位置を覚えておく
	pBufTop = pBuf;

	// インスタンス作成
	pInstance = new(pBuf) CFontRendererBmp;
	{
		pBuf += sizeof(CFontRendererBmp);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
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
	result = UN_INPUT_READ(
				in, 
				pInstance->m_pMapList,
				0,
				sizeof(S_FNT_MAP) * sHeader.numFont);
	VGOTO(result, __EXIT__);

	// フォントイメージ情報読み込み
	for (UN_UINT i = 0; i < sHeader.numTex; i++) {
		// 情報部読み込み
		result = UN_INPUT_READ(
					in,
					&pInstance->m_pImageList[i],
					0,
					sizeof(S_FNT_IMAGE));
		VGOTO(result, __EXIT__);

		pInstance->m_pImageList[i].images = reinterpret_cast<const UN_UINT8*>(pBuf);
		pBuf += pInstance->m_pImageList[i].sizeData;

		UN_UINT8* pReadBuf = const_cast<UN_UINT8*>(pInstance->m_pImageList[i].images);

		// データ部読み込み
		result = UN_INPUT_READ(
					in,
					reinterpret_cast<void*>(pReadBuf),
					0,
					pInstance->m_pImageList[i].sizeData);
		VGOTO(result, __EXIT__);
	}

	// 念のため
	UN_ASSERT((UN_UINT)(pBuf - pBufTop) == nSize);

	// フォントテクスチャ作成
	result = pInstance->CreateTexture(in);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBufTop != UN_NULL) {
			pAllocator->Free(pBufTop);
		}
	}
	return pInstance;
}

// コンストラクタ
CFontRendererBmp::CFontRendererBmp()
{
	m_pAllocator = UN_NULL;
	m_pDevice = UN_NULL;

	// ヘッダ
	CStdUtil::FillZero(&m_sHeader, 1);

	// フォントマップ情報
	m_pMapList = UN_NULL;

	// フォントイメージ情報
	m_pImageList = UN_NULL;

	// フォントテクスチャ
	m_pSrcTex = UN_NULL;
	m_pDstTex = UN_NULL;

	// イメージ情報
	CStdUtil::FillZero(&m_sImageInfo, 1);

	// 最大登録可能文字数
	m_nMaxRegisterNum = 0;

	// 登録数
	m_nRegNum = 0;

	// 登録情報
	m_pRegList = UN_NULL;

	m_Flags.isNeedUpdateSurface = UN_TRUE;
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

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

// テクスチャ作成
UN_BOOL CFontRendererBmp::CreateTexture(IInputStream* in)
{
	UN_BOOL ret = UN_TRUE;

	// 縦横のフォント数
	UN_UINT nFontNumH = static_cast<UN_UINT>(sqrtf((UN_FLOAT)m_nMaxRegisterNum) + 0.5f);
	UN_UINT nFontNumV = static_cast<UN_UINT>(static_cast<UN_FLOAT>(m_nMaxRegisterNum) / nFontNumH + 0.5f);
	UN_ASSERT(nFontNumH * nFontNumV >= m_nMaxRegisterNum);

	// テクスチャサイズ
	UN_UINT nWidth = nFontNumH * m_sHeader.fontHeight;
	UN_UINT nHeight = nFontNumV * m_sHeader.fontHeight;

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
					E_GRAPH_RSC_CREATE_TYPE_STATIC_DYNAMIC);
	ret = (m_pDstTex != NULL);
	VGOTO(ret, __EXIT__);

__EXIT__:
	return ret;
}

namespace {
	inline void* _GetOneCharCode(
		void* src,
		UN_UINT *ret,
		uranus::E_FONT_CHAR_ENCODE encode)
	{
		UN_ASSERT(src != UN_NULL);
		UN_ASSERT(ret != UN_NULL);

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
			UN_ASSERT(UN_FALSE);
			break;
		}

		return src;
	}
}	// namespace

/**
* 登録
* 存在しない文字があったときにFALSEを返す
*/
UN_BOOL CFontRendererBmp::Register(void* pStr)
{
	if (!BeginRegister()) {
		// 登録開始処理失敗
		return UN_FALSE;
	}

	UN_BOOL ret = UN_TRUE;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	UN_UINT nCode = 0;

	for (;;) {
		pStr = _GetOneCharCode(
				pStr,
				&nCode,
				encode);

		if (nCode == 0) {
			break;
		}

		ret = (RegisterInternal(nCode) != UN_NULL);
		UN_ASSERT(ret);

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
	void* pStr,
	UN_INT nX, UN_INT nY,
	UN_COLOR nColor/*= UN_COLOR_RGB(255, 255, 255, 255)*/)
{
	// 転送
	if (m_Flags.isNeedUpdateSurface) {
		UpdateSurface();
		m_Flags.isNeedUpdateSurface = UN_FALSE;
	}

	// 描画モード変更
	E_GRAPH_2D_RENDER_OP nPrevOp = m_pDevice->Get2DRenderOp();
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
	UN_BOOL bIsLineFeed = UN_FALSE;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	UN_UINT nCode = 0;

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
				bIsLineFeed = UN_TRUE;
				continue;
			}
		}

		if (bIsLineFeed) {
			// 改行
			sDstPoint.x = nX;
			sDstPoint.y += m_sHeader.fontHeight;

			bIsLineFeed = UN_FALSE;
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
UN_BOOL CFontRendererBmp::RenderImmediate(
	void* pStr,
	UN_INT nX, UN_INT nY,
	UN_COLOR nColor/*= UN_COLOR_RGB(255, 255, 255, 255)*/)
{
	// 登録する
	UN_BOOL ret = Register(pStr);

	// 描画する
	Render(
		pStr, 
		nX, nY, 
		nColor);

	return ret;
}

// フォント登録
CFontRendererBmp::SRegInfo* CFontRendererBmp::RegisterInternal(UN_UINT code)
{
	// マップ情報取得
	const S_FNT_MAP* pMap = GetFontMap(code);
	if (pMap == NULL) {
		return NULL;
	}

	// 登録情報を取得
	SRegInfo* pRegInfo = GetRegInfo(code);
	if (pRegInfo != UN_NULL) {
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
		CFntHashItem* pListItem = m_FontHash.GetOrderTop();
		UN_ASSERT(pListItem != UN_NULL);

		// リストから外す
		pListItem->Leave();

		pRegInfo = pListItem->GetData();
	}
	else {
		pRegInfo = &m_pRegList[m_nRegNum];
		pRegInfo->x = -1;
		pRegInfo->y = -1;
		m_nRegNum++;
	}

	// 登録
	pRegInfo->map = pMap;
	pRegInfo->isRenderTex = UN_FALSE;

	pRegInfo->Init();
	m_FontHash.Add(pRegInfo->GetItem());

	// 新規に登録したので、転送元テクスチャに描き込む
	RenderToSrcTex(
		pRegInfo,
		code,
		UN_FALSE);

	return pRegInfo;
}

// フォント登録開始
UN_BOOL CFontRendererBmp::BeginRegister()
{
	m_sImageInfo.pitch = m_pSrcTex->Lock(
							0,
							reinterpret_cast<void**>(&m_sImageInfo.data),
							UN_TRUE);

	UN_BOOL ret = (m_sImageInfo.pitch > 0);
	UN_ASSERT(ret);

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
CFontRendererBmp::SRegInfo* CFontRendererBmp::GetRegInfo(UN_UINT code)
{
	SRegInfo* ret = UN_NULL;

#if 0
	for (UN_UINT i = 0; i < m_nRegNum; i++) {
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
const S_FNT_MAP* CFontRendererBmp::GetFontMap(UN_UINT code)
{
	S_FNT_MAP* ret = UN_NULL;

	// 二分探索する
	S_FNT_MAP* pTop = &m_pMapList[0];
	S_FNT_MAP* pTail = &m_pMapList[m_sHeader.numFont - 1];

	while (pTop <= pTail) {
		S_FNT_MAP* pMid = m_pMapList + ((pTail->idx + pTop->idx) >> 1);

		if (pMid->code < code) {
			pTop = pMid + 1;
		}
		else if (pMid->code > code) {
			pTail = pMid - 1;
		}
		else {
			// あった
			ret = pMid;
			break;
		}
	}

	return ret;
}

// 転送元テクスチャにフォント描画
void CFontRendererBmp::RenderToSrcTex(
	SRegInfo* pRegInfo,
	UN_UINT code,
	UN_BOOL bRegister)
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

	UN_ASSERT(pRegInfo != UN_NULL);
	
	if (pRegInfo->isRenderTex) {
		// 既に描き込み済みなので終了
		return;
	}

	if (pRegInfo != UN_NULL) {
		// イメージ情報取得
		const S_FNT_IMAGE& sFntImage = m_pImageList[pRegInfo->map->texID];

#if 0
		UN_UINT nDstFontWidth = pRegInfo->map->fontWidth;
#else
		UN_UINT nDstFontWidth = m_sHeader.fontHeight;
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
		const UN_UINT8* pSrc = sFntImage.images;
		UN_UINT nSrcPitch = sFntImage.pitch;
		UN_UINT nSrcBpp = (m_sHeader.texFmt == E_GRAPH_PIXEL_FMT_A8 ? 1 : 4);
		UN_ASSERT(nSrcBpp == 1);

		// 転送先矩形
		SIntRect sDstRect;
		{
			if (pRegInfo->x < 0) {
				UN_UINT pos = m_sImageInfo.x + nDstFontWidth;
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
		UN_UINT8* pDst = m_sImageInfo.data;
		UN_UINT nDstPitch = m_sImageInfo.pitch;
		UN_ASSERT(pDst != NULL);

		// コピー
		for (UN_UINT y = 0; y < m_sHeader.fontHeight; y++) {
			const UN_UINT8* src = pSrc + nSrcPitch * (y + sSrcRect.top) + nSrcBpp * sSrcRect.left;
			UN_UINT8* dst = pDst + nDstPitch * (y + sDstRect.top) + TEX_BPP * sDstRect.left;

			for (UN_UINT x = 0; x < pRegInfo->map->fontWidth; x++) {
				UN_UINT8 alpha = *src;

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
	pRegInfo->isRenderTex = UN_TRUE;

	// 転送が必要になる
	m_Flags.isNeedUpdateSurface = UN_TRUE;
}

// 転送
UN_BOOL CFontRendererBmp::UpdateSurface()
{
	UN_ASSERT(m_pDevice != NULL);

	// TODO
	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	HRESULT hr = pD3DDev->UpdateSurface(
					m_pSrcTex->GetSurface(0)->GetRawInterface(),
					NULL,
					m_pDstTex->GetSurface(0)->GetRawInterface(),
					NULL);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

// フォント描画
void CFontRendererBmp::RenderInternal(
	UN_UINT code,
	CIntPoint* pDstPoint,
	UN_COLOR nColor)
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
			sDstRect.left = pDstPoint->x;
			sDstRect.top = pDstPoint->y;
			sDstRect.right = sDstRect.left + pRegInfo->map->fontWidth;
			sDstRect.bottom = sDstRect.top + m_sHeader.fontHeight;
		}

		// 描画
		if (m_pRenderFontHadler != UN_NULL) {
			UN_UINT nW = (*m_pRenderFontHadler)(
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
			pDstPoint->x += pRegInfo->map->fontWidth;
		}
	}
}

/**
* ピクセル幅取得
*/
UN_UINT CFontRendererBmp::GetWidth(void* pStr)
{
	// NOTE
	// 複数行あるときは一番長い行について返す

	UN_UINT nWidth = 0;
	UN_UINT nMaxWidth = 0;

	const E_FONT_CHAR_ENCODE encode = m_sHeader.charEncode;
	UN_UINT nCode = 0;

	UN_BOOL bIsLineFeed = UN_FALSE;

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
				bIsLineFeed = UN_TRUE;
				continue;
			}
		}

		if (bIsLineFeed) {
			nMaxWidth = (nWidth > nMaxWidth ? nWidth : nMaxWidth);
			bIsLineFeed = UN_FALSE;
			continue;
		}

		const S_FNT_MAP* pMap = GetFontMap(nCode);
		UN_ASSERT(pMap != UN_NULL);

		if (pMap != UN_NULL) {
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
UN_BOOL CFontRendererBmp::IsRegistered(UN_UINT code)
{
	// 登録情報探す
	SRegInfo* pRegInfo = GetRegInfo(code);
	return (pRegInfo != UN_NULL);
}
