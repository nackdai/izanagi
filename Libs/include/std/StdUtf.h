#if !defined(__URANUS_STD_STD_UTF_H__)
#define __URANUS_STD_STD_UTF_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	class CStdUtf {
	public:
		// Converts UTF8 to UTF16
		static void ConvertUtf8ToUtf16(
			void* dst,
			UN_UINT nDstSize,
			UN_CHAR* src);

		// Converts UTF16 to UTF8
		static void ConvertUtf16ToUtf8(
			void* dst,
			UN_UINT nDstSize,
			UN_UINT16* src);

		// Get a character code as specified character encode.
		static void* GetOneCharCodeAsUTF8(void* src, UN_UINT* ret);
		static void* GetOneCharCodeAsUTF16(void* src, UN_UINT* ret);
		static void* GetOneCharCodeAsSJIS(void* src, UN_UINT* ret);

		/**
		* Returns whether character code is ASCII.
		*/
		static inline UN_BOOL IsAscii(UN_UINT ch)
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
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_UTF_H__)
