#include "std/StdUtf.h"
#include "std/StdBit.h"
#include "std/StdUtil.h"

using namespace izanagi;

#define __RETURN(b) if (b) { return; }

void* CStdUtf::SFuncIfLowMemory::operator()(
    void* ptr, 
    IZ_UINT size)
{
    if (funcIfLowMem != IZ_NULL)
    {
        return (*funcIfLowMem)(allocator, ptr, size);
    }
    return IZ_NULL;
}

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
    IZ_UINT _ConvertUTF8toUnicode(IZ_UINT code, IZ_UINT* pDstByte)
    {
        union {
            IZ_UINT code;
            IZ_BYTE ch[4];
        } sChar;

        IZ_UINT nDstCode = 0;
        CStdBitOutput cBitOut(&nDstCode, sizeof(nDstCode));

        sChar.code = code;

        IZ_UINT bytes = 0;

        if (code <= 0x7f) {
            // ASCII
            nDstCode = code & (0xffff);
            bytes = 1;
        }
        else
        {
            if (((code >> 24) & 0xf8) == 0xf0) {
                nDstCode = sChar.ch[3] & 0x07;
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);

                nDstCode = _ConvertToSurrogate(nDstCode);

                bytes = 4;
            }
            else if (((code >> 16) & 0xf0) == 0xe0) {
                nDstCode = sChar.ch[2] & 0x0f;
                nDstCode = (nDstCode << 6) | (sChar.ch[1] & 0x3f);
                nDstCode = (nDstCode << 6) | (sChar.ch[0] & 0x3f);
                bytes = 2;
            }
            else if (((code >> 8) & 0xe0) == 0xc0) {
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

    IZ_BYTE* _GetUTF8Code(const IZ_BYTE* src, IZ_UINT* pRet)
    {
        IZ_BYTE* ptrSrc = const_cast<IZ_BYTE*>(src);

        IZ_BYTE ch = *(ptrSrc++);

        if (ch <= 0x7f) {
            return ptrSrc;
        }

        IZ_UINT count = 1;
        IZ_UINT code = ch;

        for (;;) {
#if 0
            ch = *(ptrSrc++);

            IZ_ASSERT(ch > 0x7f);

            if ((ch & 0xc0) == 0x80
                || ((ch & 0xe0) == 0xc0)
                || ((ch & 0xf0) == 0xe0)
                || ((ch & 0xf8) == 0xf0))
            {
                IZ_ASSERT(count <= 4);

                IZ_UINT tmp = ch;
                code = ((tmp << (count * 8)) | code);

                if (((ch & 0xe0) == 0xc0)
                    || ((ch & 0xf0) == 0xe0)
                    || ((ch & 0xf8) == 0xf0))
                {
                    break;
                }
            }
#else
            ch = *ptrSrc;

            if (ch <= 0x7f)
            {
                break;
            }

            if ((ch & 0xc0) == 0x80
                || ((ch & 0xe0) == 0xc0)
                || ((ch & 0xf0) == 0xe0)
                || ((ch & 0xf8) == 0xf0))
            {
                if (((ch & 0xe0) == 0xc0)
                    || ((ch & 0xf0) == 0xe0)
                    || ((ch & 0xf8) == 0xf0))
                {
                    break;
                }

                code = ((code << 8) | ch);
                ptrSrc++;
            }
#endif
            else {
                // Not surpported yet...
                IZ_ASSERT(IZ_FALSE);
            }

            count++;
        }

        *pRet = code;

        return ptrSrc;
    }
}   // namespace

/**
* Converts UTF8 to Unicode
*/
IZ_UINT CStdUtf::ConvertUtf8ToUnicode(
    void** dst,
    IZ_UINT nDstSize,
    const void* src)
{
    SFuncIfLowMemory func = 
    {
        IZ_NULL,
        IZ_NULL,
    };

    return ConvertUtf8ToUnicode(
        dst,
        nDstSize,
        src,
        func);
}

IZ_UINT CStdUtf::ConvertUtf8ToUnicode(
    void** dst,
    IZ_UINT nDstSize,
    const void* src,
    SFuncIfLowMemory& func)
{
    // 一応
    //FILL_ZERO(dst, nDstSize);

    IZ_UINT8* pDst = reinterpret_cast<IZ_UINT8*>(*dst);

    IZ_UINT nPos = 0;
    IZ_UINT nDstByte = 0;

    IZ_UINT count = 0;

    IZ_BYTE* pSrc = CONST_CAST(IZ_BYTE*, void*, src);

    for (;;) {
        IZ_BYTE ch = *pSrc;

        if (ch == 0) {
            break;
        }

        IZ_UINT code = 0;
        pSrc = _GetUTF8Code(pSrc, &code);
        code = _ConvertUTF8toUnicode(code, &nDstByte);

        if ((nPos + nDstByte) >= nDstSize)
        {
            // 出力先メモリが足りない場合
            nDstSize *= 2;
            IZ_UINT8* tmp = (IZ_UINT8*)func(dst, nDstSize);

            if (tmp != IZ_NULL)
            {
                pDst = tmp + nPos;
            }
            else
            {
                break;
            }
        }

        memcpy(pDst + nPos, &code, nDstByte);
        nPos += nDstByte;

        count++;
    }

    if (nPos <= nDstByte)
    {
        pDst[nPos + 1] = 0;
    }

    return count;
}

IZ_UINT CStdUtf::ConvertUtf8ToUnicode(IZ_UINT code)
{
    IZ_UINT bytes = 0;
    IZ_UINT ret = _ConvertUTF8toUnicode(code, &bytes);
    return ret;
}

/**
* Converts UTF16 to Unicode
*/
IZ_UINT CStdUtf::ConvertUtf16ToUnicode(
    void** dst,
    IZ_UINT nDstSize,
    const void* src)
{
    SFuncIfLowMemory func = 
    {
        IZ_NULL,
        IZ_NULL,
    };

    return ConvertUtf16ToUnicode(
        dst,
        nDstSize,
        src,
        func);
}

IZ_UINT CStdUtf::ConvertUtf16ToUnicode(
    void** dst,
    IZ_UINT nDstSize,
    const void* src,
    SFuncIfLowMemory& func)
{
    // 一応
    //FILL_ZERO(dst, nDstSize);

    IZ_UINT8* pDst = reinterpret_cast<IZ_UINT8*>(*dst);
    IZ_UINT16* ptrSrc = CONST_CAST(IZ_UINT16*, void*, src);

    IZ_UINT nPos = 0;
    IZ_UINT nDstByte = 0;

    IZ_UINT count = 0;

    for (;;) {
        IZ_UINT16 ch = *(ptrSrc++);

        if (ch == 0) {
            break;
        }

        IZ_UINT code = ch;
        IZ_UINT bytes = 2;

        if (_IsSurrogate(ch)) {
            // Surrogate

            // Lower Surrogate
            IZ_UINT16 lower = ch;

            // Upper Surrogate
            IZ_UINT16 upper = *(ptrSrc++);
            IZ_ASSERT(_IsSurrogate(upper));

            // Surrogate -> char code
            code = _ConvertFromSurrogate(upper, lower);

            bytes = 4;
        }

        if ((nPos + bytes) >= nDstSize)
        {
            // 出力先メモリが足りない場合
            nDstSize *= 2;
            IZ_UINT8* tmp = (IZ_UINT8*)func(dst, nDstSize);

            if (tmp != IZ_NULL)
            {
                pDst = tmp + nPos;
            }
            else
            {
                break;
            }
        }

        IZ_UINT8* ptrCode = reinterpret_cast<IZ_UINT8*>(&code);

        for (IZ_UINT i = 0; i < bytes; i++)
        {
            pDst[nPos + i] = ptrCode[i];
        }

        nPos += bytes;
        count++;
    }

    if (nPos <= nDstSize)
    {
        pDst[nPos + 1] = 0;
    }

    return count;
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
* Get a character code as specified character ecode.
*/
void* CStdUtf::GetOneCharCodeAsUTF8(const void* src, IZ_UINT* ret)
{
    IZ_ASSERT(src != IZ_NULL);
    IZ_BYTE* pSrc = CONST_CAST(IZ_BYTE*, void*, src);

    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, pSrc);

    *ret = 0;

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
* Get a character code as specified character ecode.
*/
void* CStdUtf::GetOneCharCodeAsUnicode(const void* src, IZ_UINT* ret)
{
    IZ_ASSERT(src != IZ_NULL);
    IZ_UINT16* pSrc = CONST_CAST(IZ_UINT16*, void*, src);

    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, pSrc);

    *ret = 0;

    IZ_UINT16 ch = *(pSrc++);

    if (ch == 0) {
        goto __EXIT__;
    }

    if (_IsSurrogate(ch)){
        // Lower Surrogate
        IZ_UINT16 lower = ch;

        IZ_UINT16 upper = *(pSrc++);
        IZ_ASSERT(_IsSurrogate(upper));

        *ret = _ConvertFromSurrogate(upper, lower);
    }
    else {
        *ret = ch;
    }

__EXIT__:
    return pSrc;
}

/**
* Get a character code as specified character ecode.
*/
void* CStdUtf::GetOneCharCodeAsSJIS(const void* src, IZ_UINT* ret)
{
    IZ_ASSERT(src != IZ_NULL);
    IZ_BYTE* pSrc = CONST_CAST(IZ_BYTE*, void*, src);

    // If ret is NULL, nothing is done.
    VRETURN_VAL(ret != IZ_NULL, pSrc);

    *ret = 0;

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

void* CStdUtf::GetOneCharCodeAsUTF8(const void* src, IZ_UINT* ret, IZ_UINT* bytes)
{
    void* retPtr = GetOneCharCodeAsUTF8(src, ret);
    if (ret != 0 && bytes != IZ_NULL)
    {
        *bytes = static_cast<IZ_UINT>(CStdUtil::GetPtrDistance((const IZ_UINT8*)src, (const IZ_UINT8*)retPtr));
    }
    return retPtr;
}

void* CStdUtf::GetOneCharCodeAsUnicode(const void* src, IZ_UINT* ret, IZ_UINT* bytes)
{
    void* retPtr = GetOneCharCodeAsUnicode(src, ret);
    if (ret != 0 && bytes != IZ_NULL)
    {
        *bytes = static_cast<IZ_UINT>(CStdUtil::GetPtrDistance((const IZ_UINT8*)src, (const IZ_UINT8*)retPtr));
    }
    return retPtr;
}

void* CStdUtf::GetOneCharCodeAsSJIS(const void* src, IZ_UINT* ret, IZ_UINT* bytes)
{
    void* retPtr = GetOneCharCodeAsSJIS(src, ret);
    if (ret != 0 && bytes != IZ_NULL)
    {
        *bytes = static_cast<IZ_UINT>(CStdUtil::GetPtrDistance((const IZ_UINT8*)src, (const IZ_UINT8*)retPtr));
    }
    return retPtr;
}
