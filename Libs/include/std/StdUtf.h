#if !defined(__IZANAGI_STD_STD_UTF_H__)
#define __IZANAGI_STD_STD_UTF_H__

#include "izDefs.h"

namespace izanagi {
	/**
	*/
	class CStdUtf {
	public:
		// Converts UTF8 to UTF16
		static void ConvertUtf8ToUtf16(
			void* dst,
			IZ_UINT nDstSize,
			IZ_CHAR* src);

		// Converts UTF16 to UTF8
		static void ConvertUtf16ToUtf8(
			void* dst,
			IZ_UINT nDstSize,
			IZ_UINT16* src);

		// Get a character code as specified character encode.
		static void* GetOneCharCodeAsUTF8(void* src, IZ_UINT* ret);
		static void* GetOneCharCodeAsUTF16(void* src, IZ_UINT* ret);
		static void* GetOneCharCodeAsSJIS(void* src, IZ_UINT* ret);

		/**
		* Returns whether character code is ASCII.
		*/
		static inline IZ_BOOL IsAscii(IZ_UINT ch)
		{
#if 0
			return (((0x21 <= ch) && (ch <= 0x7e))
					|| ((0xa1 <= ch) && (ch <= 0xdf)));
#else
			return (((0x21 <= ch) && (ch <= 0x7e))
					|| ((0xa1 <= ch) && (ch <= 0xdf))
					|| ((0x0a == ch) || (0x0d == ch)));
#endif
		}

		static inline IZ_BOOL IsLineFeed(IZ_UINT ch)
		{
			return ((0x0a == ch) || (0x0d == ch));
		}

		static inline IZ_BOOL IsSpace(IZ_UINT ch)
		{
			return (0x20 == ch);
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STD_UTF_H__)
