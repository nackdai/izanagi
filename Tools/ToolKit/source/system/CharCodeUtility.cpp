#include <vector>
#include "system/CharCodeUtility.h"

using namespace izanagi;
using namespace tool;

//
IZ_UINT CCharCodeUtility::ConvSjisToUtf8(const char* src, void* dst)
{
	// Convert SJIS -> UTF-16
	const int sizeUtf16 = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, -1, NULL, 0);

	std::vector<BYTE> utf16(sizeUtf16 * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, -1, (LPWSTR)&utf16[0], sizeUtf16);

	// Convert UTF-16 -> UTF-8
	const int sizeUtf8 = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&utf16[0], -1, NULL, 0, NULL, NULL);

	if (dst == IZ_NULL) {
		return sizeUtf8;
	}

	std::vector<BYTE> utf8(sizeUtf8 * 2);
	::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&utf16[0], -1, (LPSTR)&utf8[0], sizeUtf8, NULL, NULL);

	IZ_UINT size = lstrlen((char*)&utf8[0]);
	memcpy(dst, &utf8[0], size);

	return size;
}

//
IZ_UINT CCharCodeUtility::ConvUtf8ToSjis(const char* src, void* dst)
{
	// Convert UTF-8 -> UTF-16
	const int sizeUtf16 = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0);

	std::vector<BYTE> utf16(sizeUtf16 * 2 + 2);
	::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)&utf16[0], sizeUtf16);

	// Convert UTF-16 -> SJIS
	const int sizeSjis = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)&utf16[0], -1, NULL, 0, NULL, NULL);

	if (dst == IZ_NULL) {
		return sizeSjis;
	}

	std::vector<BYTE> sjis(sizeSjis * 2);
	::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)&utf16[0], -1, (LPSTR)&sjis[0], sizeSjis, NULL, NULL);

	IZ_UINT size = lstrlen((char*)&sjis[0]);
	memcpy(dst, &sjis[0], size);

	return size;
}
