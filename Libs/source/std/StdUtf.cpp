#include "std/StdUtf.h"
#include "std/StdBit.h"

using namespace uranus;

#define __RETURN(b)	if (b) { return; }

namespace {
	// Returns whether character code is surrogate.
	inline UN_BOOL _IsSurrogate(UN_UINT16 ch)
	{
		UN_BOOL ret = (((0xD800 <= ch) && (ch <= 0xDBFF))			// 上位サロゲート
						|| ((0xDC00 <= ch) && (ch <= 0xDFFF)));		// 下位サロゲート
		return ret;
	}

	// Converts character code from surrogate.
	inline UN_UINT _ConvertFromSurrogate(UN_UINT16 upper, UN_UINT16 lower)
	{
		// NOTE
		// n : Character code
		// n' = n - 0x10000
		// n' = yyyyyyyyyyxxxxxxxxxx (2)
		// upper = 110110yyyyyyyyyy
		// lower = 110111xxxxxxxxxx

		UN_UINT ret = (((upper & 0x03ff) << 10) | (lower & 0x03ff));
		ret += 0x10000;
		return ret;
	}
	// Converts character code to surrogate.
	inline UN_UINT _ConvertToSurrogate(UN_UINT code)
	{
		// NOTE
		// n : Character code
		// n' = n - 0x10000
		// n' = yyyyyyyyyyxxxxxxxxxx (2)
		// upper = 110110yyyyyyyyyy
		// lower = 110111xxxxxxxxxx

		UN_UINT tmp = code - 0x10000;

		UN_UINT upper = (0xd800 | (tmp >> 10));
		UN_UINT lower = (0xdc00 | (tmp & 0x3ff));

		UN_UINT ret = ((upper << 16) | lower);
		return ret;
	}
}	// namespace

namespace {
	UN_UINT _ConvertUTF8toUTF16(UN_UINT nCode, UN_UINT* pDstByte)
	{
		union {
			UN_UINT code;
			UN_BYTE ch[4];
		} sChar;

		UN_UINT nDstCode = 0;
		CStdBitOutput cBitOut(&nDstCode, sizeof(nDstCode));

		sChar.code = nCode;

		if (nCode <= 0x7f) {
			// ASCII
			cBitOut.Out(nCode, 16);
			cBitOut.Flush();
		}
		else if ((nCode & 0xe0) == 0xc0) {
			cBitOut.Out(sChar.ch[0], 6);
			cBitOut.Out(sChar.ch[1], 5);
			cBitOut.Flush();
		}
		else if ((nCode & 0xf0) == 0xe0) {
			cBitOut.Out(sChar.ch[0], 6);
			cBitOut.Out(sChar.ch[1], 6);
			cBitOut.Out(sChar.ch[2], 4);
			cBitOut.Flush();
		}
		else if ((nCode & 0xf8) == 0xf0) {
			cBitOut.Out(sChar.ch[0], 6);
			cBitOut.Out(sChar.ch[1], 6);
			cBitOut.Out(sChar.ch[2], 6);
			cBitOut.Out(sChar.ch[3], 3);
			cBitOut.Flush();

			nDstCode = _ConvertToSurrogate(nDstCode);
		}
		else {
			// Not surpported yet...
			UN_ASSERT(UN_FALSE);
		}

		UN_ASSERT(cBitOut.IsValid());

		UN_UINT nDstByte = cBitOut.GetOutByte();
		if (pDstByte != UN_NULL) {
			*pDstByte = nDstByte;
		}

		return nDstCode;
	}

	UN_BYTE* _GetUTF8Code(UN_BYTE* src, UN_UINT* pRet)
	{
		union {
			UN_UINT code;
			UN_BYTE ch[4];
		} sChar;

		UN_UINT nPos = 0;

		UN_BYTE ch = *src;

		if (ch <= 0x7f) {
			return src;
		}

		src++;

		sChar.ch[nPos++] = ch;

		for (;;) {
			ch = *(src++);

			if ((ch & 0xc0) == 0x80) {
				sChar.ch[nPos++] = ch;
			}
			else if ((ch & 0xe0) == 0xc0) {
				sChar.ch[nPos++] = ch;
				break;
			}
			else if ((ch & 0xf0) == 0xe0) {
				sChar.ch[nPos++] = ch;
				break;
			}
			else if ((ch & 0xf8) == 0xf0) {
				sChar.ch[nPos++] = ch;
				break;
			}
			else {
				// Not surpported yet...
				UN_ASSERT(UN_FALSE);
			}
		}

		*pRet = sChar.code;

		return src;
	}
}	// namespace

/**
* Converts UTF8 to UTF16
*/
void CStdUtf::ConvertUtf8ToUtf16(
	void* dst,
	UN_UINT nDstSize,
	UN_CHAR* src)
{
	// 一応
	FILL_ZERO(dst, nDstSize);

	UN_UINT nPos = 0;
	UN_UINT nDstByte = 0;

	UN_BYTE* pSrc = reinterpret_cast<UN_BYTE*>(src);

	for (;;) {
		UN_BYTE ch = *(pSrc++);

		if (ch == 0) {
			return;
		}

		UN_UINT nCode = 0;

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

namespace {
	inline UN_UINT _GetUTF8Byte(UN_UINT32 nChar)
	{
		UN_UINT ret = 0;

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
			UN_ASSERT(UN_FALSE);
		}

		return ret;
	}

	inline UN_CHAR* _ConvertUTF16toUTF8(UN_CHAR* dst, UN_UINT32 nChar)
	{
		if (nChar <= 0x7f) {
			// ASCII
			*(dst++) = (UN_CHAR)nChar;
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
			UN_ASSERT(UN_FALSE);
		}

		return dst;
	}
}	// namespace

/**
* Converts UTF16 to UTF8
*/
void CStdUtf::ConvertUtf16ToUtf8(
	void* dst,
	UN_UINT nDstSize,
	UN_UINT16* src)
{
	// 一応
	FILL_ZERO(dst, nDstSize);

	UN_CHAR* pDst = reinterpret_cast<UN_CHAR*>(dst);

	UN_UINT nPos = 0;
	UN_UINT nDstByte = 0;

	for (;;) {
		UN_UINT16 ch = *(src++);

		if (ch == 0) {
			return;
		}

		UN_UINT nCode = 0;

		if (_IsSurrogate(ch)) {
			// Surrogate

			// Lower Surrogate
			UN_UINT16 lower = ch;

			// Upper Surrogate
			UN_UINT16 upper = *(src++);
			UN_ASSERT(_IsSurrogate(upper));

			// Surrogate -> char code
			nCode = _ConvertFromSurrogate(upper, lower);
		}

		nDstByte = _GetUTF8Byte(nCode);

		__RETURN((nPos + nDstByte) >= nDstSize);
		pDst = _ConvertUTF16toUTF8(pDst, nCode);

		nPos += nDstByte;
	}
}

namespace {
	// Returns whether the character code is SJIS.
	inline UN_BOOL _IsSjis(UN_UINT ch)
	{
		return (((0x81 <= ch) && (ch <= 0x9f))
				|| ((0xe0 <= ch) && (ch <= 0xef)));
	}
}	// namespace

/**
* Get a character code as specified character encode.
*/
void* CStdUtf::GetOneCharCodeAsUTF8(void* src, UN_UINT* ret)
{
	// If ret is NULL, nothing is done.
	VRETURN_VAL(ret != UN_NULL, src);

	*ret = 0;

	UN_ASSERT(src != UN_NULL);
	UN_BYTE* pSrc = reinterpret_cast<UN_BYTE*>(src);

	UN_BYTE ch = *(pSrc++);

	if (ch == 0) {
		goto __EXIT__;
	}

	if (IsAscii(ch)) {
		// ASCII
		*ret = ch;
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
void* CStdUtf::GetOneCharCodeAsUTF16(void* src, UN_UINT* ret)
{
	// If ret is NULL, nothing is done.
	VRETURN_VAL(ret != UN_NULL, src);

	*ret = 0;

	UN_ASSERT(src != UN_NULL);
	UN_UINT16* pSrc = reinterpret_cast<UN_UINT16*>(src);

	UN_UINT16 ch = *(pSrc++);

	if (ch == 0) {
		goto __EXIT__;
	}

	if (_IsSurrogate(ch)){
		// Lower Surrogate
		UN_UINT16 lower = ch;

		UN_UINT16 upper = *(pSrc++);
		UN_ASSERT(_IsSurrogate(upper));

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
void* CStdUtf::GetOneCharCodeAsSJIS(void* src, UN_UINT* ret)
{
	// If ret is NULL, nothing is done.
	VRETURN_VAL(ret != UN_NULL, src);

	*ret = 0;

	UN_ASSERT(src != UN_NULL);
	UN_BYTE* pSrc = reinterpret_cast<UN_BYTE*>(src);

	UN_BYTE ch = *(pSrc++);

	if (ch == 0) {
		goto __EXIT__;
	}

	if (IsAscii(ch)) {
		// ASCII
		*ret = ch;
	}
	else {
		UN_BYTE ch_1 = *(pSrc++);

		// SJIS
		*ret = ((ch << 8) | ch_1);
	}

__EXIT__:
	return pSrc;
}
