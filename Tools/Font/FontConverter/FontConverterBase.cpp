#include "FontConverterBase.h"
#include "Option.h"

// コンストラクタ
CFontConverterBase::CFontConverterBase()
{
}

// デストラクタ
CFontConverterBase::~CFontConverterBase()
{
    Release();
}

namespace
{
    IZ_UINT _GetUnicode(
        izanagi::E_FONT_CHAR_ENCODE encode,
        IZ_UINT code)
    {
        // TODO
        // endianが逆？

        IZ_UINT ret = code;

        if (izanagi::tool::CCharCodeUtility::IsAsciiExt(ret))
        {
            return ret;
        }

        union 
        {
            IZ_UINT code;
            IZ_UINT8 ch[4];
        } tmp;

        tmp.code = code;

        int tmpSize = 0;
        std::vector<BYTE> tmpBuf;

        switch (encode)
        {
        case izanagi::E_FONT_CHAR_ENCODE_UTF8:
            {
                ret = ((tmp.ch[0] << 24) | (tmp.ch[1] << 16) | (tmp.ch[2] << 8) | tmp.ch[3]);
                tmpSize = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&ret, -1, NULL, 0);
                tmpBuf.resize(tmpSize * 2 + 2);
                ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&ret, -1, (LPWSTR)&tmpBuf[0], tmpBuf.size());
            }
            break;
        case izanagi::E_FONT_CHAR_ENCODE_SJIS:
            {
                ret = ((tmp.ch[0] << 8) | tmp.ch[1]);
                tmpSize = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)&ret, -1, NULL, 0);
                tmpBuf.resize(tmpSize * 2 + 2);
                ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)&ret, -1, (LPWSTR)&tmpBuf[0], tmpBuf.size());
            }
            break;
        default:
            break;
        }

        if (tmpSize > 0)
        {
            size_t size = ::strlen((const char*)&tmpBuf[0]);
            memcpy(&ret, &tmpBuf[0], size);
        }

        return ret;
    }
}

// フォントイメージ作成
IZ_BOOL CFontConverterBase::CreateFontImage(
    const SOption& sOption,
    const std::set<IZ_UINT>& tsCharList)
{
    IZ_BOOL ret = IZ_TRUE;

    // 登録文字数
    const UINT nRegisterCharNum = (UINT)tsCharList.size();

    // 出力したフォント数
    UINT nOutFontNum = 0;

    // Lockフラグ
    BOOL bIsLocked = FALSE;

    // 登録文字コード
    std::set<IZ_UINT>::const_iterator itChar = tsCharList.begin();

    izanagi::tool::CGraphicsDeviceLite* pDevice = izanagi::tool::CGraphicsDeviceLite::GetInstance();

    // 現在のテクスチャ
    izanagi::tool::CTextureLite* pCurTex = NULL;

    // マップ情報リスト確保
    m_FontMapList.resize(tsCharList.size());

    VRETURN(BeginCreateFontImage(sOption));

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
            izanagi::text::S_FNT_IMAGE sFntImage;

            sFntImage.width = sOption.texWidth;
            sFntImage.height = sOption.texHeight;
            sFntImage.pitch = sFntImage.width * TEX_BPP;
            sFntImage.numFont = 0;
            sFntImage.images = NULL;
            sFntImage.sizeData = sOption.texWidth * sOption.texHeight * TEX_BPP;

            m_FontImgList.push_back(sFntImage);
        }
        izanagi::text::S_FNT_IMAGE& sFntImage = m_FontImgList[nTexNum];

        // 登録
        m_TexList.push_back(pTex);
        pCurTex = pTex;

        // ロック
        void* dst = IZ_NULL;
        IZ_UINT pitch = 0;
        {
            pitch = pCurTex->Lock(0, &dst);
            VRETURN(dst != IZ_NULL);
        
            bIsLocked = TRUE;
        }

        // 出力位置
        // NOTE
        // 上下左右マージン分空ける
        UINT x = MARGIN;
        UINT y = MARGIN;

        for (UINT nCharNum = 0; ; nCharNum++) {
            // 文字コード
            UINT code = *itChar;

            // Unicodeに変換
            code = _GetUnicode(
                sOption.charEncode,
                code);

            SGlyphMetrics metrics;

            // NOTE
            // Unicodeで渡すこと!
            IZ_UINT size = GetGlyphMetrics(
                code,
                sOption,
                metrics);

            // 文字データ取得
            std::vector<IZ_BYTE> buf(size, 0);

            GetGlyphImage(
                code,
                sOption,
                metrics,
                &buf[0],
                size);

            IZ_UINT fontWidth = metrics.advance;
            IZ_UINT fontHeight = GetTextMetricsHeight();

            // テクスチャ内に収まるかチェック
            if (x + fontWidth + MARGIN > sOption.texWidth) {
                // 次の行の位置へ移動
                x = MARGIN;
                y += fontHeight + MARGIN;

                if (y + fontHeight >= sOption.texHeight) {
                    // テクスチャからはみ出す
                    if (bIsLocked) {
                        pCurTex->Unlock(0);
                    }
                    break;
                }
            }

            // TODO
            IZ_UINT offsetX = metrics.bearingX;
            IZ_UINT offsetY = metrics.ascender - metrics.bearingY;
            //IZ_UINT offsetY = 0;

            WriteImage(
                sOption,
                metrics,
                &buf[0],
                dst,
                pitch, TEX_BPP,
                x, y,
                offsetX, offsetY);

            // マップ情報
            izanagi::text::S_FNT_MAP& sFntMap = m_FontMapList[nOutFontNum];
            {
                sFntMap.idx = nOutFontNum;
                sFntMap.code = code;
                sFntMap.srcX = x;
                sFntMap.srcY = y;
                sFntMap.texID = nTexNum;

                // TODO
                // metrics
                // image
            }

            // フォントサイズ分移動
            x += fontWidth + MARGIN;

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

// 開放
void CFontConverterBase::Release()
{
    {
        std::vector<izanagi::tool::CTextureLite*>::iterator it = m_TexList.begin();

        for (; it != m_TexList.end(); it++) {
            izanagi::tool::CTextureLite* p = *it;
            SAFE_RELEASE(p);
        }

        m_TexList.clear();
    }
}

// DDSとして出力
BOOL CFontConverterBase::ExportAsDDS()
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
BOOL CFontConverterBase::ExportAs(
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
BOOL CFontConverterBase::Export(
    izanagi::IOutputStream* pOut,
    const SOption& sOption,
    const std::set<IZ_UINT>& tsCharList)
{
    // 一度全てのテクスチャを出力する
    VRETURN(ExportAsDDS());

    BOOL ret = TRUE;

    // ファイルヘッダ
    izanagi::text::S_FNT_HEADER sHeader;
    {
        memset(&sHeader, 0, sizeof(sHeader));

        sHeader.magic = izanagi::text::FNT_MAGIC_NUMBER;
        sHeader.version = izanagi::text::E_FNT_VERSION_CURRENT;

        sHeader.sizeHeader = sizeof(sHeader);

        sHeader.numFont = static_cast<IZ_UINT16>(tsCharList.size());
        sHeader.numTex = static_cast<IZ_UINT8>(m_TexList.size());
        sHeader.fontHeight = GetTextMetricsHeight();

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
            sizeof(izanagi::text::S_FNT_MAP) *    m_FontMapList.size());
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
UINT CFontConverterBase::ExportFontImageData(izanagi::IOutputStream* pOut)
{
    UINT num = static_cast<UINT>(m_FontImgList.size());

    if (num == 0) {
        // 出力するものが無い
        return TRUE;
    }

    BOOL result = FALSE;
    UINT nMaxSize = 0;

    for (UINT i = 0; i < num; i++) {
        const izanagi::text::S_FNT_IMAGE& sFntImage = m_FontImgList[i];
        izanagi::tool::CTextureLite* pTex = m_TexList[i];

        // 情報部出力
        pOut->Write(
            &sFntImage,
            0,
            sizeof(sFntImage));

        // データ部出力
        void* dst = IZ_NULL;
        pTex->Lock(0, (void**)&dst);
        result = (dst != IZ_NULL);
        
        if (!result) {
            IZ_ASSERT(FALSE);
            break;
        }

        pOut->Write(
            dst,
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
