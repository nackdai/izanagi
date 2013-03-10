#include <vector>
#include "CharList.h"

namespace {
    class CFileStream : public IInputStream {
    public:
        CFileStream(FILE* fp) { m_FP = fp; }
        ~CFileStream() { if (m_FP) { fclose(m_FP); m_FP = IZ_NULL; } }

    public:
        IZ_INT Get()
        {
            IZ_ASSERT(m_FP != IZ_NULL);
            IZ_INT ret = fgetc(m_FP);
            return ret;
        }

    private:
        FILE* m_FP;
    };

    class CBufferStream : public IInputStream {
    public:
        CBufferStream(void* buf, size_t size)
        {
            m_pBuf = reinterpret_cast<IZ_BYTE*>(buf);
            m_nSize = static_cast<IZ_UINT>(size);
            m_nPos = 0;
        }
        ~CBufferStream() {}

    public:
        IZ_INT Get()
        {
            IZ_INT ret = EOF;
            if (m_nPos < m_nSize) {
                ret = *(m_pBuf++);
                m_nPos++;
            }
            return ret;
        }

    private:
        IZ_BYTE* m_pBuf;
        IZ_UINT m_nSize;
        IZ_UINT m_nPos;
    };
}   // namespace

CCharList CCharList::s_cInstance;

// Registers characters from a file.
void CCharList::Register(
    izanagi::text::E_FONT_CHAR_ENCODE nCharFmt,
    izanagi::IInputStream* pIn)
{
    m_nCharFmt = nCharFmt;

    // 使用する文字のみを調べて登録する

    std::vector<IZ_BYTE> tvBuffer;

    // ファイルの中身を全て読み込む
    {
        // 読み込みサイズ確定
        IZ_UINT nSize = (IZ_UINT)pIn->GetSize();
        IZ_ASSERT(nSize > 0);

        // バッファ確保
        tvBuffer.resize(nSize);

        // 全て読み込む
        pIn->Read(
            &tvBuffer[0],
            0, 
            sizeof(BYTE) * nSize);
    }

    CBufferStream cInStream(&tvBuffer[0], tvBuffer.size());

    switch (nCharFmt) {
    case izanagi::text::E_FONT_CHAR_ENCODE_SJIS:
        // SJIS
        RegisterSJIS(&cInStream);
        break;
    case izanagi::text::E_FONT_CHAR_ENCODE_UTF16:
        // UTF16
        RegisterUTF16(&cInStream);
        break;
    case izanagi::text::E_FONT_CHAR_ENCODE_UTF8:
        // UTF8
        RegisterUTF8(&cInStream);
        break;
    }
}

namespace {
    // Returns whether the character code is surrogate.
    inline IZ_BOOL _IsSurrogate(IZ_UINT ch)
    {
        IZ_BOOL ret = (((0xD800 <= ch) && (ch <= 0xDBFF))           // 上位サロゲート
                        || ((0xDC00 <= ch) && (ch <= 0xDFFF)));     // 下位サロゲート
        return ret;
    }

    // 改行コード
    inline IZ_BOOL _IsLineFeedCode(IZ_UINT ch)
    {
        IZ_BOOL ret = ((ch == 0x0d) || (ch == 0x0a));
        return ret;
    }
}

// Registers characters as SJIS.
void CCharList::RegisterSJIS(IInputStream* pIn)
{
    for (;;) {
        IZ_INT ch = pIn->Get();
        if (ch == EOF) {
            break;
        }

        if (izanagi::tool::CCharCodeUtility::IsAsciiExt(ch)) {
            m_CharList.insert(ch);
        }
        else if (izanagi::tool::CCharCodeUtility::IsSjis(ch)) {
            IZ_INT ch_1 = pIn->Get();
            IZ_UINT val = ((ch << 8) | (ch_1 & 0xff));
            m_CharList.insert(val);
        }
    }
}

// Registers characters as UTF16
void CCharList::RegisterUTF16(IInputStream* pIn)
{
    for (;;) {
        IZ_INT ch = pIn->Get();
        if (ch == EOF) {
            break;
        }

        if (izanagi::tool::CCharCodeUtility::IsAscii(ch)) {
            m_CharList.insert(ch);
        }
        else {
            IZ_INT ch_1 = pIn->Get();
            IZ_UINT val = ((ch << 8) | (ch_1 & 0xff));

            if (_IsSurrogate(val)) {
                ch = pIn->Get();
                ch_1 = pIn->Get();
                IZ_UINT val_1 = ((ch << 8) | (ch_1 & 0xff));

                val = ((val << 16) | (val_1 & 0xffff));
            }

            m_CharList.insert(val);
        }
    }
}

// Registers characters as UTF8.
void CCharList::RegisterUTF8(IInputStream* pIn)
{
    IZ_INT ch = -1;

    for (;;) {
        if (ch < 0) {
            ch = pIn->Get();
        }
        if (ch == EOF) {
            break;
        }

        if (_IsLineFeedCode(ch)) {
            ch = -1;
            continue;
        }
        else if (izanagi::tool::CCharCodeUtility::IsAscii(ch)) {
            m_CharList.insert(ch);
            ch = -1;
        }
        else {
            IZ_UINT count = 1;
            IZ_UINT code = ch;

            for (;;) {
                ch = pIn->Get();
                count++;

                if (ch < 0) {
                    break;
                }
                else if (_IsLineFeedCode(ch)) {
                    ch = -1;
                    break;
                }
                else if ((ch & 0xc0) == 0xc0) {
                    break;
                }
                else {
                    VRETURN_VAL(count <= 4,);
                    code = (code << 8) | (ch & 0xff);
                }
            }

            m_CharList.insert(code);
        }
    }
}

// Returns character code bytes by character format.
IZ_UINT CCharList::GetCharBytes(IZ_UINT code)
{
    IZ_UINT ret = 0;

    switch (m_nCharFmt) {
    case izanagi::text::E_FONT_CHAR_ENCODE_SJIS:
        // SJIS
        ret = GetCharBytesAsSJIS(code);
        break;
    case izanagi::text::E_FONT_CHAR_ENCODE_UTF16:
        // UTF16
        ret = GetCharBytesAsUTF16(code);
        break;
    case izanagi::text::E_FONT_CHAR_ENCODE_UTF8:
        // UTF8
        ret = GetCharBytesAsUTF8(code);
        break;
    }

    return ret;
}

// Returns character code bytes as SJIS
IZ_UINT CCharList::GetCharBytesAsSJIS(IZ_UINT code)
{
    if (izanagi::tool::CCharCodeUtility::IsAscii(code)) {
        // ASCII
        return 1;
    }
    return 2;
}

// Returns character code bytes as UTF16
IZ_UINT CCharList::GetCharBytesAsUTF16(IZ_UINT code)
{
    IZ_UINT16 val = (code & 0xffff);
    if (_IsSurrogate(val)) {
        return 4;
    }
    
    return 2;
}

// Returns character code bytes as UTF8
IZ_UINT CCharList::GetCharBytesAsUTF8(IZ_UINT code)
{
    if (izanagi::tool::CCharCodeUtility::IsAscii(code)) {
        return 1;
    }
    else {
        if (((code >> 24) & 0xf8) == 0xf0) {
            return 4;
        }
        else if (((code >> 16) & 0xf0) == 0xe0) {
            return 3;
        }
        else if (((code>> 8) & 0xe0) == 0xc0) {
            return 2;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return 0;
}
