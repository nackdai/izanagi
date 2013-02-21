#include "std/StdUtf.h"
#include "std/StdBit.h"

using namespace izanagi;

#define __RETURN(b) if (b) { return; }

namespace {
    // Returns whether character code is surrogate.
    inline IZ_BOOL _IsSurrogate(IZ_UINT16 ch)
    {
        IZ_BOOL ret = (((0xD800 <= ch) && (ch <= 0xDBFF))           // 上位サロゲート
                        || ((0xDC00 <= ch) && (ch <= 0xDFFF)));     // 下位サロゲート
        return ret;
    }

    // Converts character code from surrogate.
    inline IZ_UINT _ConvertFromSurrogate(IZ_UINT16 upper, IZ_UINT16 lower)
    {
        // NOTE
        // n : Character code
        // n' = n - 0x10000
        // n' = yyyyyyyyyyxxxxxxxxxx (2)
        // upper = 110110yyyyyyyyyy
        // lower = 110111xxxxxxxxxx

        IZ_UINT ret = (((upper & 0x03ff) << 10) | (lower & 0x03ff));
        ret += 0x10000;
        return ret;
    }
    // Converts character code to surrogate.
    inline IZ_UINT _ConvertToSurrogate(IZ_UINT code)
    {
        // NOTE
        // n : Character code
        // n' = n - 0x10000
        // n' = yyyyyyyyyyxxxxxxxxxx (2)
        // upper = 110110yyyyyyyyyy
        // lower = 110111xxxxxxxxxx

        IZ_UINT tmp = code - 0x10000;

        IZ_UINT upper = (0xd800 | (tmp >> 10));
        IZ_UINT lower = (0xdc00 | (tmp & 0x3ff));

        IZ_UINT ret = ((upper << 16) | lower);
        return ret;
    }
}   // namespace

namespace {
    IZ_UINT _ConvertUTF8toUTF16(IZ_UINT nCode, IZ_UINT* pDstByte)
    {
        union {
            IZ_UINT code;
            IZ_BYTE ch[4];
        } sChar;

        IZ_UINT nDstCode = 0;
        CStdBitOutput cBitOut(&nDstCode, sizeof(nDstCode));

        sChar.code = nCode;

        IZ_UINT bytes = 0;

        if (nCode <= 0x7f) {
            // ASCII
            nDstCode = nCode & (0xffff);
            bytes = 2;
        }
        else
        {
            if (((nCode >> 24) & 0xf8) == 0xf0) {
                nDstCode = sChar.ch[3] & 0x07;
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);

                nDstCode = _ConvertToSurrogate(nDstCode);

                bytes = 4;
            }
            else if (((nCode >> 16) & 0xf0) == 0xe0) {
                nDstCode = sChar.ch[2] & 0x0f;
                nDstCode = (nDstCode << 6) | (sChar.ch[1] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                bytes = 2;
            }
            else if (((nCode >> 8) & 0xe0) == 0xc0) {
                nDstCode = sChar.ch[1] & 0x1f;
                nDstCode = (nDstCode << 6) | (sChar.ch[1] & 0x3f);
                bytes = 2;
            }
            else {
                // Not surpported yet...
                IZ_ASSERT(IZ_FALSE);
            }
        }

        if (pDstByte != IZ_NULL) {
            *pDstByte = bytes;
        }

        return nDstCode;
    }

    IZ_BYTE* _GetUTF8Code(IZ_BYTE* src, IZ_UINT* pRet)
    {
        IZ_BYTE ch = *src;

        if (ch <= 0x7f) {
            return src;
        }

        ++src;

        IZ_UINT count = 1;
        IZ_UINT code = ch;

        for (;; src++) {
            ch = *src;
            count++;

            if (ch <= 0x7f) {
                break;
            }
            else if ((ch & 0xc0) == 0x80) {
                IZ_ASSERT(count <= 4);
                code = ((code << 8) | ch);
            }
            else if (((ch & 0xe0) == 0xc0)
                    || ((ch & 0xf0) == 0xe0)
                    || ((ch & 0xf8) == 0xf0))
            {
                break;
            }
            else {
                // Not surpported yet...
                IZ_ASSERT(IZ_FALSE);
            }
        }

        *pRet = code;

        return src;
    }
}   // namespace

/**
* Converts UTF8 to UTF16
*/
void CStdUtf::ConvertUtf8ToUtf16(
    void* dst,
    IZ_UINT nDstSize,
    IZ_CHAR* src)
{
    // 一応
    FILL_ZERO(dst, nDstSize);

    IZ_UINT nPos = 0;
    IZ_UINT nDstByte = 0;

    IZ_BYTE* pSrc = reinterpret_cast<IZ_BYTE*>(src);

    for (;;) {
        IZ_BYTE ch = *(pSrc++);

        if (ch == 0) {
            return;
        }

        IZ_UINT nCode = 0;

        if (ch <= 0x7f) {
            // ASCII
            nCode = ch;
        }
        else {
            pSrc = _GetUTF8Code(pSrc, &nCode);
        }

        nCode = _ConvertUTF8toUTF16(nCode, &nDstByte);

        __RETURN((nPos + nDstByte) >= nDstSize);

        memcpy(dst, &nCode, nDstByte);
        nPos += nDstByte;
    }

    return;
}

IZ_UINT CStdUtf::ConvertUtf8ToUtf16(IZ_UINT code)
{
    IZ_UINT bytes = 0;
    IZ_UINT ret = _ConvertUTF8toUTF16(code, &bytes);
    return ret;
}

namespace {
    inline IZ_UINT _GetUTF8Byte(IZ_UINT32 nChar)
    {
        IZ_UINT ret = 0;

        if (nChar <= 0x7f) {
            // ASCII
            ret = 1;
        }
        else if (nChar <= 0x7ff) {
            ret = 2;
        }
        else if (nChar <= 0xffff) {
            ret = 3;
        }
        else if (nChar <= 0x10ffff) {
            ret = 4;
        }
        else {
            // Not surpported yet...
            IZ_ASSERT(IZ_FALSE);
        }

        return ret;
    }

    inline IZ_CHAR* _ConvertUTF16toUTF8(IZ_CHAR* dst, IZ_UINT32 nChar)
    {
        if (nChar <= 0x7f) {
            // ASCII
            *(dst++) = (IZ_CHAR)nChar;
        }
        else if (nChar <= 0x7ff) {
            *(dst++) = (0x80 | (nChar >> 0) & 0x3f);
            *(dst++) = (0xc0 | (nChar >> 6) & 0x1f);
        }
        else if (nChar <= 0xffff) {
            *(dst++) = (0x80 | (nChar >> 0) & 0x3f);
            *(dst++) = (0x80 | (nChar >> 6) & 0x3f);
            *(dst++) = (0xe0 | (nChar >> 12) & 0x0f);
        }
        else if (nChar <= 0x10ffff) {
            *(dst++) = (0x80 | (nChar >> 0) & 0x3f);
            *(dst++) = (0x80 | (nChar >> 6) & 0x3f);
            *(dst++) = (0x80 | (nChar >> 12) & 0x3f);
            *(dst++) = (0xf0 | (nChar >> 18) & 0x07);
        }
        else {
            // Not surpported yet...
            IZ_ASSERT(IZ_FALSE);
        }

        return dst;
    }
}   // namespace

/**
* Converts UTF16 to UTF8
*/
void CStdUtf::ConvertUtf16ToUtf8(
    void* dst,
    IZ_UINT nDstSize,
    IZ_UINT16* src)
{
    // 一応
    FILL_ZERO(dst, nDstSize);

    IZ_CHAR* pDst = reinterpret_cast<IZ_CHAR*>(dst);

    IZ_UINT nPos = 0;
    IZ_UINT nDstByte = 0;

    for (;;) {
        IZ_UINT16 ch = *(src++);

        if (ch == 0) {
            return;
        }

        IZ_UINT nCode = 0;

        if (_IsSurrogate(ch)) {
            // Surrogate

            // Lower Surrogate
            IZ_UINT16 lower = ch;

            // Upper Surrogate
            IZ_UINT16 upper = *(src++);
            IZ_ASSERT(_IsSurrogate(upper));

            // Surrogate -> char code
            nCode = _ConvertFromSurrogate(upper, lower);
        }

        nDstByte = _GetUTF8Byte(nCode);

        __RETURN((nPos + nDstByte) >= nDstSize);
        pDst = _ConvertUTF16toUTF8(pDst, nCode);

        nPos += nDstByte;
    }
}

IZ_UINT CStdUtf::ConvertUtf16ToUtf8(IZ_UINT code)
{
    IZ_UINT ret = 0;
    _ConvertUTF16toUTF8((IZ_CHAR*)&ret, code);
    return ret;
}

namespace {
    // Returns whether the character code is SJIS.
    inline IZ_BOOL _IsSjis(IZ_UINT ch)
    {
        return (((0x81 <= ch) && (ch <= 0x9f))
                || ((0xe0 <= ch) && (ch <= 0xef)));
    }
}   // namespace

/**
* Get a character code as specified character encode.
*/
void* CStdUtf::GetOneCharCodeAsUTF8(void* src, IZ_UINT* ret)
{
    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, src);

    *ret = 0;

    IZ_ASSERT(src != IZ_NULL);
    IZ_BYTE* pSrc = reinterpret_cast<IZ_BYTE*>(src);

    IZ_BYTE ch = *pSrc;

    if (ch == 0) {
        goto __EXIT__;
    }

    if (IsAscii(ch)
        || IsSpace(ch))
    {
        // ASCII or Blank
        *ret = ch;
        pSrc++;
    }
    else {
        pSrc = _GetUTF8Code(pSrc, ret);
    }

__EXIT__:
    return pSrc;
}

/**
* Get a character code as specified character encode.
*/
void* CStdUtf::GetOneCharCodeAsUTF16(void* src, IZ_UINT* ret)
{
    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, src);

    *ret = 0;

    IZ_ASSERT(src != IZ_NULL);
    IZ_UINT16* pSrc = reinterpret_cast<IZ_UINT16*>(src);

    IZ_UINT16 ch = *(pSrc++);

    if (ch == 0) {
        goto __EXIT__;
    }

    if (_IsSurrogate(ch)){
        // Lower Surrogate
        IZ_UINT16 lower = ch;

        IZ_UINT16 upper = *(pSrc++);
        IZ_ASSERT(_IsSurrogate(upper));

        *ret = ((upper << 16) | lower);
    }
    else {
        *ret = ch;
    }

__EXIT__:
    return pSrc;
}

/**
* Get a character code as specified character encode.
*/
void* CStdUtf::GetOneCharCodeAsSJIS(void* src, IZ_UINT* ret)
{
    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, src);

    *ret = 0;

    IZ_ASSERT(src != IZ_NULL);
    IZ_BYTE* pSrc = reinterpret_cast<IZ_BYTE*>(src);

    IZ_BYTE ch = *(pSrc++);

    if (ch == 0) {
        goto __EXIT__;
    }

    if (IsAscii(ch)
        || IsSpace(ch))
    {
        // ASCII or Blank
        *ret = ch;
    }
    else {
        IZ_BYTE ch_1 = *(pSrc++);

        // SJIS
        *ret = ((ch << 8) | ch_1);
    }

__EXIT__:
    return pSrc;
}
