#if !defined(__IZANAGI_TOOL_KIT_CHAR_CODE_UTILITY_H__)
#define __IZANAGI_TOOL_KIT_CHAR_CODE_UTILITY_H__

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include "izDefs.h"

namespace izanagi {
namespace tool {
	class CCharCodeUtility {
	private:
		CCharCodeUtility();
		~CCharCodeUtility();

	public:
		/**
		 */
		static IZ_UINT ConvSjisToUtf8(const char* src, void* dst);

		/**
		 */
		static IZ_UINT ConvUtf8ToSjis(const char* src, void* dst);

		// Returns whether the character code is ASCII.
		static IZ_BOOL IsAscii(IZ_UINT ch)
		{
			return ((0x21 <= ch) && (ch <= 0x7e));
		}

		// Returns whether the character code is ASCII.
		static IZ_BOOL IsAsciiExt(IZ_UINT ch)
		{
			return (((0x21 <= ch) && (ch <= 0x7e))
					|| ((0xa1 <= ch) && (ch <= 0xdf)));
		}

		// Returns whether the character code is SJIS.
		static IZ_BOOL IsSjis(IZ_UINT ch)
		{
			return (((0x81 <= ch) && (ch <= 0x9f))
					|| ((0xe0 <= ch) && (ch <= 0xef)));
		}
	};
}	// tool
}	// izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_CHAR_CODE_UTILITY_H__)
