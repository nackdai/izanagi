#include "debugutil/DebugFont.h"
#include "izGraph.h"

using namespace izanagi;

// フォントイメージデータ
static const IZ_BYTE FontImageData[] = {
#include "font.dat"
};

// インスタンス作成
CDebugFont* CDebugFont::CreateDebugFont(
    IMemoryAllocator* pAllocator,
    graph::CGraphicsDevice* pDevice)
{
    IZ_ASSERT(pDevice != IZ_NULL);
    IZ_ASSERT(pAllocator != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;
    IZ_UINT8* pBuf = IZ_NULL;
    CDebugFont* pInstance = IZ_NULL;

    // メモリ確保
    pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CDebugFont));
    result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // インスタンス作成
    pInstance = new (pBuf)CDebugFont;
    result = (pInstance != IZ_NULL);
    VGOTO(result, __EXIT__);

    {
        pInstance->AddRef();

        pInstance->m_Allocator = pAllocator;

        // テクスチャ作成
        result = pInstance->CreateTexture(pDevice);
        VGOTO(result, __EXIT__);

        pInstance->m_Renderer = graph::C2DRenderer::Create2DRenderer(pDevice, pAllocator);
        result = (pInstance->m_Renderer != IZ_NULL);
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
    m_Allocator = IZ_NULL;

    m_Renderer = IZ_NULL;

    m_pFontTex = IZ_NULL;

    m_nLeft = 0;
    m_nTop = 0;
    m_nColor = 0xffffffff;

    m_nLeftBase = 0;

    m_bIsBegin = IZ_FALSE;
}

// デストラクタ
CDebugFont::~CDebugFont()
{
    SAFE_RELEASE(m_Renderer);
    SAFE_RELEASE(m_pFontTex);
}

// 解放
void CDebugFont::InternalRelease()
{
    delete this;

    if (m_Allocator != IZ_NULL) {
        m_Allocator->Free(this);
    }
}

/**
* 描画開始
*/
IZ_BOOL CDebugFont::Begin(graph::CGraphicsDevice* device)
{
    return Begin(device, 0, 0);
}

IZ_BOOL CDebugFont::Begin(
    graph::CGraphicsDevice* device,
    IZ_INT left, IZ_INT top)
{
    IZ_ASSERT(device != IZ_NULL);
    IZ_ASSERT(m_pFontTex != IZ_NULL);

    device->SetTexture(0, m_pFontTex);
    m_bIsBegin = IZ_TRUE;

    m_nLeft = left;
    m_nTop = top;
    m_nColor = 0xffffffff;

    m_nLeftBase = 0;

    m_Renderer->Begin();

    IZ_BOOL ret = m_Renderer->BeginDraw(device);

    return ret;
}

/**
* 描画終了
*/
void CDebugFont::End()
{
    m_bIsBegin = IZ_FALSE;

    m_Renderer->EndDraw();
    m_Renderer->End();
}

/////////////////////////////////////////////////

// テクスチャ作成
IZ_BOOL CDebugFont::CreateTexture(graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);
    IZ_ASSERT(m_pFontTex == IZ_NULL);

    // テクスチャ本体作成
    m_pFontTex = device->CreateTexture(
                    IMAGE_WIDTH,
                    IMAGE_HEIGHT,
                    0xffffffff,
                    graph::E_GRAPH_PIXEL_FMT_RGBA8,
                    graph::E_GRAPH_RSC_USAGE_STATIC);
    VRETURN(m_pFontTex != IZ_NULL);
    
    // データセット
    IZ_BYTE* data = IZ_NULL;
    IZ_UINT nPitch = m_pFontTex->Lock(
                        0, 
                        reinterpret_cast<void**>(&data),
                        IZ_FALSE);
    VRETURN(nPitch > 0);

    size_t nSize = nPitch * IMAGE_HEIGHT;
    VRETURN(nSize == sizeof(FontImageData));

    memcpy(data, FontImageData, nSize);

    m_pFontTex->Unlock(0);
    
    return IZ_TRUE;
}

// 文字描画
void CDebugFont::DrawFont(
    graph::CGraphicsDevice* device,
    const IZ_CHAR* str)
{
    IZ_ASSERT(device != NULL);
    IZ_ASSERT(m_pFontTex != NULL);
    IZ_ASSERT(m_bIsBegin);

    m_Renderer->SetTexture(0, m_pFontTex);

    CIntRect rcSrc;

    CIntRect rcDst;
    {
        rcDst.left = m_nLeft;
        rcDst.top = m_nTop;
        rcDst.right = rcDst.left + FONT_SIZE;
        rcDst.bottom = rcDst.top + FONT_SIZE;
    }

    IZ_UINT nLength = (IZ_UINT)strlen(str);
    
    for (IZ_UINT i = 0; i < nLength; ++i) {
        IZ_CHAR ch = str[i];

        if (ch == '\n') {
            // 改行
            rcDst.OffsetRect(0, FONT_SIZE);
            m_nTop += FONT_SIZE;

            m_nLeft = m_nLeftBase;
        }
        else {
            SetTexRect(ch, rcSrc);

            // 描画
            m_Renderer->DrawSprite(
                rcSrc, rcDst,
                m_nColor);

            rcDst.OffsetRect(FONT_SIZE, 0);

            m_nLeft += FONT_SIZE;
        }
    }
}

// 切り出し矩形をセットする
void CDebugFont::SetTexRect(
	IZ_CHAR ch,
	CIntRect& rc)
{
	IZ_INT nPosY = ch / FONT_NUM_PER_LINE_IN_IMAGE;
	IZ_INT nPosX = ch % FONT_NUM_PER_LINE_IN_IMAGE;

	rc.left = FONT_SIZE * nPosX;
	rc.top = FONT_SIZE * nPosY;
	rc.right = rc.left + FONT_SIZE;
	rc.bottom = rc.top + FONT_SIZE;
}

static const IZ_INT BUF_SIZE = 1024;
static IZ_CHAR BUF[BUF_SIZE];

/**
* 描画
*/
void CDebugFont::DBPrint(
    graph::CGraphicsDevice* device,
    const IZ_CHAR* str, ...)
{
    FILL_ZERO(BUF, sizeof(BUF));

    va_list ap; // 引数リスト

    va_start(ap, str);

    vsprintf_s(BUF, sizeof(BUF), str, ap);

    va_end(ap);

    DrawFont(device, BUF);
}

/**
* 描画
*/
void CDebugFont::DBPrint(
    graph::CGraphicsDevice* device,
    IZ_DWORD color,
    const IZ_CHAR* str, ...)
{
    FILL_ZERO(BUF, sizeof(BUF));

    va_list ap; // 引数リスト

    va_start(ap, str);

    vsprintf_s(BUF, sizeof(BUF), str, ap);

    va_end(ap);

#if 0
    // 元の値を覚えておく
    IZ_DWORD buf_color = m_nColor;
#endif

    SetFontColor(color);
    DrawFont(device, BUF);

#if 0
    // 元に戻す
    SetFontColor(buf_color);
#endif
}

/**
* 描画
*/
void CDebugFont::DBPrint(
    graph::CGraphicsDevice* device,
    IZ_INT left, IZ_INT top,
    const IZ_CHAR* str, ...)
{
    FILL_ZERO(BUF, sizeof(BUF));

    va_list ap; // 引数リスト

    va_start(ap, str);

    vsprintf_s(BUF, sizeof(BUF), str, ap);

    va_end(ap);

#if 0
    // 元の値を覚えておく
    IZ_INT buf_left = m_nLeft;
    IZ_INT buf_top = m_nTop;
#endif

    SetFontPos(left, top);
    DrawFont(device, BUF);

#if 0
    // 元に戻す
    SetFontPos(buf_left, buf_top);
#endif
}

/**
* 描画
*/
void CDebugFont::DBPrint(
    graph::CGraphicsDevice* device,
    IZ_INT left, IZ_INT top,
    IZ_DWORD color,
    const IZ_CHAR* str, ...)
{
    FILL_ZERO(BUF, sizeof(BUF));

    va_list ap; // 引数リスト

    va_start(ap, str);

    vsprintf_s(BUF, sizeof(BUF), str, ap);

    va_end(ap);

#if 0
    // 元の値を覚えておく
    IZ_INT buf_left = m_nLeft;
    IZ_INT buf_top = m_nTop;
    IZ_DWORD buf_color = m_nColor;
#endif

    SetFontPos(left, top);
    SetFontColor(color);
    DrawFont(device, BUF);

#if 0
    // 元に戻す
    SetFontPos(buf_left, buf_top);
    SetFontColor(buf_color);
#endif
}