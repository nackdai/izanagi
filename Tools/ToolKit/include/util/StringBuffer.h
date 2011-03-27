#if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_BUFFER_H__)
#define __IZANAGI_TOOL_KIT_UTIL_STRING_BUFFER_H__

#include "izDefs.h"

namespace izanagi {
namespace izanagi_tk {
	// Internal class in Uranus toolkit.
	class CStringBuffer {
		friend class CEnvVarHelper;
		friend class CFileUtility;

	private:
		static IZ_CHAR s_Buf[1024];

		static IZ_CHAR* GetBuf() { return s_Buf; }
		static size_t GetBufSize() { return sizeof(s_Buf); }
		static void ClearBuf() { FILL_ZERO(s_Buf, sizeof(s_Buf)); }

	private:
		CStringBuffer();
		~CStringBuffer();

		NO_COPIABLE(CStringBuffer);
	};
}	// namespace izanagi_tk
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_UTIL_STRING_BUFFER_H__)
