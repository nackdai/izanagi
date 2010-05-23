#include "debugutil/DebugFont.h"
#include "unGraph.h"

using namespace uranus;

// フォントイメージデータ
static const UN_BYTE FontImageData[] = {
#include "font.dat"
};

// インスタンス作成
CDebugFont* CDebugFont::CreateDebugFont(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	UN_ASSERT(pDevice != UN_NULL);
	UN_ASSERT(pAllocator != UN_NULL);

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CDebugFont* pInstance = UN_NULL;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CDebugFont));
	result = (pBuf != UN_NULL);
	VGOTO(result, __EXIT__);

	// インスタンス作成
	pInstance = new (pBuf)CDebugFont;
	result = (pInstance != UN_NULL);
	VGOTO(result, __EXIT__);

	{
		pInstance->AddRef();

		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		// テクスチャ作成
		result = pInstance->CreateTexture();
		VGOTO(result, __EXIT__);
	}

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
		pAllocator->Free(pBuf);
	}

	return pInstance;
}

// コンストラクタ
CDebugFont::CDebugFont()
{
	m_pAllocator = UN_NULL;
	m_pDevice = UN_NULL;

	m_pFontTex = UN_NULL;

	m_nLeft = 0;
	m_nTop = 0;
	m_nColor = 0xffffffff;

	m_nLeftBase = 0;

	m_bIsBegin = UN_FALSE;
}

// デストラクタ
CDebugFont::~CDebugFont()
{
	SAFE_RELEASE(m_pFontTex);
	SAFE_RELEASE(m_pDevice);
}

// 解放
void CDebugFont::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

/**
* 描画開始
*/
void CDebugFont::Begin()
{
	UN_ASSERT(m_pDevice != UN_NULL);
	UN_ASSERT(m_pFontTex != UN_NULL);

	m_pDevice->SetTexture(0, m_pFontTex);
	m_bIsBegin = UN_TRUE;

	m_nLeft = 0;
	m_nTop = 0;
	m_nColor = 0xffffffff;

	m_nLeftBase = 0;
}

/**
* 描画終了
*/
void CDebugFont::End()
{
	m_bIsBegin = UN_FALSE;
}

/////////////////////////////////////////////////

// テクスチャ作成
UN_BOOL CDebugFont::CreateTexture()
{
	UN_ASSERT(m_pDevice != UN_NULL);
	UN_ASSERT(m_pFontTex == UN_NULL);

	// テクスチャ本体作成
	m_pFontTex = m_pDevice->CreateTexture(
					IMAGE_WIDTH,
					IMAGE_HEIGHT,
					0xffffffff,
					E_GRAPH_PIXEL_FMT_RGBA8,
					E_GRAPH_RSC_CREATE_TYPE_STATIC);
	VRETURN(m_pFontTex != UN_NULL);
	
	// データセット
	UN_BYTE* data = UN_NULL;
	UN_UINT nPitch = m_pFontTex->Lock(
						0, 
						reinterpret_cast<void**>(&data),
						UN_FALSE);
	VRETURN(nPitch > 0);

	size_t nSize = nPitch * IMAGE_HEIGHT;
	VRETURN(nSize == sizeof(FontImageData));

	memcpy(data, FontImageData, nSize);

	m_pFontTex->Unlock(0);
	
	return UN_TRUE;
}

// 文字描画
void CDebugFont::DrawFont(const UN_CHAR* str)
{
	UN_ASSERT(m_pDevice != NULL);
	UN_ASSERT(m_pFontTex != NULL);
	UN_ASSERT(m_bIsBegin);

	m_pDevice->SetTexture(0, m_pFontTex);

	CIntRect rcSrc;

	CIntRect rcDst;
	{
		rcDst.left = m_nLeft;
		rcDst.top = m_nTop;
		rcDst.right = rcDst.left + FONT_SIZE;
		rcDst.bottom = rcDst.top + FONT_SIZE;
	}

	UN_UINT nLength = (UN_UINT)strlen(str);
	
	for (UN_UINT i = 0; i < nLength; i++) {
		UN_CHAR ch = str[i];

		if (ch == '\n') {
			// 改行
			rcDst.OffsetRect(0, FONT_SIZE);
			m_nTop += FONT_SIZE;

			m_nLeft = m_nLeftBase;
		}
		else {
			SetTexRect(ch, rcSrc);

			// 描画
			m_pDevice->Draw2DSpriteEx(
				rcSrc, rcDst,
				m_nColor);

			rcDst.OffsetRect(FONT_SIZE, 0);

			m_nLeft += FONT_SIZE;
		}
	}
}

static const UN_INT BUF_SIZE = 1024;
static UN_CHAR BUF[BUF_SIZE];

/**
* 描画
*/
void CDebugFont::DBPrint(
	const UN_CHAR* str, ...)
{
	FILL_ZERO(BUF, sizeof(BUF));

	va_list ap;	// 引数リスト

	va_start(ap, str);

	vsprintf_s(BUF, sizeof(BUF), str, ap);

	va_end(ap);

	DrawFont(BUF);
}

/**
* 描画
*/
void CDebugFont::DBPrint(
	UN_DWORD color,
	const UN_CHAR* str, ...)
{
	FILL_ZERO(BUF, sizeof(BUF));

	va_list ap;	// 引数リスト

	va_start(ap, str);

	vsprintf_s(BUF, sizeof(BUF), str, ap);

	va_end(ap);

#if 0
	// 元の値を覚えておく
	UN_DWORD buf_color = m_nColor;
#endif

	SetFontColor(color);
	DrawFont(BUF);

#if 0
	// 元に戻す
	SetFontColor(buf_color);
#endif
}

/**
* 描画
*/
void CDebugFont::DBPrint(
	UN_INT left, UN_INT top,
	const UN_CHAR* str, ...)
{
	FILL_ZERO(BUF, sizeof(BUF));

	va_list ap;	// 引数リスト

	va_start(ap, str);

	vsprintf_s(BUF, sizeof(BUF), str, ap);

	va_end(ap);

#if 0
	// 元の値を覚えておく
	UN_INT buf_left = m_nLeft;
	UN_INT buf_top = m_nTop;
#endif

	SetFontPos(left, top);
	DrawFont(BUF);

#if 0
	// 元に戻す
	SetFontPos(buf_left, buf_top);
#endif
}

/**
* 描画
*/
void CDebugFont::DBPrint(
	UN_INT left, UN_INT top,
	UN_DWORD color,
	const UN_CHAR* str, ...)
{
	FILL_ZERO(BUF, sizeof(BUF));

	va_list ap;	// 引数リスト

	va_start(ap, str);

	vsprintf_s(BUF, sizeof(BUF), str, ap);

	va_end(ap);

#if 0
	// 元の値を覚えておく
	UN_INT buf_left = m_nLeft;
	UN_INT buf_top = m_nTop;
	UN_DWORD buf_color = m_nColor;
#endif

	SetFontPos(left, top);
	SetFontColor(color);
	DrawFont(BUF);

#if 0
	// 元に戻す
	SetFontPos(buf_left, buf_top);
	SetFontColor(buf_color);
#endif
}