#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include "FontConverterImpl.h"
#include "CharList.h"

//#define REMOVE_TMP_DDS

CFontConverter CFontConverter::s_cInstance;

// コンストラクタ
CFontConverter::CFontConverter()
{
    m_hFont = NULL;
    m_hOldFont = NULL;

    m_TexMetrics.height = 0;
    m_TexMetrics.ascent = 0;
}

// デストラクタ
CFontConverter::~CFontConverter()
{
    Release();
}

// 開放
void CFontConverter::Release()
{
    {
        std::vector<izanagi::tool::CTextureLite*>::iterator it = m_TexList.begin();

        for (; it != m_TexList.end(); it++) {
            izanagi::tool::CTextureLite* p = *it;
            SAFE_RELEASE(p);
        }

        m_TexList.clear();
    }

    if (m_hFont != NULL) {
        ::DeleteObject(m_hFont);
        m_hFont = NULL;
    }
}

// DDSとして出力
BOOL CFontConverter::ExportAsDDS()
{
    BOOL ret = TRUE;

    std::vector<izanagi::tool::CTextureLite*>::iterator it = m_TexList.begin();

    for (UINT cnt = 0; it != m_TexList.end(); it++, cnt++) {
        izanagi::tool::CTextureLite* p = *it;

        izanagi::tool::CString strOut;
        strOut.format("%d.dds", cnt);

        ret = ExportAs(
                p,
                strOut,
                D3DXIFF_DDS);

        if (!ret) {
            IZ_ASSERT(ret);
            break;
        }
    }

    return ret;
}

// ファイルフォーマットを指定してイメージ出力
BOOL CFontConverter::ExportAs(
    izanagi::tool::CTextureLite* pTex,
    LPCSTR pszName,
    D3DXIMAGE_FILEFORMAT nFileFmt)
{
    BOOL ret = TRUE;

    D3D_TEXTURE* pD3DTex = pTex->GetRawInterface();

    HRESULT hr = D3DXSaveTextureToFile(
                    pszName,
                    nFileFmt,
                    pD3DTex,
                    NULL);

    ret = SUCCEEDED(hr);
    IZ_ASSERT(ret);

    return ret;
}

namespace {
    inline IZ_UINT _GetTextHeight(
        HDC hDC,
        izanagi::E_FONT_CHAR_ENCODE encode)
    {
        IZ_UINT ret = 0;

        if (encode == izanagi::E_FONT_CHAR_ENCODE_UTF16) {
            TEXTMETRICW tm;
            GetTextMetricsW(hDC, &tm);
            ret = tm.tmHeight;
        }
        else {
            TEXTMETRICA tm;
            GetTextMetricsA(hDC, &tm);
            ret = tm.tmHeight;
        }

        return ret;
    }
}   // namespace

// 初期化
BOOL CFontConverter::Init(
    HDC hDC,
    const CFontConverter::SOption& sOption)
{
    BOOL ret = FALSE;

    // TODO
    // localeの設定
    //::setlocale(LC_ALL, _T("Japanese"));

    // ピッチ
    BYTE nFontPitch = (sOption.isFixedPitch ? FIXED_PITCH : DEFAULT_PITCH);

    // NOTE
    // フォントファミリは、フォントの見た目を定義します。
    // lpszFace で指定した フォント名のフォントが利用できないときに、フォントの選択に使われます。

    // フォントファミリ
    BYTE nFontFamily = FF_DONTCARE;

    IZ_UINT nFontHeight = sOption.fontSize;

    for (;;) {
        // TODO
        // フォント作成
        m_hFont = ::CreateFont(
                        nFontHeight,        // 高さ
                        0,                  // 幅
                        0,
                        0,
                        sOption.isBold ? FW_BOLD : FW_NORMAL,   // 太さ
                        sOption.isItalic,                       // 斜体
                        FALSE,                                  // 下線
                        FALSE,                                  // StrikeOut (水平線？）
                        DEFAULT_CHARSET,                        // キャラクタセット
                        OUT_TT_ONLY_PRECIS,                     // 出力精度
                        CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY,                      // 品質
                        nFontPitch | nFontFamily,               // ピッチ
                        sOption.fontFace);
        ret = (m_hFont != NULL);
        VRETURN(ret);

        m_hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
        VRETURN(m_hOldFont);

        IZ_UINT nHeight = _GetTextHeight(hDC, sOption.charEncode);
        if (nHeight <= sOption.fontSize) {
            break;
        }

        nFontHeight--;
        IZ_ASSERT(nFontHeight != 0);

        ::SelectObject(hDC, m_hOldFont);
        ::DeleteObject(m_hFont);

        m_hOldFont = IZ_NULL;
        m_hFont = IZ_NULL;
    }

    return ret;
}

namespace {
    inline void _Swap(IZ_BYTE ch[4], IZ_UINT num)
    {
        switch (num) {
        case 2:
            {
                IZ_BYTE tmp = ch[0];
                ch[0] = ch[1];
                ch[1] = tmp;
            }
            break;
        case 3:
            {
                IZ_BYTE tmp = ch[0];
                ch[0] = ch[2];
                ch[2] = tmp;
            }
            break;
        case 4:
            {
                IZ_BYTE tmp = ch[0];
                ch[0] = ch[3];
                ch[3] = ch[0];

                tmp = ch[1];
                ch[1] = ch[2];
                ch[2] = tmp;
            }
            break;
        }
    }

    inline DWORD _GetGlyphOutline(
        izanagi::E_FONT_CHAR_ENCODE nCharFmt,
        HDC hdc, 
        IZ_UINT uChar, 
        IZ_UINT uFormat, 
        LPGLYPHMETRICS lpgm, 
        IZ_DWORD cbBuffer, 
        void* lpvBuffer, 
        const MAT2* lpmat2)
    {
        DWORD ret = 0;

        if (nCharFmt == izanagi::E_FONT_CHAR_ENCODE_UTF16) {
            // Unicode
            ret = ::GetGlyphOutlineW(
                    hdc,
                    uChar,
                    uFormat,
                    lpgm,
                    cbBuffer,
                    lpvBuffer,
                    lpmat2);
        }
        else {
            if (nCharFmt == izanagi::E_FONT_CHAR_ENCODE_UTF8
                && !izanagi::tool::CCharCodeUtility::IsAscii(uChar))
            {
                // NOTE
                // GetGlyphOutlineAはSJISしか受け付けないので
                // UTF8 -> SJIS変換が必要
                union {
                    IZ_UINT code;
                    IZ_BYTE ch[4];
                } tmp;

                tmp.code = uChar;

                IZ_UINT zeroPos = 0;
                for (; zeroPos < COUNTOF(tmp.ch); zeroPos++) {
                    if (tmp.ch[zeroPos] == 0) {
                        break;
                    }
                }

                _Swap(tmp.ch, zeroPos);

                IZ_UINT size = izanagi::tool::CCharCodeUtility::ConvUtf8ToSjis((const char*)tmp.ch, NULL);

                std::vector<BYTE> buf(size + 1);
                izanagi::tool::CCharCodeUtility::ConvUtf8ToSjis((const char*)tmp.ch, &buf[0]);

                uChar = 0;
                IZ_UINT count = IZ_MIN(COUNTOF(tmp.ch), (IZ_UINT)buf.size());
                for (IZ_UINT i = 0; i < count; i++) {
                    if (buf[i] > 0) {
                        uChar = (uChar << (8 * i)) | buf[i];
                    }
                }
            }

            // SJIS or UTF8
            ret = ::GetGlyphOutlineA(
                    hdc,
                    uChar,
                    uFormat,
                    lpgm,
                    cbBuffer,
                    lpvBuffer,
                    lpmat2);
        }

        return ret;
    }

#ifndef CLAMP
#define CLAMP(v, a, b)  ((v) < (a) ? (a) : ((v) > (b) ? (b) : (v)))
#endif

    // 描き込み
    void _WriteColorWithAA(
        const GLYPHMETRICS& gm,
        const LONG nTMAscent,
        BYTE* pSrc,
        UINT nSrcPitch,
        BYTE* pDst,
        UINT nDstPitch, UINT nDstBpp,
        UINT nDstX, UINT nDstY,
        UINT nDstLeft, UINT nDstTop,
        UINT nAALevel)
    {
        UINT nWidth = gm.gmBlackBoxX;
        UINT nHeight = gm.gmBlackBoxY;

        UINT nOffsetX = gm.gmptGlyphOrigin.x;
        UINT nOffsetY = nTMAscent - gm.gmptGlyphOrigin.y;

        for (UINT y = 0; y < nHeight; y++) {
            BYTE* src = pSrc + nSrcPitch * y;
            BYTE* dst = pDst + nDstPitch * (nDstY + nDstTop + y) + (nDstX + nDstLeft) * nDstBpp;

            for (UINT x = 0; x < nWidth; x++) {
#if 0
                BYTE alpha = (*src++) * 255 / nAALevel; // よく分からんがこういうもんらしい・・・
#else
                UINT tmp = *src++;
                BYTE alpha = static_cast<BYTE>(CLAMP((tmp * 256 / 16), 0, 255));
#endif

#ifdef _TEX_FMT_A8_
                *dst++ = alpha;
#else
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = alpha;
#endif
            }
        }
    }

    // 描き込み
    void _WriteColor(
        const GLYPHMETRICS& gm,
        const LONG nTMAscent,
        BYTE* pSrc,
        BYTE* pDst,
        UINT nDstPitch, UINT nDstBpp,
        UINT nDstX, UINT nDstY,
        UINT nDstLeft, UINT nDstTop)
    {
        UINT nOffsetX = gm.gmptGlyphOrigin.x;
        UINT nOffsetY = nTMAscent - gm.gmptGlyphOrigin.y;
        UINT nPitch = (1 + (gm.gmBlackBoxX / 32)) * 4;

        UINT nHeight = gm.gmBlackBoxY;
        UINT nWidth = gm.gmBlackBoxX;

        for (UINT y = 0; y < nHeight; y++) {
            BYTE* dst = pDst + nDstPitch * (nDstY + nDstTop + y) + (nDstX + nDstLeft) * nDstBpp;

            for (UINT x = 0; x < nWidth; x++) {
                DWORD num = x / 8;      // 現在のxが1行の何BYTE目かを算出
                BYTE bit = x % 8;       // 現在のxが1バイト内の何ビット目かを算出
                BYTE mask = ((BYTE)1) << (7 - bit); // 現在のビット数のマスク作成

                // 現在のバイト位置にあるビットマップをを取得
                BYTE cur = *(pSrc + nPitch * y + num);

                // 作成したマスクと現在のバイト位置とで論理積演算
                cur &= mask;

                // curに立ったビットフラグを最下位ビットまでシフトしてアルファ値に変換
                BYTE alpha = (cur >> (7 - bit)) * 255;
                
#ifdef _TEX_FMT_A8_
                *dst++ = alpha;
#else
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = alpha;
#endif
            }
        }
    }

#define _SUBSTITUTE_PTR(p, val) if (p) { *p = val; }

    inline void _GetTextMetrics(
        HDC hDC,
        izanagi::E_FONT_CHAR_ENCODE fmt,
        IZ_UINT* pAscent,
        IZ_UINT* pHeight)
    {
        if (fmt == izanagi::E_FONT_CHAR_ENCODE_UTF16) {
            TEXTMETRICW tm;
            GetTextMetricsW(hDC, &tm);

            _SUBSTITUTE_PTR(pAscent, tm.tmAscent);
            _SUBSTITUTE_PTR(pHeight, tm.tmHeight);
        }
        else {
            TEXTMETRICA tm;
            GetTextMetricsA(hDC, &tm);

            _SUBSTITUTE_PTR(pAscent, tm.tmAscent);
            _SUBSTITUTE_PTR(pHeight, tm.tmHeight);
        }
    }
}   // namespace

// フォントイメージ作成
BOOL CFontConverter::CreateFontImage(
    HDC hDC,
    const SOption& sOption)
{
    IZ_BOOL ret = TRUE;

    if (m_hOldFont == IZ_NULL) {
        m_hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
        VRETURN(m_hOldFont);
    }

    const std::set<IZ_UINT>& tsCharList = CCharList::GetInstance().GetCharList();

    // 登録文字数
    const UINT nRegisterCharNum = (UINT)tsCharList.size();

    // 出力したフォント数
    UINT nOutFontNum = 0;

    // Lockフラグ
    BOOL bIsLocked = FALSE;

    // 登録文字コード
    std::set<IZ_UINT>::const_iterator itChar = tsCharList.begin();

    GLYPHMETRICS sGlyphMetrics;
    const MAT2 mat = {{0, 1},{0, 0},{0, 0},{0, 1}};

    // TODO
    IZ_UINT nTMAscent, nTMHeight;
    _GetTextMetrics(
        hDC,
        sOption.charEncode,
        &nTMAscent,
        &nTMHeight);

    m_TexMetrics.height = nTMHeight;
    m_TexMetrics.ascent = nTMAscent;

    // アンチエイリアスレベル
    IZ_UINT nAALevel = 1;
    switch (sOption.typeAA) {
        case GGO_GRAY2_BITMAP:
            nAALevel = 4;
            break;
        case GGO_GRAY4_BITMAP:
            nAALevel = 16;
            break;
        case GGO_GRAY8_BITMAP:
            nAALevel = 64;
            break;
    }

    izanagi::tool::CGraphicsDeviceLite* pDevice = izanagi::tool::CGraphicsDeviceLite::GetInstance();

    // 現在のテクスチャ
    izanagi::tool::CTextureLite* pCurTex = NULL;

    // マップ情報リスト確保
    m_FontMapList.resize(tsCharList.size());

    for (UINT nTexNum = 0; ; nTexNum++) {
        // テクスチャ作成
        izanagi::tool::CTextureLite* pTex = pDevice->CreateTexture(
                                                    sOption.texWidth,
                                                    sOption.texHeight,
                                                    D3D_TEX_FMT);
        if (pTex == IZ_NULL) {
            IZ_ASSERT(FALSE);
            ret = FALSE;
            break;
        }

        // フォントイメージ情報
        {
            izanagi::S_FNT_IMAGE sFntImage;

            sFntImage.width = sOption.texWidth;
            sFntImage.height = sOption.texHeight;
            sFntImage.pitch = sFntImage.width * TEX_BPP;
            sFntImage.numFont = 0;
            sFntImage.images = NULL;
            sFntImage.sizeData = sOption.texWidth * sOption.texHeight * TEX_BPP;

            m_FontImgList.push_back(sFntImage);
        }
        izanagi::S_FNT_IMAGE& sFntImage = m_FontImgList[nTexNum];

        // 登録
        m_TexList.push_back(pTex);
        pCurTex = pTex;

        // ロック
        void* pBits = IZ_NULL;
        IZ_UINT nTexPitch = 0;
        {
            nTexPitch = pCurTex->Lock(0, &pBits);
            VRETURN(pBits != IZ_NULL);
        
            bIsLocked = TRUE;
        }

        // 出力位置
        // NOTE
        // 上下左右マージン分空ける
        UINT nDstX = MARGIN;
        UINT nDstY = MARGIN;

        for (UINT nCharNum = 0; ; nCharNum++) {
            // 文字コード
            UINT code = *itChar;

            // 文字情報取得
            DWORD size = _GetGlyphOutline(
                            sOption.charEncode,
                            hDC,
                            code,
                            sOption.typeAA,
                            &sGlyphMetrics,
                            0,
                            NULL,
                            &mat);

            // 文字データ取得
            std::vector<IZ_BYTE> buf(size, 0);
            _GetGlyphOutline(
                sOption.charEncode,
                hDC,
                code,
                sOption.typeAA,
                &sGlyphMetrics,
                size, &buf[0],
                &mat);

            // フォントサイズ
#if 0
            IZ_UINT nFontW = (sOption.isFixedPitch
                                ? nTMHeight
                                : sGlyphMetrics.gmBlackBoxX);
#else
            IZ_UINT nFontW = sGlyphMetrics.gmBlackBoxX;
#endif
            IZ_UINT nFontH = nTMHeight;

            // ピッチ（４バイトアライン）
            IZ_UINT pitch = (sGlyphMetrics.gmBlackBoxX + 3) & 0xfffc;

            // 出力左上位置
            UINT nDstTop = nTMAscent - sGlyphMetrics.gmptGlyphOrigin.y;
            UINT nDstLeft = sGlyphMetrics.gmptGlyphOrigin.x;
            {
                IZ_ASSERT(nFontW >= sGlyphMetrics.gmBlackBoxX);
                nDstLeft = nFontW - sGlyphMetrics.gmBlackBoxX;

                // TODO
                // どっちがいい・・・？
#if 0
                nDstLeft >>= 1;
#else
                nDstLeft = (nDstLeft >> 1) + (nDstLeft & 0x01);
#endif

                IZ_ASSERT(nDstLeft + sGlyphMetrics.gmBlackBoxX <= nFontW);
            }

            // テクスチャ内に収まるかチェック
            if (nDstX + nFontW + MARGIN > sOption.texWidth) {
                // 次の行の位置へ移動
                nDstX = MARGIN;
                nDstY += nFontH + MARGIN;

                if (nDstY + nFontH >= sOption.texHeight) {
                    // テクスチャからはみ出す
                    if (bIsLocked) {
                        pCurTex->Unlock(0);
                    }
                    break;
                }
            }

            // 描き込み
            if (sOption.typeAA == GGO_BITMAP) {
                _WriteColor(
                    sGlyphMetrics,
                    nTMAscent,
                    &buf[0],            // pSrc
                    (BYTE*)pBits,       // pDst
                    nTexPitch,          // pDstPitch
                    TEX_BPP,            // pDstBpp
                    nDstX, nDstY,
                    nDstLeft, nDstTop);
            }
            else {
                _WriteColorWithAA(
                    sGlyphMetrics,
                    nTMAscent,
                    &buf[0],            // pSrc
                    pitch,              // nSrcPitch
                    (BYTE*)pBits,       // pDst
                    nTexPitch,          // pDstPitch
                    TEX_BPP,            // pDstBpp
                    nDstX, nDstY,
                    nDstLeft, nDstTop,
                    nAALevel);
            }

            // マップ情報
            izanagi::S_FNT_MAP& sFntMap = m_FontMapList[nOutFontNum];
            {
                sFntMap.idx = nOutFontNum;
                sFntMap.code = code;
                sFntMap.srcX = nDstX;
                sFntMap.srcY = nDstY;
                sFntMap.fontWidth = nFontW;
                sFntMap.texID = nTexNum;
            }

            // フォントサイズ分移動
            nDstX += nFontW + MARGIN;

            itChar++;
            nOutFontNum++;

            sFntImage.numFont++;

            if ((itChar == tsCharList.end())
                || (nOutFontNum == nRegisterCharNum))
            {
                // 全て出力した
                goto __EXIT__;
            }
        }
    }

__EXIT__:
    if (bIsLocked) {
        // LockされているならUnlockする
        IZ_ASSERT(pCurTex != NULL);
        pCurTex->Unlock(0);
    }

    return ret;
}

namespace {
    // ファイルサイズ取得
    inline UINT _GetFileSize(FILE* pFile)
    {
        // 現在位置
        UINT cur = ftell(pFile);

        // ファイル終端に移動
        fseek(pFile, 0, SEEK_END);
        UINT ret = ftell(pFile);

        // 現在位置に戻る
        fseek(pFile, cur, SEEK_SET);

        return ret;
    }

    // ファイルサイズ取得
    inline UINT _GetFileSize(LPCSTR pszFile)
    {
        FILE* pTmp = NULL;
        fopen_s(&pTmp, pszFile, "rb");
        if (pTmp == NULL) {
            IZ_ASSERT(FALSE);
            return 0;
        }

        UINT ret = _GetFileSize(pTmp);

        fclose(pTmp);

        return ret;
    }
}   // namespace

// 出力
BOOL CFontConverter::Export(
    izanagi::IOutputStream* pOut,
    const SOption& sOption)
{
    // 一度全てのテクスチャを出力する
    VRETURN(ExportAsDDS());

    const std::set<IZ_UINT>& tsCharList = CCharList::GetInstance().GetCharList();

    BOOL ret = TRUE;

    // ファイルヘッダ
    izanagi::S_FNT_HEADER sHeader;
    {
        memset(&sHeader, 0, sizeof(sHeader));

        sHeader.magic = izanagi::FNT_MAGIC_NUMBER;
        sHeader.version = izanagi::E_FNT_VERSION_CURRENT;

        sHeader.sizeHeader = sizeof(sHeader);

        sHeader.numFont = static_cast<IZ_UINT16>(tsCharList.size());
        sHeader.numTex = static_cast<IZ_UINT8>(m_TexList.size());
        sHeader.fontHeight = m_TexMetrics.height;

        sHeader.charEncode = sOption.charEncode;
        
        sHeader.texFmt = IZ_TEX_FMT;

        sHeader.isFixedPitch = sOption.isFixedPitch;

        sHeader.sizeMaxImage = 0;
    }

    // ヘッダ分を空けておく
    pOut->Seek(sizeof(sHeader), izanagi::E_IO_STREAM_SEEK_POS_START);

    // マップ情報
    if (!m_FontMapList.empty()) {
        pOut->Write(
            &m_FontMapList[0],
            0,
            sizeof(izanagi::S_FNT_MAP) *    m_FontMapList.size());
    }

    // ファイルサイズ取得
    {
        // イメージ出力
        UINT num = static_cast<UINT>(m_FontImgList.size());
        for (UINT i = 0; i < num; i++) {
            izanagi::tool::CString strTmp;
            strTmp.format("%d.dds", i);

            m_FontImgList[i].sizeData = _GetFileSize(strTmp);
        }
    }

    // イメージ情報部出力
    {
        UINT nMaxSize = ExportFontImageData(pOut);
        ret = (nMaxSize != 0);
        VGOTO(ret, __EXIT__);

        sHeader.sizeMaxImage = nMaxSize;
    }

    sHeader.sizeFile = pOut->GetCurPos();

    // 先頭に戻る
    pOut->Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START);

    // ヘッダを出力する
    pOut->Write(
        &sHeader,
        0,
        sizeof(sHeader));

__EXIT__:
#ifdef REMOVE_TMP_DDS
    // いらないから削除
    {
        // イメージ出力
        UINT num = static_cast<UINT>(m_FontImgList.size());
        for (UINT i = 0; i < num; i++) {
            izanagi::tool::CString strTmp;
            strTmp.format("%d.dds", i);
            izanagi::tool::CFileUtility::RemoveFile(strTmp);
        }
    }
#endif  // #ifdef REMOVE_TMP_DDS

    return ret;
}

// イメージデータ部出力
UINT CFontConverter::ExportFontImageData(izanagi::IOutputStream* pOut)
{
    UINT num = static_cast<UINT>(m_FontImgList.size());

    if (num == 0) {
        // 出力するものが無い
        return TRUE;
    }

    BOOL result = FALSE;
    UINT nMaxSize = 0;

    for (UINT i = 0; i < num; i++) {
        const izanagi::S_FNT_IMAGE& sFntImage = m_FontImgList[i];
        izanagi::tool::CTextureLite* pTex = m_TexList[i];

        // 情報部出力
        pOut->Write(
            &sFntImage,
            0,
            sizeof(sFntImage));

        // データ部出力
        void* pBits = IZ_NULL;
        pTex->Lock(0, (void**)&pBits);
        result = (pBits != IZ_NULL);
        
        if (!result) {
            IZ_ASSERT(FALSE);
            break;
        }

        pOut->Write(
            pBits,
            0,
            sFntImage.sizeData);

        // 最大サイズ
        nMaxSize = (sFntImage.sizeData > nMaxSize
                    ? sFntImage.sizeData
                    : nMaxSize);

        pTex->Unlock(0);
    }

    // 失敗したら０を返す
    nMaxSize = (result ? nMaxSize : 0);

    return nMaxSize;
}
